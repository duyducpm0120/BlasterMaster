#pragma once
#include "GameObject.h"
#define BULLET_SPEED 0.1f

#define BULLET_HORIZONTAL_BBOX_WIDTH 24
#define BULLET_HORIZONTAL_BBOX_HEIGHT 8

#define BULLET_VERTICAL_BBOX_WIDTH 8
#define BULLET_VERTICAL_BBOX_HEIGHT 24

#define BULLET_FLYING_SPACE 50

#define BULLET_STATE_FLYING_LEFT 100
#define BULLET_STATE_FLYING_RIGHT 200
#define BULLET_STATE_DIE 300

#define BULLET_ANI_FLYING_LV1	0
#define BULLET_ANI_FLYING_LV2	1


class CBullet : public CGameObject
{
	float startPositionX;
	float startPositionY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int direct;		// -1: left	1:right 
	int level;


public:
	CBullet(int direct, int level);
	virtual void SetState(int state);
	virtual void SetStartPositon(float x, float y, int nX);
};

