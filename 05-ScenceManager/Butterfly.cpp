#include "Butterfly.h"
CButterfly::CButterfly()
{
	damage = 1;
	health = 3;
	isEnemy = true;
	nx = -1;
	SetState(BUTTERFLY_STATE_WALKING_LEFT);
}

void CButterfly::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BUTTERFLY_BBOX_WIDTH;
	bottom = y + BUTTERFLY_BBOX_HEIGHT;
}

void CButterfly::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Golem can interact with the world and to each of them too!
	// 
	CatchPlayer();
	if (GetDistance(tank) == 0) {
		this->visible = false;
		return;
	};

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	float min_tx, min_ty, nx = 0, ny;

	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	// block 
	x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	y += min_ty * dy + ny * 0.4f;

	if (nx != 0) vx = 0;
	if (ny != 0) vy = 0.00f;
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CButterfly ::Render()
{
	int ani;
	if (state == BUTTERFLY_STATE_WALKING_LEFT)
		ani = BUTTERFLY_ANI_WALKING_LEFT;
	else
		ani = BUTTERFLY_ANI_WALKING_RIGHT;


	/*if (state == BUTTERFLY_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CButterfly::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BUTTERFLY_STATE_DIE:
		//y += BUTTERFLY_BBOX_HEIGHT - BUTTERFLY_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case BUTTERFLY_STATE_WALKING_LEFT:
		break;
	case BUTTERFLY_STATE_WALKING_RIGHT:
		break;
	}
}

void CButterfly::CatchPlayer()
{
	tank->GetPosition(this->target_x, this->target_y);
	if(target_x > x) {
		if (target_y > y) {
			vx = BUTTERFLY_WALKING_SPEED;
			vy = BUTTERFLY_WALKING_SPEED;
			nx = 1;
			SetState(BUTTERFLY_STATE_WALKING_RIGHT);
		}
		else
		{
			vx = BUTTERFLY_WALKING_SPEED;
			vy = -BUTTERFLY_WALKING_SPEED;
			nx = 1;
			SetState(BUTTERFLY_STATE_WALKING_RIGHT);
		}
	}
	else
	{
	if (target_y > y) {
		vx = -BUTTERFLY_WALKING_SPEED;
		vy = BUTTERFLY_WALKING_SPEED;
		nx = -1;
		SetState(BUTTERFLY_STATE_WALKING_LEFT);
	}
	else
	{
		vx = -BUTTERFLY_WALKING_SPEED;
		vy = -BUTTERFLY_WALKING_SPEED;
		nx = -1;
		SetState(BUTTERFLY_STATE_WALKING_LEFT);
	}
	}
}

void CButterfly::SetPlayer(CTank *tank)
{
	this->tank = &(*tank);
}

