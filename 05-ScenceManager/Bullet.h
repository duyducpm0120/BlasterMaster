#pragma once
#include "GameObject.h"
#define BULLET_SPEED 0.1f

#define BULLET_HORIZONTAL_BBOX_WIDTH 24
#define BULLET_HORIZONTAL_BBOX_HEIGHT 8

#define BULLET_VERTICAL_BBOX_WIDTH 8
#define BULLET_VERTICAL_BBOX_HEIGHT 24

#define BULLET_FLYING_SPACE 100

#define BULLET_STATE_FLYING_LEFT 100
#define BULLET_STATE_FLYING_RIGHT 200
#define	BULLET_STATE_FLYING_UP	300

#define BULLET_ANI_FLYING_LEFT_LV1	0
#define BULLET_ANI_FLYING_RIGHT_LV1	1
#define BULLET_ANI_FLYING_UP_LV1	2
#define BULLET_ANI_FLYING_LEFT_LV2	3
#define BULLET_ANI_FLYING_RIGHT_LV2	4
#define BULLET_ANI_FLYING_UP_LV2	5




class CBullet : public CGameObject
{
protected:
	float startPositionX;
	float startPositionY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int level;


public:
	CBullet() = default;
	CBullet(int level, int state);
	virtual void SetState(int state);
	void SetStartPositon(float x, float y);
	int GetState();
};

