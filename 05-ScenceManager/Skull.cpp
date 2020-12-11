#include "Skull.h"
CSkull::CSkull()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(SKULL_STATE_WALKING_LEFT);
}

void CSkull::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + SKULL_BBOX_WIDTH;

	if (state == SKULL_STATE_DIE)
		bottom = y + SKULL_BBOX_HEIGHT_DIE;
	else
		bottom = y + SKULL_BBOX_HEIGHT;
}

void CSkull::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure SKULL can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < (startX - 80)) {
		x = startX - 80; vx = -vx;
		SetState(SKULL_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		SetState(SKULL_STATE_WALKING_LEFT);
	}
}

void CSkull::Render()
{
	int ani;
	if (state == SKULL_STATE_WALKING_LEFT)
		ani = SKULL_ANI_WALKING_LEFT;
	else
		ani = SKULL_ANI_WALKING_RIGHT;


	/*if (state == SKULL_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CSkull::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKULL_STATE_DIE:
		//y += SKULL_BBOX_HEIGHT - SKULL_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case SKULL_STATE_WALKING_LEFT:
		nx = -1;
		vx = -SKULL_WALKING_SPEED;
		break;
	case SKULL_STATE_WALKING_RIGHT:
		nx = 1;
		vx = SKULL_WALKING_SPEED;
	}
}

void CSkull::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
