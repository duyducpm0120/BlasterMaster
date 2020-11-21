#pragma once
#include "GameObject.h"


#define COLUMN_MAX	16
#define ROW_MAX		55
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
	static int constexpr CELL_HEIGHT = 32;
	void Clear();
	void Add(LPGAMEOBJECT Obj);
	static CGrid* GetInstance();
	int count = 0;
	
};

