#pragma once
#include "GameObject.h"

#define FACE_WALKING_SPEED 0.05f;

#define FACE_BBOX_WIDTH 26
#define FACE_BBOX_HEIGHT 34
#define FACE_BBOX_HEIGHT_DIE 9

#define FACE_STATE_WALKING_UP 100
#define FACE_STATE_WALKING_DOWN 200
#define FACE_STATE_DIE 300

#define FACE_ANI_WALKING_UP 0
#define FACE_ANI_WALKING_DOWN 1
#define FACE_ANI_DIE	2

class CFace : public CGameObject
{
	int ny;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CFace();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};