#pragma once
#include "GameObject.h"

#define WORM_WALKING_SPEED 0.05f;

#define WORM_BBOX_WIDTH 17
#define WORM_BBOX_HEIGHT 10
#define WORM_BBOX_HEIGHT_DIE 9

#define WORM_STATE_WALKING_LEFT 100
#define WORM_STATE_WALKING_RIGHT 200
#define WORM_STATE_DIE 300

#define WORM_ANI_WALKING_LEFT 0
#define WORM_ANI_WALKING_RIGHT 1
#define WORM_ANI_DIE	2
#define WORM_MOVING_SPACE	200

class CWorm : public CGameObject
{
	int nx;
	float startX;
	float startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CWorm();
	virtual void SetState(int state);
	void SetStartPosition(float x,float y);
};