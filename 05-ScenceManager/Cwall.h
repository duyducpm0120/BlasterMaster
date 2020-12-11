#pragma once
#include "GameObject.h"

#define CWALL_WALKING_SPEED 0.05f;

#define CWALL_BBOX_WIDTH 17
#define CWALL_BBOX_HEIGHT 17
#define CWALL_BBOX_HEIGHT_DIE 9

#define CWALL_STATE_WALKING_LEFT 100
#define CWALL_STATE_WALKING_RIGHT 200
#define CWALL_STATE_WALKING_LEFT_ON_WALL 400
#define CWALL_STATE_WALKING_RIGHT_ON_WALL 500
#define CWALL_STATE_WALKING_UP_LEFT 600
#define CWALL_STATE_WALKING_UP_RIGHT 700
#define CWALL_STATE_WALKING_DOWN_LEFT 800
#define CWALL_STATE_WALKING_DOWN_RIGHT 1000
#define CWALL_STATE_DIE 300

#define CWALL_ANI_WALKING_LEFT 0
#define CWALL_ANI_WALKING_RIGHT 1
#define CWALL_ANI_WALKING_LEFT_ON_WALL 3
#define CWALL_ANI_WALKING_RIGHT_ON_WALL 4
#define CWALL_ANI_WALKING_UP_LEFT 5
#define CWALL_ANI_WALKING_UP_RIGHT 6
#define CWALL_ANI_WALKING_DOWN_LEFT 7
#define CWALL_ANI_WALKING_DOWN_RIGHT 8
#define CWALL_ANI_DIE	2

class CCwall : public CGameObject
{
	int nx, ny;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CCwall();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};