#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Tile.h"
#include "Tank.h"
#include "Bullet.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILE_MAP 7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1

#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define	OBJECT_TYPE_TANK	4
#define	OBJECT_TYPE_GOLEM	5
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);
	
	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	
	
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
	
	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	

}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_TANK:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CTank(x, y);
		player = (CTank*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	//case OBJECT_TYPE_GOLEM: obj = new CGolem(); break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_TILE_MAP(string line)
{
	int curentMapId = (CGame::GetInstance()->GetSceneId());
	LPDIRECT3DTEXTURE9 tilesheet = CTextures::GetInstance()->Get(curentMapId);

	//Texture IDs and SceneIDs are identical (this is not good design but yeah...

	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	tilesheet->GetLevelDesc(level, &surfaceDesc);

	//int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / CTile::TILEWIDTH;

	vector<string> map_tokens = split(line);

	for (int i = 0; i < map_tokens.size(); i++)
	{
		RECT rectTile;
		int index = atoi(map_tokens[i].c_str());
		rectTile.left = (index % nums_colToRead) * CTile::TILEWIDTH;
		rectTile.top = (index / nums_colToRead) * CTile::TILEHEIGHT;
		rectTile.right = rectTile.left + CTile::TILEWIDTH;
		rectTile.bottom = rectTile.top + CTile::TILEHEIGHT;
		int x, y;
		x = i * CTile::TILEHEIGHT;
		y = this->offset_y;
		CTile* tile = new CTile(x, y, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		tiledMap.push_back(tile);
		
	}
	this->offset_y += CTile::TILEHEIGHT;
	
	

	
}


void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		{

			//
			// data section
			//
			switch (section)
			{
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_TILE_MAP: _ParseSection_TILE_MAP(line); break;
			}
		}
	}

	f.close();


	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->visible==true)
			objects[i]->Update(dt, &coObjects);
		/*else {
			objects.pop_back();
			delete objects[i];
		}*/

	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	//cx -= game->GetScreenWidth() / 2;
	//cy -= game->GetScreenHeight() / 2;
	if (cx + game->GetScreenWidth() / 2 >= scene_width - 1)
		cx = (scene_width - 1) - game->GetScreenWidth();
	else
	{
		if (cx < game->GetScreenWidth() / 2)
		{
			cx = 0;
		}
		else
			cx -= game->GetScreenWidth() / 2;
	}


	if (cy + game->GetScreenHeight() / 2 >= (scene_height -1))
		cy = (scene_height -1) - game->GetScreenHeight();
	else
	{
		if (cy < game->GetScreenHeight() / 2)
		{
			cy = 0;
		}
		else
			cy -= game->GetScreenHeight() / 2;
	}

	game->SetCamPos(cx, cy);

}

void CPlayScene::Render()
{


	//Render Tiles
	for (int i = 0; i < tiledMap.size(); i++)
		tiledMap[i]->Render();
	//Object Rendering
	for (int i = 0; i < objects.size(); i++)

	{
		if (objects[i]->visible == true)
		{
			objects[i]->Render();
			objects[i]->RenderBoundingBox();
		}
	}
	
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CTank* tank = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_UP:
		tank->y += 16.0f;
		tank->SetDimension(TANK_NORMAL_WIDTH, TANK_NORMAL_HEIGHT);
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CTank* tank = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (tank->GetState() == TANK_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_UP)) {
		tank->nx = 1;
		if (tank->vy >= 0.05f) {
				tank->SetState(TANK_STATE_JUMP_RIGHT);
		}
		else 
			tank->SetState(TANK_STATE_WALKING_RIGHT);		
	}
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_UP)) {
		tank->nx = -1;
		if (tank->vy >= 0.05f) {
				tank->SetState(TANK_STATE_JUMP_LEFT);
		}
		else
			tank->SetState(TANK_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_UP)) {
		if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_RIGHT)) {
			tank->SetState(TANK_STATE_UP_GUN_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_LEFT)) {
			tank->SetState(TANK_STATE_UP_GUN_WALKING_LEFT);
		}
		else {
			if (tank->nx == -1)
				tank->SetState(TANK_STATE_UP_GUN_LEFT);
			else
				tank->SetState(TANK_STATE_UP_GUN_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_SPACE)) {
		//if(tank->vy == )
	}
	else
	{	
		if (tank->nx == -1)
			tank->SetState(TANK_STATE_IDLE_LEFT);
		else
			tank->SetState(TANK_STATE_IDLE_RIGHT);

	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	//CMario mario = ((CPlayScene)scence)->GetPlayer();
	CTank* tank = ((CPlayScene*)scence)->GetPlayer();
	vector<LPGAMEOBJECT> *objects = ((CPlayScene*)scence)->GetObjects();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	switch (KeyCode)
	{
	case DIK_X: 
		int width, height;
		tank->GetDimension(width, height);
		if (height == TANK_NORMAL_HEIGHT)
		{
			if (tank->nx == -1)
			{

				CBullet* bullet = new CBullet(2, BULLET_STATE_FLYING_LEFT);
				float x1, y1;
				tank->GetPosition(x1, y1);
				bullet->SetPosition(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				bullet->SetStartPositon(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
			else
			{
				CBullet* bullet = new CBullet(2, BULLET_STATE_FLYING_RIGHT);
				float x1, y1;
				tank->GetPosition(x1, y1);
				bullet->SetPosition(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				bullet->SetStartPositon(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
		}
		else {
			CBullet* bullet = new CBullet(2, BULLET_STATE_FLYING_UP);
			float x1, y1;
			tank->GetPosition(x1, y1);
			bullet->SetPosition(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT);
			bullet->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT);
			LPANIMATION_SET ani_set = animation_sets->Get(6);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);
		}

		break;
	case DIK_SPACE:
		if (tank->vy <= 0.05f && tank->vy >= 0) {
			if (tank->nx == -1)
				tank->SetState(TANK_STATE_JUMP_IDLE_LEFT);
			else
				tank->SetState(TANK_STATE_JUMP_IDLE_RIGHT);
		}
		break;
	case DIK_A:
		tank->SetState(TANK_STATE_IDLE_RIGHT);
		tank->SetPosition(100.0f, 0.0f);
		tank->SetSpeed(0, 0);
		break;
	case DIK_UP:
		int w, h;
		tank->GetDimension(w, h);
		if (h == TANK_NORMAL_HEIGHT)
			tank->y -= 16.0f;
		tank->SetDimension(TANK_UP_GUN_WIDHT, TANK_UP_GUN_HEIGHT);
		if (tank->nx == -1)
			tank->SetState(TANK_STATE_UPING_GUN_LEFT);
		else
			tank->SetState(TANK_STATE_UPING_GUN_RIGHT);
		break;
	}
}
