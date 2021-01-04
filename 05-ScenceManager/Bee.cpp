#include "BEE.h"
CBee::CBee()
{
	isEnemy = true;
	damage = 1;
	health = 3;
	nx = -1;
	SetState(BEE_STATE_WALKING_LEFT);
	vx = -BEE_WALKING_SPEED;
	vy = BEE_WALKING_SPEED;
}

void CBee::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BEE_BBOX_WIDTH;

	if (state == BEE_STATE_DIE)
		bottom = y + BEE_BBOX_HEIGHT_DIE;
	else
		bottom = y + BEE_BBOX_HEIGHT;
}

void CBee::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure BEE can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;


	if (vx < 0 && x < (startX - 100)) {
		x= startX - 100;
		SetState(BEE_STATE_WALKING_RIGHT);
		nx = 1;
		vx = BEE_WALKING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -BEE_WALKING_SPEED;
		SetState(BEE_STATE_WALKING_LEFT);
	}
	if (vy < 0 && y < (startY - 40)) {
		y = startY - 40;
		vy = BEE_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -BEE_WALKING_SPEED;
	}
}


void CBee::Render()
{
	int ani;
	if (state == BEE_STATE_WALKING_LEFT)
		ani = BEE_ANI_WALKING_LEFT;
	else
		ani = BEE_ANI_WALKING_RIGHT;


	/*if (state == BEE_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBee::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BEE_STATE_DIE:
		//y += BEE_BBOX_HEIGHT - BEE_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case BEE_STATE_WALKING_LEFT:
		
		break;
	case BEE_STATE_WALKING_RIGHT:
		nx = 1;
		vx = BEE_WALKING_SPEED;
	}
}

void CBee::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
