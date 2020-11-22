#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Koopas.h"
#include "Tile.h"
#include "Tank.h"
#include "Golem.h"
#include "HealthBar.h"
#include "Grid.h"
#include "HUD.h"


class CPlayScene: public CScene
{
protected: 
	CTank *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	HUD* hud;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILE_MAP(string line);
	
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CTank * GetPlayer() { return player; } 
	vector<LPGAMEOBJECT>* GetObjects(){ return &objects; }
	//friend class CPlayScenceKeyHandler;

	//New stuff:
protected:
	vector<LPTILE> tiledMap;
	int offset_y = 0; //To render the tiles rows
	int testx = 0;
	CGrid* grid = CGrid::GetInstance();
	vector<LPGAMEOBJECT> updateObjects;



};



