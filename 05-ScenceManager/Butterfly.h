#pragma once
#include "GameObject.h"
#include "Tank.h"

#define BUTTERFLY_WALKING_SPEED 0.05f;

#define BUTTERFLY_BBOX_WIDTH 18
#define BUTTERFLY_BBOX_HEIGHT 18
#define BUTTERFLY_BBOX_HEIGHT_DIE 9

#define BUTTERFLY_STATE_WALKING_LEFT 100
#define BUTTERFLY_STATE_WALKING_RIGHT 200
#define BUTTERFLY_STATE_DIE 300

#define BUTTERFLY_ANI_WALKING_LEFT 0
#define BUTTERFLY_ANI_WALKING_RIGHT 1
#define BUTTERFLY_ANI_DIE	2

class CButterfly : public CGameObject
{
	int nx;
	float target_x, target_y;
	CTank* tank;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CButterfly();
	virtual void SetState(int state);
	void CatchPlayer();
	void SetPlayer(CTank *tank);
};