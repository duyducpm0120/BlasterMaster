#pragma once
#include "GameObject.h"

#define BLINK_WALKING_SPEED 0.05f;

#define BLINK_BBOX_WIDTH 26
#define BLINK_BBOX_HEIGHT 26
#define BLINK_STATE_CLOAKED 100
#define BLINK_STATE_UNCLOAKED 200
#define BLINK_DISTANCE 30
#define BLINK_ANI_CLOAKED 0
#define BLINK_ANI_UNCLOAKED 1
	

class CBlink : public CGameObject
{
	int nx;
	float startX, startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void BlinkFoward();
	void BlinkBack();
	void BlinkLeft();
	void BlinkRight();
	int BlinkCounter = 0;
	void ToggleState();
	int BlinkIncsCounter = 0;

public:
	CBlink();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
};