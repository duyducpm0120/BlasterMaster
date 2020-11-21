#include "Grid.h"
#include "Utils.h"
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
	DebugOut(L"[INFO] %d\n", CellY);

	
}

CGrid* CGrid::__instance = NULL;


CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}

void CGrid::GetUpdateObjects(vector<LPGAMEOBJECT>& updateobjects, float left, float top, float right, float bottom)
{
	int firstCellColumn, firstCellRow;		// the left-top cell that containing the rectangle's area
	int lastCellColumn, lastCellRow;		// the right-bottom cell that containing the rectangle's area

	firstCellColumn = (int)left / CELL_WIDTH;
	firstCellRow = (int)top / CELL_HEIGHT;

	lastCellColumn = (int)(right) / CELL_WIDTH;
	lastCellRow = (int)(bottom) / CELL_HEIGHT;

	for (int row = firstCellRow; row <= lastCellRow; row++)
	{
		for (int column = firstCellColumn; column <= lastCellColumn; column++)
		{
			for (UINT k = 0; k < cells[row][column].size(); k++)
			{
				// Check the object if is in the vector or not ! Ref: https://en.cppreference.com/w/cpp/algorithm/find
				//if ((find(updateobjects.begin(), updateobjects.end(), cells[row][column].at(k)) != updateobjects.end()== false))
				//{
				
				if (true)
				{
					updateobjects.push_back(cells[row][column].at(k));
					DebugOut(L"Row index: %d, Col index: %d\n", row, column);
				}

				//}
			}
		}
	}
}
