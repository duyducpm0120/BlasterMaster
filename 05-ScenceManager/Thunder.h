#pragma once
#include "GameObject.h"

#define THUNDER_HORIZONTAL_BBOX_WIDTH 12
#define THUNDER_HORIZONTAL_BBOX_HEIGHT 53

#define THUNDER_TIME_EXIST	3000
class CThunder
{
public:
	CThunder();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

