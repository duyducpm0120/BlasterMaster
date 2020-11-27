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
#include "Butterfly.h"
#include "PlayScenceKeyHandler.h"
#include "Destroyed.h"
#include "Item.h"
#include "Worm.h"
#include "Bee.h"
#include "Flame.h"
#include "Sophia.h"
#include "Boss.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	CGame *game = CGame::GetInstance();
	this->playerHealth = game->GetHealth();
	this->playerPower = game->GetPower();
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
#define OBJECT_TYPE_BULLET	6
#define OBJECT_TYPE_BUTTERFLY	7
#define OBJECT_TYPE_WORM	12
#define OBJECT_TYPE_BEE	13
#define OBJECT_TYPE_FLAME	14
#define OBJECT_TYPE_PORTAL	50
#define OBJECT_TYPE_BOSS	16

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
		tank = dynamic_cast<CTank*> (obj);
		player->health = *this->playerHealth;
		player->damage = *this->playerPower;

		DebugOut(L"[INFO] Player object created!\n");
		hud = new HUD(dynamic_cast<CTank*> (player)->GetHealth(), player->GetDamage());
		break;
	case 15:
		obj = new CSophia(x, y);
		player = (CSophia*)obj;
		player->health = *this->playerHealth;
		player->damage = *this->playerPower;
		hud = new HUD(player->GetHealth(), player->GetDamage());
		break;
	case OBJECT_TYPE_GOLEM: obj = new CGolem();
		dynamic_cast<CGolem*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BUTTERFLY: {
		obj = new CButterfly(); 
		dynamic_cast<CButterfly*>(obj)->SetPlayer(dynamic_cast<CTank*> (this->GetPlayer()));
		break; 
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_FLAME: obj = new CFlame(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_WORM: obj = new CWorm();
		dynamic_cast<CWorm*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BEE: obj = new CBee();
		dynamic_cast<CBee*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BOSS: obj = new CBoss();
		dynamic_cast<CBoss*>(obj)->SetStartPosition(x, y);
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

void CPlayScene::CallDestroyed(CGameObject* object)
{
	if (!dynamic_cast<CDestroyed*>(object)) {

		if (dynamic_cast<CBullet*>(object)) {
			CDestroyed* destroyed = new CDestroyed(1);
			destroyed->SetPosition(object->x, object->y);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 1
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
		}
		else if (dynamic_cast<CTank*>(object)) {
			CDestroyed* destroyed = new CDestroyed(3);
			destroyed->SetPosition(object->x - 19, object->y - 30);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 3
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
		}
		else if (!dynamic_cast<CItem*>(object)) {
			if (object->IsEnemy() == true)
			{
				srand(time(NULL));
				int n = rand() % 2;
				if (n == 1)
				{
					int type = rand() % 3;
					CItem* item = new CItem(type);
					item->SetPosition(object->x, object->y - 10);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(11);		//call a Destroyed type 2
					item->SetAnimationSet(ani_set);
					objects.push_back(item);
				}

			}
			CDestroyed* destroyed = new CDestroyed(2);
			destroyed->SetPosition(object->x, object->y);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 2
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
		}

	}
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
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;
		}
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

	if (dynamic_cast<CTank*>(player)) {
		for (int i = 0; i < objects.size(); i++) {
			if (dynamic_cast<CSophia*>(objects.at(i)))
			{
				dynamic_cast<CSophia*>(objects.at(i))->vy -= 0.01f;
				objects.erase(objects.begin() + i);
				break;
			}

		}
	}
	CGame* game = CGame::GetInstance();
	grid->Clear();

	for (size_t i = 0; i < objects.size(); i++)
	{
		//coObjects.push_back(objects[i]);
		if (objects.at(i)->visible == false)
		{
			CallDestroyed(objects.at(i));
			objects.erase(objects.begin() + i);
		}

	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		//coObjects.push_back(objects[i]);
		grid->Add(objects[i]);
		objects[i]->setToUpdate(true);

	}
	
	updateObject.clear();
	float left, top, right, bottom;
	game->GetCameraBoundingBox(left, top, right, bottom);
	grid->GetUpdateObjects(updateObject, left, top, right, bottom);
	DebugOut(L"Size of update array %d\n", updateObject.size());
	DebugOut(L"Size of object array %d\n", objects.size());

	for (size_t i = 1; i < updateObject.size(); i++)
	{
		if (player == NULL) return;
		if (updateObject[i]->visible == true && updateObject[i]->isToUpdate)
			updateObject[i]->Update(dt, &updateObject);

	}
	/*for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == true)
			objects[i]->Update(dt, &coObjects);
		else {
			if (!dynamic_cast<CDestroyed*>(objects.at(i))) {

				if (dynamic_cast<CBullet*>(objects.at(i))) {
					CDestroyed* destroyed = new CDestroyed(1);
					destroyed->SetPosition(objects.at(i)->x, objects.at(i)->y);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 1
					destroyed->SetAnimationSet(ani_set);
					objects.push_back(destroyed);
				}
				else if (dynamic_cast<CTank*>(objects.at(i))) {
					CDestroyed* destroyed = new CDestroyed(3);
					destroyed->SetPosition(objects.at(i)->x - 19, objects.at(i)->y - 30);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 3
					destroyed->SetAnimationSet(ani_set);
					objects.push_back(destroyed);
				}
				else if(!dynamic_cast<CItem*>(objects.at(i))) {
					if (objects[i]->IsEnemy() == true)
					{
						srand(time(NULL));
						int n = rand() % 2 ;					
						if (n == 1)
						{
							int type = rand() % 3 ;
							CItem* item = new CItem(type);
							item->SetPosition(objects[i]->x, objects[i]->y - 10);
							CAnimationSets* animation_sets = CAnimationSets::GetInstance();
							LPANIMATION_SET ani_set = animation_sets->Get(11);		//call a Destroyed type 2
							item->SetAnimationSet(ani_set);
							objects.push_back(item);
						}

					}
					CDestroyed* destroyed = new CDestroyed(2);
					destroyed->SetPosition(objects.at(i)->x, objects.at(i)->y);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 2
					destroyed->SetAnimationSet(ani_set);
					objects.push_back(destroyed);
				}

			}
			objects.erase(objects.begin() + i);//erase obj at (i)
			return;
		}
		
	}
	*/
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	
	if (dynamic_cast<CTank*>(player)) {
		int w, h;
		player->GetDimension(w, h);
		if (h == TANK_UP_GUN_HEIGHT)
			cy += 16;
	}

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

	if (scene_height <= 270) {
	
		cy = 0;
	}
	else {
		if (cy + game->GetScreenHeight() / 2 >= (scene_height - 1))
			cy = (scene_height - 1) - game->GetScreenHeight();
		else
		{
			if (cy < game->GetScreenHeight() / 2)
			{
				cy = 0;
			}
			else
				cy -= game->GetScreenHeight() / 2;
		}
	}

	game->SetCamPos(cx, cy);



	//updateObject.clear();
	//float left, top, right, bottom;
	//game->GetCameraBoundingBox(left, top, right, bottom);
	//grid->GetUpdateObjects(updateObject, left, top, right, bottom);
	DebugOut(L"Size of object array %d\n", objects.size());
	hud->Update(cx+5, cy, player->GetHealth(), player->GetDamage());

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
			//objects[i]->RenderBoundingBox();
		}
	}
	//hud->Update(cx + 5, cy, player->GetHealth(), player->GetDamage());
	hud->Render(player);
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

void CPlayScene::SetPlayer(CPlayer* player)
{
	this->player = player;
}


