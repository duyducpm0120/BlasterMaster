#pragma once
#include "GameObject.h"


#define COLUMN_MAX	35
#define ROW_MAX		35
class CGrid
{
	friend class GameObject;
	int column;
	int row;

	static CGrid* __instance;
	//Objects Grid 
	
	vector<LPGAMEOBJECT> cells[ROW_MAX][COLUMN_MAX];

public:
	static int constexpr  CELL_WIDTH = 64;
	static int constexpr CELL_HEIGHT = 64;
	void Clear();
	void Add(LPGAMEOBJECT Obj);
	static CGrid* GetInstance();
	void GetUpdateObjects(vector<LPGAMEOBJECT>& updateobjects, float left, float top, float right, float bottom); // This gives the objects that needs to be updated
	void Render();
};

