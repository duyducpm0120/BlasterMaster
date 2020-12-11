#pragma once
#include "GameObject.h"

#define ROBOT_WALKING_SPEED 0.05f;

#define ROBOT_BBOX_WIDTH 26
#define ROBOT_BBOX_HEIGHT 26
#define ROBOT_BBOX_HEIGHT_DIE 9

#define ROBOT_STATE_WALKING_LEFT 100
#define ROBOT_STATE_WALKING_RIGHT 200
#define ROBOT_STATE_DIE 300

#define ROBOT_ANI_WALKING_LEFT 0
#define ROBOT_ANI_WALKING_RIGHT 1
#define ROBOT_ANI_DIE	2

class CRobot : public CGameObject
{
	int nx;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CRobot();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};