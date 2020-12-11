#pragma once
#include "GameObject.h"
#include "Tank.h"

#define EYE_WALKING_SPEED 0.05f;

#define EYE_BBOX_WIDTH 18
#define EYE_BBOX_HEIGHT 18
#define EYE_BBOX_HEIGHT_DIE 9

#define EYE_STATE_WALKING_LEFT 100
#define EYE_STATE_WALKING_RIGHT 200
#define EYE_STATE_WALKING_UP 400
#define EYE_STATE_WALKING_DOWN 500
#define EYE_STATE_DIE 300

#define EYE_ANI_WALKING_LEFT 0
#define EYE_ANI_WALKING_RIGHT 1
#define EYE_ANI_WALKING_UP 3
#define EYE_ANI_WALKING_DOWN 4
#define EYE_ANI_DIE	2

class CEye : public CGameObject
{
	int nx, ny;
	float startX, startY, target_x, target_y;
	CTank* tank;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CEye();
	virtual void SetState(int state);
	void CatchPlayer();
	void SetPlayer(CTank* tank);
	void SetStartPosition(float x, float y);
};