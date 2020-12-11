#include "CWall.h"
CCwall::CCwall()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(CWALL_STATE_WALKING_LEFT_ON_WALL);
}

void CCwall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + CWALL_BBOX_WIDTH;

	if (state == CWALL_STATE_DIE)
		bottom = y + CWALL_BBOX_HEIGHT_DIE;
	else
		bottom = y + CWALL_BBOX_HEIGHT;
}

void CCwall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure CWALL can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < (startX - 80)) {
		x = startX - 80; vx = -vx;
		SetState(CWALL_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		SetState(CWALL_STATE_WALKING_LEFT);
	}
}

void CCwall::Render()
{
	int ani;
	if (state == CWALL_STATE_WALKING_LEFT)
		ani = CWALL_ANI_WALKING_LEFT;
	else if (state == CWALL_STATE_WALKING_RIGHT)
		ani = CWALL_ANI_WALKING_RIGHT;
	else if (state == CWALL_STATE_WALKING_LEFT_ON_WALL)
		ani = CWALL_ANI_WALKING_LEFT_ON_WALL;
	else if (state == CWALL_STATE_WALKING_RIGHT_ON_WALL)
		ani = CWALL_ANI_WALKING_RIGHT_ON_WALL;
	else if (state == CWALL_STATE_WALKING_UP_LEFT)
		ani = CWALL_ANI_WALKING_UP_LEFT;
	else if (state == CWALL_STATE_WALKING_UP_RIGHT)
		ani = CWALL_ANI_WALKING_UP_RIGHT;
	else if (state == CWALL_STATE_WALKING_DOWN_LEFT)
		ani = CWALL_ANI_WALKING_DOWN_LEFT;
	else if (state == CWALL_STATE_WALKING_DOWN_RIGHT)
		ani = CWALL_ANI_WALKING_DOWN_RIGHT;



	/*if (state == CWALL_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CCwall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CWALL_STATE_DIE:
		//y += CWALL_BBOX_HEIGHT - CWALL_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case CWALL_STATE_WALKING_LEFT:
		nx = -1;
		vx = -CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_RIGHT:
		nx = 1;
		vx = CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_LEFT_ON_WALL:
		nx = -1;
		vx = -CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_RIGHT_ON_WALL:
		nx = 1;
		vx = CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_UP_LEFT:
		ny = -1;
		vy = -CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_UP_RIGHT:
		ny = -1;
		vy = -CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_DOWN_LEFT:
		ny = 1;
		vy = CWALL_WALKING_SPEED;
		break;
	case CWALL_STATE_WALKING_DOWN_RIGHT:
		ny = 1;
		vy = CWALL_WALKING_SPEED;
		break;
	}
}

void CCwall::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
