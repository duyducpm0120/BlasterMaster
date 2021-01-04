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
#include "OHSophia.h"
#include "Ladder.h"
#include "AutoRunPortal.h"
#include "Robot.h"
#include "Skull.h"
#include "Face.h"
#include "Quadcannon.h"
#include "Blink.h"
#include "Eye.h"
#include "Ball.h"
#include "Jumper.h"
#include "BossArm.h"
#include "StupidHead.h"
#include "Flyingclaw.h"
#include "Choose.h"
#include "Sound.h"
#include "Text.h"
#include "Thunder.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	isCameraAutorun = false;
	CGame *game = CGame::GetInstance();
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
#define OBJECT_TYPE_SOPHIA	15
#define OBJECT_TYPE_PORTAL	100
#define OBJECT_TYPE_AUTORUNPORTAL	101
#define OBJECT_TYPE_BOSS	16
#define OBJECT_TYPE_OHSOPHIA	20
#define OBJECT_TYPE_LADDER	21
#define OBJECT_TYPE_ROBOT 50
#define OBJECT_TYPE_SKULL 52
#define OBJECT_TYPE_FACE 53
#define OBJECT_TYPE_JUMPER 55
#define OBJECT_TYPE_QUADCANNON 56
#define OBJECT_TYPE_BLINK 57
#define OBJECT_TYPE_EYE 58
#define OBJECT_TYPE_BALL 60
#define OBJECT_TYPE_FLYINGCLAW 62
#define OBJECT_TYPE_STUPIDHEAD	63
#define OBJECT_TYPE_ITEM	64
#define OBJECT_TYPE_CHOOSE	70
#define MAX_SCENE_LINE 1024
#define BOSS_APPEAR_TIME	200
#define ENDING_COUNT_TIME	200

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
		this->tank = dynamic_cast<CTank*>( player);
		int health, power;
		bool enableRocket, enableThunder;
		if (CGame::GetInstance()->GetPlayer()) {
			CGame::GetInstance()->GetPlayerProperties(health, power, enableRocket, enableThunder);
			dynamic_cast<CTank*>(player)->SetPlayerProperties(health, power, enableRocket, enableThunder);			
		}
		CGame::GetInstance()->SetPlayer(player);
		DebugOut(L"[INFO] Player object created!\n");
		hud = new HUD(dynamic_cast<CTank*> (player)->GetHealth(), player->GetDamage());
		break;
	case OBJECT_TYPE_OHSOPHIA:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] player was created before!\n");
			return;
		}
		obj = new COHSophia(x, y);
		player = (COHSophia*)obj;
		player->health = CGame::GetInstance()->GetPlayer()->GetHealth();
		player->damage = CGame::GetInstance()->GetPlayer()->GetDamage();
		CGame::GetInstance()->SetPlayer(player);
		DebugOut(L"[INFO] Player object created!\n");
		hud = new HUD(dynamic_cast<COHSophia*> (player)->GetHealth(), player->GetDamage());
		break;
	case OBJECT_TYPE_SOPHIA:
		obj = new CSophia(x, y);
		player = (CSophia*)obj;
		player->health = *this->playerHealth;
		player->damage = *this->playerPower;
		CGame::GetInstance()->SetPlayer(player);
		hud = new HUD(player->GetHealth(), player->GetDamage());
		break;
	case OBJECT_TYPE_GOLEM: obj = new CGolem();
		dynamic_cast<CGolem*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_STUPIDHEAD: obj = new CStupidHead();
		dynamic_cast<CStupidHead*>(obj)->SetPosition(x, y);
		dynamic_cast<CStupidHead*>(obj)->SetTarget(player);
		break;
	case OBJECT_TYPE_BUTTERFLY: {
		obj = new CButterfly(); 
		dynamic_cast<CButterfly*>(obj)->SetPlayer(dynamic_cast<CTank*> (this->GetPlayer()));
		break; 
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_CHOOSE: obj = new CChoose();
		this->choose = (CChoose*) obj;
		break;
	case OBJECT_TYPE_FLAME: obj = new CFlame(); break;
	case OBJECT_TYPE_LADDER: obj = new CLadder(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_AUTORUNPORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		float targetx = atof(tokens[6].c_str());
		float targety = atof(tokens[7].c_str());
		obj = new CAutoRunPortal(x, y, r, b, targetx,targety);
	}
	break;
	case OBJECT_TYPE_ITEM:
	{	int t = atoi(tokens[4].c_str());
		obj = new CItem(t); 
	}
		break;
	case OBJECT_TYPE_WORM: obj = new CWorm();
		dynamic_cast<CWorm*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BEE: obj = new CBee();
		dynamic_cast<CBee*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BOSS: 
	{
	obj = new CBoss();
	dynamic_cast<CBoss*>(obj)->SetStartPosition(x, y);
	BossArm *left = new BossArm(dynamic_cast<CBoss*>(obj), TYPE_LEFT_CLAW);
	objects.push_back(left);
	BossArm* right = new BossArm(dynamic_cast<CBoss*>(obj), TYPE_RIGHT_CLAW);
	objects.push_back(right);
	break;
	}
	case OBJECT_TYPE_ROBOT: obj = new CRobot();
		dynamic_cast<CRobot*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_SKULL: obj = new CSkull();
		dynamic_cast<CSkull*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_FACE: obj = new CFace();
		dynamic_cast<CFace*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_QUADCANNON: obj = new CQuadcannon();
		dynamic_cast<CQuadcannon*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BLINK: obj = new CBlink();
		dynamic_cast<CBlink*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_EYE: obj = new CEye();
		dynamic_cast<CEye*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_FLYINGCLAW: obj = new CFlyingclaw();
		dynamic_cast<CFlyingclaw*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_JUMPER: obj = new CJumper();
		dynamic_cast<CJumper*>(obj)->SetStartPosition(x, y);
		break;
	case OBJECT_TYPE_BALL: obj = new CBall();
		dynamic_cast<CBall*>(obj)->SetStartPosition(x, y);
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
	int curentMapId = (CGame::GetInstance()->GetCurrentSceneId());
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
	if (!dynamic_cast<CDestroyed*>(object) && !dynamic_cast<CThunder*>(object)) {

		if (dynamic_cast<CBullet*>(object)) {
			CDestroyed* destroyed = new CDestroyed(DESTROYED_TYPE_BULLET);
			destroyed->SetPosition(object->x, object->y);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 1
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
			Sound::GetInstance()->Play("PlayerBulletHitBrick", 0, 1);
		}
		else if (dynamic_cast<CTank*>(object)) {
			CDestroyed* destroyed = new CDestroyed(DESTROYED_TYPE_TANK);
			destroyed->SetPosition(object->x - 19, object->y - 30);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 3
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
			Sound::GetInstance()->Play("TankDie", 0, 1);
			Sound::GetInstance()->Play("LifeLost", 0, 1);
		}
		else if (dynamic_cast<CSophia*>(object)) {
			CDestroyed* destroyed = new CDestroyed(DESTROYED_TYPE_SOPHIA);
			destroyed->SetPosition(object->x, object->y);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 3
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
			Sound::GetInstance()->Play("LifeLost", 0, 1);
		}
		else if (dynamic_cast<COHSophia*>(object)) {
			CDestroyed* destroyed = new CDestroyed(DESTROYED_TYPE_OHSOPHIA);
			destroyed->SetPosition(object->x, object->y);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 3
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
			Sound::GetInstance()->Play("LifeLost", 0, 1);
		}
		else if (!dynamic_cast<CItem*>(object)) {
			if (object->IsEnemy() == true && !dynamic_cast<CButterfly*>(object) && !dynamic_cast<CStupidHead*>(object) && !dynamic_cast<BossArm*>(object) && !dynamic_cast<CBoss*>(object))
			{
				srand(time(NULL));
				int n = rand() % 3;
				if (n == ITEM_TYPE_HEALTH)
				{
					int type = n;
					CItem* item = new CItem(type);
					item->SetPosition(object->x, object->y - 10);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(11);		//call a Destroyed type 2
					item->SetAnimationSet(ani_set);
					objects.push_back(item);
				}

			}
			else if (object->IsEnemy() == true && dynamic_cast<CButterfly*>(object)) {
			
				if (CGame::GetInstance()->GetPlayer() && !dynamic_cast<CTank*> (CGame::GetInstance()->GetPlayer())->GetEnableRocket())
				{
					CItem* item = new CItem(ITEM_TYPE_ENABLE_ROCKET);
					item->SetPosition(object->x, object->y - 10);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(11);
					item->SetAnimationSet(ani_set);
					dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects()->push_back(item);
				}
			}
			else if (object->IsEnemy() == true && dynamic_cast<CStupidHead*>(object)) {

				if (CGame::GetInstance()->GetPlayer()&& !dynamic_cast<CTank*> (CGame::GetInstance()->GetPlayer())->GetEnableThunder())
				{
					CItem* item = new CItem(ITEM_TYPE_THUNDER);
					item->SetPosition(object->x, object->y - 10);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(11);
					item->SetAnimationSet(ani_set);
					dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects()->push_back(item);
				}
			}
			CDestroyed* destroyed = new CDestroyed(DESTROYED_TYPE_OBJECT);
			destroyed->SetPosition(object->x, object->y);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(9);		//call a Destroyed type 2
			destroyed->SetAnimationSet(ani_set);
			objects.push_back(destroyed);
			Sound::GetInstance()->Play("Enemydie", 0, 1);
		}

	}
}


void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	isLoaded = true;
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
void CPlayScene::Load(vector<LPGAMEOBJECT> objects)
{
}


void CPlayScene::Update(DWORD dt)
{

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	if (player != NULL) {
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
		CGame::GetInstance()->SetPlayer(this->player);
	}
	CGame* game = CGame::GetInstance();
	grid->Clear();


	for (size_t i = 0; i < objects.size(); i++)
	{
		//coObjects.push_back(objects[i]);
		if (objects.at(i)->visible == false)
		{
			//DebugOut(L"Objects size before: %d \n", objects.size());
			CallDestroyed(objects.at(i));
			//delete objects.at(i);
			//objects.at(i) = NULL;
			objects.erase(objects.begin() + i);
			//DebugOut(L"Objects size after: %d \n \n \n", objects.size());
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
	//DebugOut(L"Size of update array %d\n", updateObject.size());
	//DebugOut(L"Size of object array %d\n", objects.size());

	for (size_t i = 1; i < updateObject.size(); i++)
	{
		if (player == NULL) return;
		if (updateObject[i]->visible == true && updateObject[i]->isToUpdate)
			updateObject[i]->Update(dt, &updateObject);
	}
	
	

	if (this->id != 10 ) {
		// Update camera to follow player
		float cx, cy;
		player->GetPosition(cx, cy);
		if (dynamic_cast<CTank*>(player)) {
			int w, h;
			player->GetDimension(w, h);
			if (h == TANK_UP_GUN_HEIGHT)
				cy += 16;
		}
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
		if(CGame::GetInstance()->GetPlayer())
			hud->Update(cx + 5, cy, CGame::GetInstance()->GetPlayer()->GetHealth(), CGame::GetInstance()->GetPlayer()->GetDamage());
	}
	else if(id ==10) {
		if (isCamSetInit == false) {
			game->SetCamPos(0, 1296);
			isCamSetInit = true;
		}
		if(isCameraAutorun)
			UpdateAutorunCamera();
		float x, y;
		CGame* game = CGame::GetInstance();
		game->GetCamPosition(x, y);
		hud->Update(x + 5, y, CGame::GetInstance()->GetPlayer()->GetHealth(), CGame::GetInstance()->GetPlayer()->GetDamage());
	}
	if(text!=NULL)
		text->Update(dt);
	if (fireworks.size() != 0) {
		for (int i = 0; i < fireworks.size(); i++) {
			fireworks.at(i)->Update(dt);
		}
	}

	if (id == 10) {
		if (this->player->y <= 224 && this->player->x > 870)
			ReadyForBossAppear();
	}
	if(EndingCount !=0)
		ReadyForEnding();

}

void CPlayScene::Render()
{
	//Render Tiles
	for (int i = 0; i < tiledMap.size(); i++)
		tiledMap[i]->Render();
	//Object Rendering
	for (int i = 0; i < updateObject.size(); i++)

	{
		if (updateObject[i]->visible == true)
		{
			updateObject[i]->Render();
			//objects[i]->RenderBoundingBox();
		}
	}
	if(CGame::GetInstance()->GetPlayer())
		hud->Render(CGame::GetInstance()->GetPlayer());
	if (text != NULL)
		text->Render();
	if (fireworks.size() != 0) {
		for (int i = 0; i < fireworks.size(); i++)
			fireworks.at(i)->Render();
	}
}

void CPlayScene::UpdateAutorunCamera()
{
	float x, y;
	CGame* game = CGame::GetInstance();
	game->GetCamPosition(x, y);
	if (x < CameraAutorunTargetX) {
		x+=5;
		if (x >= CameraAutorunTargetX)
			x = CameraAutorunTargetX;
	}
	else if (x > CameraAutorunTargetX) {
		x-=5;
		if (x <= CameraAutorunTargetX)
			x = CameraAutorunTargetX;
	}
	else {
		x = x;

	}
	if (y < CameraAutorunTargetY) {
		y+=5;
		if (y >= CameraAutorunTargetY)
			y = CameraAutorunTargetY;
	}
	else if (y > CameraAutorunTargetY) {
		y-=5;
		if (y <= CameraAutorunTargetY)
			y = CameraAutorunTargetY;
	}
	else {
		y = y;
	}
	game->SetCamPos(x, y);		
	if (x == CameraAutorunTargetX && y == CameraAutorunTargetY)
		isCameraAutorun = false;
}
void CPlayScene::CallNewText(int type, float x, float y)
{
	this->text = new Text(type);
	text->Setposition(x, y);
}
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	for (int i = 0; i < tiledMap.size(); i++)
		delete tiledMap[i];
	tiledMap.clear();
	//player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::SetCameraAutorunTarget(float movingSpaceX, float movingSpaceY)
{
	float x, y;
	CGame* game = CGame::GetInstance();
	game->GetCamPosition(x, y);
	this->CameraAutorunTargetX = x + movingSpaceX;
	this->CameraAutorunTargetY = y + movingSpaceY;
	if(movingSpaceX == 0)
		player->SetPosition(player->x, player->y + (movingSpaceY/abs(movingSpaceY)) * 114 );
	if (movingSpaceY == 0)
		player->SetPosition(player->x + (movingSpaceX / abs(movingSpaceX)) * 90, player->y );
	isCameraAutorun = true;
}

void CPlayScene::SetPlayer(CPlayer* player)
{
	this->player = player;
}
void CPlayScene::ReadyForBossAppear()
{
	
	BossAppearCount++;
	if (BossAppearCount == 1) {
		Sound::GetInstance()->Stop("Area2");
		Sound::GetInstance()->Play("BossIntro", 1, 100000);
	}
	if (BossAppearCount <= BOSS_APPEAR_TIME) {
		if (BossAppearCount % 10 < 5)
			CGame::GetInstance()->SetTileMapAlpha(100);
		else
			CGame::GetInstance()->SetTileMapAlpha(255);
	}
	else {
		Sound::GetInstance()->Stop("BossIntro");
		Sound::GetInstance()->Play("Boss", 1, 100000);
		CGame::GetInstance()->SetTileMapAlpha(0);
		BossAppearCount = 0;
		CGame::GetInstance()->SetCamPos(32, 0);
		player->SetPosition(148,192);
	}
}

void CPlayScene::ReadyForEnding()
{
	EndingCount++;
	if (EndingCount == ENDING_COUNT_TIME) {
		CGame::GetInstance()->SwitchScene(13);
		Sound::GetInstance()->Stop("Area2");
	}
}


