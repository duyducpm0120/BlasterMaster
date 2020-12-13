#include "Eye.h"

void CEye::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYE_BBOX_WIDTH;
	bottom = y + EYE_BBOX_HEIGHT;
}

void CEye::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure EYE can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;


	if (vx < 0 && x < (startX - 200)) {
		x = startX - 200;
		SetState(EYE_STATE_WALKING_RIGHT);
		nx = 1;
		vx = EYE_WALKING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -EYE_WALKING_SPEED;
		SetState(EYE_STATE_WALKING_LEFT);
	}
	if (vy < 0 && y < (startY - 50)) {
		y = startY - 50;
		vy = EYE_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -EYE_WALKING_SPEED;
	}

	
}

void CEye::Render()
{

	int ani;
	if (state == EYE_STATE_SHOOTING)	ani = EYE_ANI_SHOOTING;
	else ani = EYE_ANI_WALKING;
	animation_set->at(ani)->Render(x, y);
}

CEye::CEye()
{
	damage = 1;
	health = 3;
	nx = -1;
	SetState(EYE_STATE_WALKING_LEFT);
	vx = -EYE_WALKING_SPEED;
	vy = EYE_WALKING_SPEED;
}

void CEye::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	
	case EYE_STATE_WALKING_LEFT:

		break;
	case EYE_STATE_WALKING_RIGHT:
		nx = 1;
		vx = EYE_WALKING_SPEED;
	}
}

void CEye::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
