#pragma once
#include "GameObject.h"

#define BUTERFLY_WALKING_SPEED 0.05f;

#define BUTERFLY_BBOX_WIDTH 18
#define BUTERFLY_BBOX_HEIGHT 18
#define BUTERFLY_BBOX_HEIGHT_DIE 9

#define BUTERFLY_STATE_WALKING_LEFT 100
#define BUTERFLY_STATE_WALKING_RIGHT 200
#define BUTERFLY_STATE_DIE 300

#define BUTERFLY_ANI_WALKING_LEFT 0
#define BUTERFLY_ANI_WALKING_RIGHT 1
#define BUTERFLY_ANI_DIE	2

class CButterfly : public CGameObject
{
	int nx;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CButterfly();
	virtual void SetState(int state);
};