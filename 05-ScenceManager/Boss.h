#pragma once
#include "GameObject.h"

#define Boss_WALKING_SPEED 0.03f;

#define Boss_BBOX_WIDTH 18
#define Boss_BBOX_HEIGHT 18
#define Boss_BBOX_HEIGHT_DIE 9

#define Boss_STATE_WALKING_LEFT 100
#define Boss_STATE_WALKING_RIGHT 200
#define Boss_STATE_DIE 300

#define Boss_ANI_WALKING_LEFT 0
#define Boss_ANI_WALKING_RIGHT 1
#define Boss_ANI_DIE	2

class CBoss : public CGameObject
{
	int nx;
	float startX;
	float startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CBoss();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};