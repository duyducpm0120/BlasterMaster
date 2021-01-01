#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scence.h"
#include "Player.h"
#include "Tank.h"
#include "BlankScene.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

#define RESOURCE_FILE_PATH L"Resources.txt"

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height; 

	unordered_map<int, LPSCENE> scenes;
	int current_scene; 
	int HolderSceneId;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	int playerHealth = 7;
	int playerPower = 2;

	int TileMapAlpha = 255;

	CPlayer* player;

	//New Stuff
public:
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	int* GetHealth() { return &playerHealth; };
	int* GetPower() { return &playerPower; };

	void LoadResources();
	void GetCamPosition(float& x, float& y) { x = cam_x; y = cam_y; };
	void SetPlayer(CPlayer* player) { this->player = player; };
	CPlayer* GetPlayer() { return player; };
	void IntroDraw(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);
	void GetPlayerProperties(int& health, int& power, bool& enableRocket, bool& enableThunder) {
		health = player->health;
		power = player->damage;
		enableRocket = dynamic_cast<CTank*>(player)->GetEnableRocket();
		enableThunder = dynamic_cast<CTank*>(player)->GetEnableThunder();
	};
	int GetTileMapAlpha() { return TileMapAlpha; };
	void SetTileMapAlpha(int alpha) { this->TileMapAlpha = alpha; };
	void SetCurrentSceneId(int id) { current_scene = id; }

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	HWND GetHWND() { return hWnd; };
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	void LoadSound();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);
	void SwitchToSelectWeaponScene();
	void SwitchToBlankScene(int nextSceneId);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	static CGame * GetInstance();

	~CGame();

	//New Stuff
public:
int GetCurrentSceneId() { return current_scene; }
void GetCameraBoundingBox(float& left, float& top, float& right, float& bottom);
	
	
};


