#pragma once
#include "GameObject.h"

#define BALL_WALKING_SPEED 0.05f;

#define BALL_BBOX_WIDTH 18
#define BALL_BBOX_HEIGHT 28
#define BALL_BBOX_HEIGHT_DIE 9

#define BALL_STATE_WALKING_LEFT 100
#define BALL_STATE_WALKING_RIGHT 200
#define BALL_STATE_DIE 300

#define BALL_ANI_WALKING_LEFT 0
#define BALL_ANI_WALKING_RIGHT 1
#define BALL_ANI_DIE	2

class CBall : public CGameObject
{
	int nx;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CBall();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};