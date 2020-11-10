#pragma once
#include "GameObject.h"

#define GOLEM_WALKING_SPEED 0.05f;

#define GOLEM_BBOX_WIDTH 14
#define GOLEM_BBOX_HEIGHT 26
#define GOLEM_BBOX_HEIGHT_DIE 9

#define GOLEM_STATE_WALKING_LEFT 100
#define GOLEM_STATE_WALKING_RIGHT 200
#define GOLEM_STATE_DIE 300

#define GOLEM_ANI_WALKING_LEFT 0
#define GOLEM_ANI_WALKING_RIGHT 1
#define GOLEM_ANI_DIE	2

class CGolem : public CGameObject
{
	int nx;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CGolem();
	virtual void SetState(int state);
};