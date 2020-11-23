#include "Golem.h"
CGolem::CGolem()
{
	damage = 1;
	health = 2;
	nx = -1;
	SetState(GOLEM_STATE_WALKING_LEFT);
}

void CGolem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + GOLEM_BBOX_WIDTH;

	if (state == GOLEM_STATE_DIE)
		bottom = y + GOLEM_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOLEM_BBOX_HEIGHT;
}

void CGolem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure Golem can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 50) {
		x = 50; vx = -vx;
		SetState(GOLEM_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
		SetState(GOLEM_STATE_WALKING_LEFT);
	}
}

void CGolem::Render()
{
	int ani;
	if (state == GOLEM_STATE_WALKING_LEFT)
		ani = GOLEM_ANI_WALKING_LEFT;
	else
		ani = GOLEM_ANI_WALKING_RIGHT;


	/*if (state == GOLEM_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGolem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOLEM_STATE_DIE:
		//y += GOLEM_BBOX_HEIGHT - GOLEM_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case GOLEM_STATE_WALKING_LEFT:
		nx = -1;
		vx = -GOLEM_WALKING_SPEED;
		break;
	case GOLEM_STATE_WALKING_RIGHT:
		nx = 1;
		vx = GOLEM_WALKING_SPEED;
	}
}
