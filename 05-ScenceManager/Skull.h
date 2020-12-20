#pragma once
#include "GameObject.h"

#define SKULL_WALKING_SPEED 0.05f;

#define SKULL_BBOX_WIDTH 18
#define SKULL_BBOX_HEIGHT 28
#define SKULL_BBOX_HEIGHT_DIE 9

#define SKULL_STATE_WALKING_LEFT 100
#define SKULL_STATE_WALKING_RIGHT 200
#define SKULL_STATE_DIE 300

#define SKULL_ANI_WALKING_LEFT 0
#define SKULL_ANI_WALKING_RIGHT 1
#define SKULL_ANI_DIE	2

class CSkull : public CGameObject
{
	int nx;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CSkull();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};