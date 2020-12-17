#pragma once
#include "GameObject.h"
#include "Player.h"

#define STUPIDHEAD_WALKING_SPEED 0.05f;
#define STUPIDHEAD_CLIMBING_SPEED 0.05f;

#define STUPIDHEAD_BBOX_WIDTH 18
#define STUPIDHEAD_BBOX_HEIGHT 18
#define STUPIDHEAD_BBOX_HEIGHT_DIE 9

#define STUPIDHEAD_STATE_CLIMBING_TOP 100
#define STUPIDHEAD_STATE_CLIMBING_BOT 200
#define STUPIDHEAD_STATE_CLIMBING_LEFT 300
#define STUPIDHEAD_STATE_CLIMBING_RIGHT 400
#define STUPIDHEAD_STATE_FALL	500

#define STUPIDHEAD_ANI_CLIMBING_TOP_LEFT 0
#define STUPIDHEAD_ANI_CLIMBING_TOP_RIGHT 1
#define STUPIDHEAD_ANI_CLIMBING_BOT_LEFT 2
#define STUPIDHEAD_ANI_CLIMBING_BOT_RIGHT 3
#define STUPIDHEAD_ANI_CLIMBING_LEFT_LEFT 4
#define STUPIDHEAD_ANI_CLIMBING_LEFT_RIGHT 5
#define STUPIDHEAD_ANI_CLIMBING_RIGHT_LEFT 6
#define STUPIDHEAD_ANI_CLIMBING_RIGHT_RIGHT 7
#define DEVIATIONS	2.5

class CStupidHead : public CGameObject
{
	int nx;
	int state;
	int ani;
	bool isTouchBrick;
	CPlayer* target;
public:
	void Fall();
	void ChangeDirection();
	void ChangeDirectionInverse();
	void HandleIsTouchBrick();
	CStupidHead();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetTarget(CPlayer* target) { this->target = target; }
};