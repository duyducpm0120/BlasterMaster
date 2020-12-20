#pragma once
#include "GameObject.h"

#define JUMPER_JUMPING_SPEED 0.05f;
#define JUMPER_GRAVITY 0.001f;

#define JUMPER_BBOX_WIDTH 17
#define JUMPER_BBOX_HEIGHT 25
#define JUMPER_BBOX_HEIGHT_DIE 9

#define JUMPER_STATE_JUMPING_LEFT 100
#define JUMPER_STATE_JUMPING_RIGHT 200
#define JUMPER_STATE_DIE 300

#define JUMPER_ANI_JUMPING_LEFT 0
#define JUMPER_ANI_JUMPING_RIGHT 1
#define JUMPER_ANI_DIE	2

class CJumper : public CGameObject
{
	int nx;
	bool isJumping;
	float startX;
	float startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CJumper();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};