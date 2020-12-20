#include "BALL.h"
CBall::CBall()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(BALL_STATE_WALKING_LEFT);
}

void CBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + BALL_BBOX_WIDTH;

	if (state == BALL_STATE_DIE)
		bottom = y + BALL_BBOX_HEIGHT_DIE;
	else
		bottom = y + BALL_BBOX_HEIGHT;
}

void CBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure BALL can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < (startX - 30)) {
		x = startX - 30; vx = -vx;
		SetState(BALL_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		SetState(BALL_STATE_WALKING_LEFT);
	}
}

void CBall::Render()
{
	int ani;
	if (state == BALL_STATE_WALKING_LEFT)
		ani = BALL_ANI_WALKING_LEFT;
	else
		ani = BALL_ANI_WALKING_RIGHT;


	/*if (state == BALL_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BALL_STATE_DIE:
		//y += BALL_BBOX_HEIGHT - BALL_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case BALL_STATE_WALKING_LEFT:
		nx = -1;
		vx = -BALL_WALKING_SPEED;
		break;
	case BALL_STATE_WALKING_RIGHT:
		nx = 1;
		vx = BALL_WALKING_SPEED;
	}
}

void CBall::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
