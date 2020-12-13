#pragma once
#include "GameObject.h"

#define EYE_WALKING_SPEED 0.05f;

#define EYE_BBOX_WIDTH 16
#define EYE_BBOX_HEIGHT 16
#define EYE_BBOX_HEIGHT_DIE 9

#define EYE_STATE_WALKING_LEFT 100
#define EYE_STATE_WALKING_RIGHT 200
#define EYE_STATE_SHOOTING 300

#define EYE_ANI_WALKING 0
#define EYE_ANI_SHOOTING 1
#define EYE_ANI_DIE	2

class CEye : public CGameObject
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
	CEye();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};