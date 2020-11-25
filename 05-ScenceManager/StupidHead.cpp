#include "StupidHead.h"
CStupidHead::CStupidHead()
{
	damage = 1;
	health = 10000;
	nx = -1;
	SetState(STUPIDHEAD_STATE_WALKING_LEFT);
}

void CStupidHead::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + STUPIDHEAD_BBOX_WIDTH;

	if (state == STUPIDHEAD_STATE_DIE)
		bottom = y + STUPIDHEAD_BBOX_HEIGHT_DIE;
	else
		bottom = y + STUPIDHEAD_BBOX_HEIGHT;
}

void CStupidHead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure STUPIDHEAD can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 50) {
		x = 50; vx = -vx;
		SetState(STUPIDHEAD_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
		SetState(STUPIDHEAD_STATE_WALKING_LEFT);
	}
}

void CStupidHead::Render()
{
	int ani;
	if (state == STUPIDHEAD_STATE_WALKING_LEFT)
		ani = STUPIDHEAD_ANI_WALKING_LEFT;
	else
		ani = STUPIDHEAD_ANI_WALKING_RIGHT;


	/*if (state == STUPIDHEAD_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CStupidHead::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STUPIDHEAD_STATE_DIE:
		//y += STUPIDHEAD_BBOX_HEIGHT - STUPIDHEAD_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case STUPIDHEAD_STATE_WALKING_LEFT:
		nx = -1;
		vx = -STUPIDHEAD_WALKING_SPEED;
		break;
	case STUPIDHEAD_STATE_WALKING_RIGHT:
		nx = 1;
		vx = STUPIDHEAD_WALKING_SPEED;
	}
}
