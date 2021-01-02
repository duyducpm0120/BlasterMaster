#include "Worm.h"
#include "Brick.h"
CWorm::CWorm()
{
	damage = 1;
	health = 2;
	nx = -1;
	SetState(WORM_STATE_WALKING_LEFT);
}

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;

	if (state == WORM_STATE_DIE)
		bottom = y + WORM_BBOX_HEIGHT_DIE;
	else
		bottom = y + WORM_BBOX_HEIGHT;
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure WORM can interact with the world and to each of them too!
	// 
	vy += 0.001 * dt;

	if (vx < 0 && x < (startX- WORM_MOVING_SPACE) ){
		x = startX - WORM_MOVING_SPACE; vx = -vx;
		SetState(WORM_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > startX ){
		x = startX; vx = -vx;
		SetState(WORM_STATE_WALKING_LEFT);
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPCOLLISIONEVENT> coCollisoningEvents;


	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	CalcColliding(coObjects, coCollisoningEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
}

void CWorm::Render()
{
	int ani;
	if (vx < 0)
		ani = WORM_ANI_WALKING_LEFT;
	else
		ani = WORM_ANI_WALKING_RIGHT;


	/*if (state == WORM_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CWorm::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
		//y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case WORM_STATE_WALKING_LEFT:
		nx = -1;
		vx = -WORM_WALKING_SPEED;
		break;
	case WORM_STATE_WALKING_RIGHT:
		nx = 1;
		vx = WORM_WALKING_SPEED;
	}
	
}

void CWorm::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
