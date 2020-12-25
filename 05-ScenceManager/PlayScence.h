#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Tile.h"
#include "Tank.h"
#include "Golem.h"
#include "HealthBar.h"
#include "Grid.h"
#include "HUD.h"
#include "Player.h"
#include "Sophia.h"
#include "OHSophia.h"
#include "Choose.h"
#include "Text.h"
class CPlayScene: public CScene
{
protected: 
	CPlayer *player;			// A play scene has to have player, right? 

	CTank* tank;
	CSophia* sophia;
	COHSophia* OHSophia;
	vector<LPGAMEOBJECT> objects;
	HUD* hud;
	CChoose* choose;

	int *playerHealth;
	int *playerPower;
	bool isCameraAutorun;
	float CameraAutorunTargetX;
	float CameraAutorunTargetY;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILE_MAP(string line);

	void CallDestroyed(CGameObject* object);
	Text* text;
	int BossAppearCount = 0;
	
public: 
	CPlayScene(int id, LPCWSTR filePath);
	CTank* GetTank() { return tank; };
	virtual void Load();
	virtual void Load(vector<LPGAMEOBJECT> objects);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CPlayer * GetPlayer() { return player; } 
	int GetPlayerHealth() { return *playerHealth; };
	int GetPlayerPower() { return *playerPower; };
	bool IsCameraAutorun() { return isCameraAutorun; };
	bool isCamSetInit = false;
	void SetIsCameraAutorun(bool param) { isCameraAutorun = param; };
	void SetCameraAutorunTarget(float movingSpaceX, float movingSpaceY);
	void SetPlayer(CPlayer* player);
	HUD* GetHUD() { return hud; };
	void SetHUD(HUD* hud) { this->hud = hud; };
	vector<LPGAMEOBJECT>* GetObjects(){ return &objects; }
	vector<LPGAMEOBJECT>* GetUpdateObjects() { return &updateObject; }
	void UpdateAutorunCamera();
	CChoose* GetChoose(){ return choose; };
	void CallNewText(int type, float x, float y);
	void ReadyForBossAppear();
	//friend class CPlayScenceKeyHandler;

	//New stuff:
protected:
	vector<LPTILE> tiledMap;
	int offset_y = 0; //To render the tiles rows
	int testx = 0;
	CGrid* grid = CGrid::GetInstance();
	vector<LPGAMEOBJECT> updateObject;



};



