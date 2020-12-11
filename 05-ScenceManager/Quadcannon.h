#pragma once
#include "GameObject.h"

#define QUADCANNON_WALKING_SPEED 0.05f;

#define QUADCANNON_BBOX_WIDTH 26
#define QUADCANNON_BBOX_HEIGHT 22
#define QUADCANNON_BBOX_HEIGHT_DIE 9

#define QUADCANNON_STATE_SHOOTING 100
#define QUADCANNON_STATE_RECHARGING 200
#define QUADCANNON_STATE_DIE 300

#define QUADCANNON_ANI_SHOOTING 0
#define QUADCANNON_ANI_RECHARGING 1
#define QUADCANNON_ANI_DIE	2

class CQuadcannon : public CGameObject
{
	int nx;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CQuadcannon();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};