#pragma once
#include "GameObject.h"

#define FLYINGCLAW_WALKING_SPEED 0.05f;

#define FLYINGCLAW_BBOX_WIDTH 18
#define FLYINGCLAW_BBOX_HEIGHT 16
#define FLYINGCLAW_BBOX_HEIGHT_DIE 9

#define FLYINGCLAW_STATE_WALKING_LEFT 100
#define FLYINGCLAW_STATE_WALKING_RIGHT 200
#define FLYINGCLAW_STATE_SHOOTING 300

#define FLYINGCLAW_ANI_WALKING 0
#define FLYINGCLAW_ANI_SHOOTING 1
#define FLYINGCLAW_ANI_DIE	2

class CFlyingclaw : public CGameObject
{
	int nx;
	float startX;
	float startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Shoot();
	int counter = 0;

public:
	CFlyingclaw();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};