#pragma once
#include "GameObject.h"

#define SKULL_WALKING_SPEED 0.05f;

#define SKULL_BBOX_WIDTH 18
#define SKULL_BBOX_HEIGHT 18
#define SKULL_BBOX_HEIGHT_DIE 9

#define SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED 100
#define SKULL_STATE_WALKING_RIGHT_MOUTH_CLOSED 200
#define SKULL_STATE_WALKING_LEFT_MOUTH_OPENED 300
#define SKULL_STATE_WALKING_RIGHTT_MOUTH_OPENED 400
#define SKULL_STATE_DIE 500

#define SKULL_ANI_WALKING_LEFT_CLOSED 0
#define SKULL_ANI_WALKING_LEFT_OPENED 1
#define SKULL_ANI_WALKING_RIGHT_CLOSED 2
#define SKULL_ANI_WALKING_RIGHT_OPENED 3
#define SKULL_ANI_DIE	6

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