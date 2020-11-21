#include "Grid.h"

void CGrid::Clear()
{
	for (int i = 0; i < ROW_MAX; i++)
	{
		for (int j = 0; j < COLUMN_MAX; j++)
		{
			cells[i][j].clear();
			for (UINT k = 0; k < cells[row][column].size(); k++)
			{
				delete (cells[row][column].at(k));
			}
		}
	}
}

void CGrid::Add(LPGAMEOBJECT Obj)
{
	int beginCellColumn, beginCellRow, endCellColumn, endCellRow;
	float l, t, r, b;
	Obj->GetBoundingBox(l, t, r, b);

	int CellX = (int)(l / CELL_WIDTH);
	int CellY = (int)(t / CELL_HEIGHT);

	//endCellColumn = (int)(r / CELL_WIDTH);
	//endCellRow = (int)(b / CELL_HEIGHT);

	cells[CellX][CellY].push_back(Obj);
	
}

CGrid* CGrid::__instance = NULL;


CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}
