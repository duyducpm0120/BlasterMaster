﻿#include <iostream>
#include <fstream>

#include "Game.h"
#include "Utils.h"
#include "IntroScene.h"
#include "PlayScence.h"
#include "Sound.h"
#include "Boss.h"
#include "BlankScene.h"
#define MAX_RESOURCES_LINE	1024

#define RESOURCES_SECTION_UNKNOWN			-1
#define RESOURCE_SECTION_TEXTURES				2
#define RESOURCE_SECTION_SPRITES					3
#define RESOURCE_SECTION_ANIMATIONS			4
#define RESOURCE_SECTION_ANIMATION_SETS	5

CGame * CGame::__instance = NULL;
int CBoss::ani = 0;
int CBoss::alpha = 255;
/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;									

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw 
*/
void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	D3DXVECTOR3 p(x - cam_x, y - cam_y, 0);
	RECT r; 
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK) 
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CGame::ProcessKeyboard()
{
	HRESULT hr; 

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h==DI_OK)
			{ 
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE *)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}
void CGame::LoadSound()
{
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (13).wav", "PlayerBulletHitBrick");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (7).wav", "PlayerFireUnderWorld");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (8).wav", "PlayerFireOverWorld");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (9).wav", "BossFire");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (10).wav", "PlayerJump");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (16).wav", "EnemyBulletBang");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (19).wav", "PlayerInjured");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (21).wav", "PickingItems");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (22).wav", "TeleporterTransform");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (27).wav", "Enemydie");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (36).wav", "BossIntro");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/GameOver.wav", "GameOver");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/LifeLost.wav", "LifeLost");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (15).wav", "MineBip");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (16).wav", "EnemyBulletBang");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (19).wav", "PlayerInjured");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (23).wav", "FireRocket");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (24).wav", "TransingWeaponScene");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (25).wav", "FireHomingMissles");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (30).wav", "SkullFire");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (35).wav", "BossDie");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (29).wav", "TankDie");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (22).wav", "Blink");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound/Blaster Master SFX (26).wav", "SwitchScene");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound//Blaster Master SFX (17).wav", "Thunder");
	Sound::GetInstance()->LoadSound("Sources/Sound/rawSound//Blaster Master SFX (4).wav", "BulletTouchBoss");
	Sound::GetInstance()->LoadSound("Sources/Sound/Intro/Opening.wav", "Opening");
	Sound::GetInstance()->LoadSound("Sources/Sound/Intro/CarSplash.wav", "CarSplash");
	Sound::GetInstance()->LoadSound("Sources/Sound/Intro/CarBackground.wav", "CarBackground");
	Sound::GetInstance()->LoadSound("Sources/Sound/Ending.wav","Ending");
	Sound::GetInstance()->LoadSound("Sources/Sound/Boss.wav", "Boss");
	Sound::GetInstance()->LoadSound("Sources/Sound/Area2.wav", "Area2");
	Sound::GetInstance()->LoadSound("Sources/Sound/Ending/Mountain.wav", "Mountain");
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

void CGame::GetCameraBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = this->cam_x;
	top = this->cam_y;
	right = this->cam_x + GetScreenWidth();
	bottom = this->cam_y + GetScreenHeight();
}

/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void CGame::SweptAABB(
	float ml, float mt,	float mr, float mb,			
	float dx, float dy,			
	float sl, float st, float sr, float sb,
	float &t, float &nx, float &ny)
{

	if (sl <= mr && sr >= ml && st <= mb && sb >= mt) {
		if (st >= mt && sl >= ml) {
			t = 0;
			nx = 1.0f;
			ny = 1.0f;
			return;
		}
		else if (st <= mt && sl <= ml) {
			t = 0;
			nx = -1.0f;
			ny = -1.0f;
			return;
		}
		else if (st <= mt && sl >= ml) {
			t = 0;
			nx = 1.0f;
			ny = -1.0f;
			return;
		}
		else {
			t = 0;
			nx = -1.0f;
			ny = 1.0f;
			return;
		}
	}
	else {

		float dx_entry, dx_exit, tx_entry, tx_exit;
		float dy_entry, dy_exit, ty_entry, ty_exit;

		float t_entry;
		float t_exit;

		t = -1.0f;			// no collision
		nx = ny = 0;

		//
		// Broad-phase test 
		//

		float bl = dx > 0 ? ml : ml + dx;
		float bt = dy > 0 ? mt : mt + dy;
		float br = dx > 0 ? mr + dx : mr;
		float bb = dy > 0 ? mb + dy : mb;

		if (br < sl || bl > sr || bb < st || bt > sb) return;


		if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

		if (dx > 0)
		{
			dx_entry = sl - mr;
			dx_exit = sr - ml;
		}
		else if (dx < 0)
		{
			dx_entry = sr - ml;
			dx_exit = sl - mr;
		}


		if (dy > 0)
		{
			dy_entry = st - mb;
			dy_exit = sb - mt;
		}
		else if (dy < 0)
		{
			dy_entry = sb - mt;
			dy_exit = st - mb;
		}

		if (dx == 0)
		{
			tx_entry = -999999.0f;
			tx_exit = 999999.0f;
		}
		else
		{
			tx_entry = dx_entry / dx;
			tx_exit = dx_exit / dx;
		}

		if (dy == 0)
		{
			ty_entry = -99999.0f;
			ty_exit = 99999.0f;
		}
		else
		{
			ty_entry = dy_entry / dy;
			ty_exit = dy_exit / dy;
		}


		if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

		t_entry = max(tx_entry, ty_entry);
		t_exit = min(tx_exit, ty_exit);

		if (t_entry > t_exit) return;

		t = t_entry;

		if (tx_entry > ty_entry)
		{
			ny = 0.0f;
			dx > 0 ? nx = -1.0f : nx = 1.0f;
		}
		else
		{
			nx = 0.0f;
			dy > 0 ? ny = -1.0f : ny = 1.0f;
		}
	}
}

CGame *CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	if (id != 12 && id != 13 && id!=14) {
		LPSCENE scene = new CPlayScene(id, path);
		scenes[id] = scene;
	}
	else if(id == 12)
	{
		LPSCENE scene = new IntroScene(id, path);
		scenes[id] = scene;
	}
	else if (id == 13)
	{
		LPSCENE scene = new IntroScene(id, path);
		scenes[id] = scene;
	}
	else if (id == 14)
	{
		LPSCENE scene = new BlankScene(id, path);
		scenes[id] = scene;
	}
}

void CGame::_ParseSection_TEXTURES(string line)
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

void CGame::_ParseSection_SPRITES(string line)

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

void CGame::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CGame::_ParseSection_ANIMATION_SETS(string line)
{

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CGame::LoadResources()
{
	DebugOut(L"[INFO] Start loading game resources from : %s \n", RESOURCE_FILE_PATH);

	ifstream f;
	f.open(RESOURCE_FILE_PATH);

	int section = RESOURCES_SECTION_UNKNOWN;

	char str[MAX_RESOURCES_LINE];

	while (f.getline(str, MAX_RESOURCES_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]")
		{
			section = RESOURCE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]") {
			section = RESOURCE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = RESOURCE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = RESOURCE_SECTION_ANIMATION_SETS; continue;
		}
		if (line[0] == '[') { section = RESOURCES_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case RESOURCE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case RESOURCE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case RESOURCE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case RESOURCE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"resources\\bbox.png", D3DCOLOR_XRGB(0, 0, 0));
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_UNDERWORLD);
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_OVERWORLD);
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_ENDING);
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_INTRO);
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_BOSS);
	DebugOut(L"[INFO] Done loading game resources %s\n", RESOURCE_FILE_PATH);
	
}



/*
	Load game campaign file and load/initiate first scene
*/
void CGame::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
			case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n",gameFile);
	LoadSound();
	SwitchScene(current_scene);
	
}


void CGame::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);
	if (scene_id != 11 && scene_id != 12 && scene_id != 13 && scene_id != 14 && current_scene != 11 && current_scene != 12 && current_scene != 13 && !(scene_id == 5 && current_scene == 9) && !(scene_id == 8 && current_scene == 10)) {
		if (dynamic_cast<CPlayScene*>(scenes[current_scene])->GetPlayer())
		{
			player = dynamic_cast<CPlayScene*>(scenes[current_scene])->GetPlayer();
			DebugOut(L"Get player done \n");
		}

	}
	if (scene_id == 5 && current_scene == 9) {
		player = dynamic_cast<CPlayScene*>(scenes[current_scene])->GetPlayer();
		dynamic_cast<CPlayScene*>(scenes[scene_id])->GetPlayer()->health = dynamic_cast<CPlayScene*>(scenes[current_scene])->GetPlayer()->GetHealth();
		dynamic_cast<CPlayScene*>(scenes[scene_id])->GetPlayer()->damage = dynamic_cast<CPlayScene*>(scenes[current_scene])->GetPlayer()->GetDamage();
		DebugOut(L"Get player done \n");
	}
	if (current_scene == 12 && scene_id == 1) {
		Sound::GetInstance()->Stop("CarBackground");
		Sound::GetInstance()->Play("Area2", 1, 100000);
	}
	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	if (s->IsLoaded() == false)
		s->Load();
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	
	if (current_scene == 10) {
		SetCamPos(0, 1296);		
		dynamic_cast<CPlayScene*>(scenes[current_scene])->isCamSetInit = false;
	}
	if (current_scene == 11 || current_scene == 12) {
		SetCamPos(0,0);
	}
}
void CGame::SwitchToSelectWeaponScene()
{
	if (current_scene != 11) {
		HolderSceneId = current_scene;
		SwitchScene(11);
	}
	else {
		int x = dynamic_cast<CPlayScene*>(scenes[current_scene])->GetChoose()->x;
		if (x == 78)
			dynamic_cast<CPlayScene*>(scenes[HolderSceneId])->GetPlayer()->SetSecondWeapon(WEAPONS_TYPE_TRIPLEROCKET);
		else if(x==110)
			dynamic_cast<CPlayScene*>(scenes[HolderSceneId])->GetPlayer()->SetSecondWeapon(WEAPONS_TYPE_THUNDER);
		else if(x==142)
			dynamic_cast<CPlayScene*>(scenes[HolderSceneId])->GetPlayer()->SetSecondWeapon(WEAPONS_TYPE_ROCKET);
		SwitchScene(HolderSceneId);
	}
	Sound::GetInstance()->Play("TransingWeaponScene", 0, 1);
}

void CGame::SwitchToScene(int nextSceneId)
{
	Sound::GetInstance()->Play("SwitchScene", 0, 1);
	dynamic_cast<BlankScene*>(scenes[14])->SetNextSceneId(nextSceneId);
	dynamic_cast<BlankScene*>(scenes[14])->SetLastSceneId(current_scene);
	current_scene = 14;
	SetCamPos(0, 0);
}

void CGame::IntroDraw(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	float camX, camY;
	CGame::GetInstance()->GetCamPosition(camX, camY);
	D3DXVECTOR3 p(floor(x -camX), floor(y -camY), 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXMATRIX oldMatrix;
	D3DXMATRIX newMatrix;

	D3DXVECTOR2 scaling;
	//1.1852 1.0538
	if (direction > 0)
		scaling = D3DXVECTOR2(0.99032882, 1.12280702);
	else
		scaling = D3DXVECTOR2(-1, 1);

	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2((float)(right - left) / 2 - 130, p.y + (float)(bottom - top) / 2 - 150), 0, &scaling, NULL, 0, NULL);
	spriteHandler->GetTransform(&oldMatrix);
	spriteHandler->SetTransform(&newMatrix);
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	spriteHandler->SetTransform(&oldMatrix);
}

