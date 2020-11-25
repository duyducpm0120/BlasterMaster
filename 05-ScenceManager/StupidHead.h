#pragma once
#include "GameObject.h"

#define STUPIDHEAD_WALKING_SPEED 0.05f;

#define STUPIDHEAD_BBOX_WIDTH 17
#define STUPIDHEAD_BBOX_HEIGHT 10
#define STUPIDHEAD_BBOX_HEIGHT_DIE 9

#define STUPIDHEAD_STATE_WALKING_LEFT 100
#define STUPIDHEAD_STATE_WALKING_RIGHT 200
#define STUPIDHEAD_STATE_DIE 300

#define STUPIDHEAD_ANI_WALKING_LEFT 0
#define STUPIDHEAD_ANI_WALKING_RIGHT 1
#define STUPIDHEAD_ANI_DIE	2

class CStupidHead : public CGameObject
{
	int nx;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CStupidHead();
	virtual void SetState(int state);
};