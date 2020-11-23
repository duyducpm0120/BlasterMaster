#pragma once
#include "GameObject.h"

#define BEE_WALKING_SPEED 0.05f;

#define BEE_BBOX_WIDTH 16
#define BEE_BBOX_HEIGHT 16
#define BEE_BBOX_HEIGHT_DIE 9

#define BEE_STATE_WALKING_LEFT 100
#define BEE_STATE_WALKING_RIGHT 200
#define BEE_STATE_DIE 300

#define BEE_ANI_WALKING_LEFT 0
#define BEE_ANI_WALKING_RIGHT 1
#define BEE_ANI_DIE	2

class CBee : public CGameObject
{
	int nx;
	float startX;
	float startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CBee();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};