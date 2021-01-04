#include "JUMPER.h"
CJumper::CJumper()
{
	isEnemy = true;
	damage = 1;
	health = 3;
	nx = -1;
	SetState(JUMPER_STATE_JUMPING_LEFT);
	vx = -JUMPER_JUMPING_SPEED;
	vy = JUMPER_JUMPING_SPEED;
}

void CJumper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + JUMPER_BBOX_WIDTH;

	if (state == JUMPER_STATE_DIE)
		bottom = y + JUMPER_BBOX_HEIGHT_DIE;
	else
		bottom = y + JUMPER_BBOX_HEIGHT;
}

void CJumper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure JUMPER can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;


	if (vx < 0 && x < (startX - 100)) {
		x = startX - 100;
		SetState(JUMPER_STATE_JUMPING_RIGHT);
		nx = 1;
		vx = JUMPER_JUMPING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -JUMPER_JUMPING_SPEED;
		SetState(JUMPER_STATE_JUMPING_LEFT);
	}
	if (vy < 0 && y < (startY - 20)) {
		y = startY - 20;
		vy = JUMPER_JUMPING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -JUMPER_JUMPING_SPEED;
	}
}


void CJumper::Render()
{
	int ani;
	if (state == JUMPER_STATE_JUMPING_LEFT)
		ani = JUMPER_ANI_JUMPING_LEFT;
	else
		ani = JUMPER_ANI_JUMPING_RIGHT;


	/*if (state == JUMPER_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CJumper::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case JUMPER_STATE_DIE:
		//y += JUMPER_BBOX_HEIGHT - JUMPER_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case JUMPER_STATE_JUMPING_LEFT:

		break;
	case JUMPER_STATE_JUMPING_RIGHT:
		nx = 1;
		vx = JUMPER_JUMPING_SPEED;
	}
}

void CJumper::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
