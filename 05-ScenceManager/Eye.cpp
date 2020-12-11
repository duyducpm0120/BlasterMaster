#include "Eye.h"
CEye::CEye()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(EYE_STATE_WALKING_LEFT);
}

void CEye::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + EYE_BBOX_WIDTH;

	if (state == EYE_STATE_DIE)
		bottom = y + EYE_BBOX_HEIGHT_DIE;
	else
		bottom = y + EYE_BBOX_HEIGHT;
}

void CEye::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		SetState(EYE_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		SetState(EYE_STATE_WALKING_LEFT);
	}
}

void CEye::Render()
{
	int ani;
	if (state == EYE_STATE_WALKING_LEFT)
		ani = EYE_ANI_WALKING_LEFT;
	else if (state == EYE_STATE_WALKING_RIGHT)
		ani = EYE_ANI_WALKING_RIGHT;
	else if (state == EYE_STATE_WALKING_UP)
		ani = EYE_ANI_WALKING_UP;
	else if (state == EYE_STATE_WALKING_DOWN)
		ani = EYE_ANI_WALKING_DOWN;


	/*if (state == EYE_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CEye::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYE_STATE_DIE:
		//y += EYE_BBOX_HEIGHT - EYE_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case EYE_STATE_WALKING_LEFT:
		nx = -1;
		vx = -EYE_WALKING_SPEED;
		break;
	case EYE_STATE_WALKING_RIGHT:
		nx = 1;
		vx = EYE_WALKING_SPEED;
	}
}
/*if
void CEye::CatchPlayer()
{
	if (GetDistance(tank) <= 300)
	{
		tank->GetPosition(this->target_x, this->target_y);

		/*if (target_x > x) {
			if (target_y > y) {
				vx = EYE_WALKING_SPEED;
				vy = EYE_WALKING_SPEED;
				nx = 1;
				SetState(EYE_STATE_WALKING_RIGHT);
			}
			else
			{
				vx = EYE_WALKING_SPEED;
				vy = -EYE_WALKING_SPEED;
				nx = 1;
				SetState(EYE_STATE_WALKING_RIGHT);
			}
		}
		else
		{
			if (target_y > y) {
				vx = -EYE_WALKING_SPEED;
				vy = EYE_WALKING_SPEED;
				nx = -1;
				SetState(EYE_STATE_WALKING_LEFT);
			}
			else
			{
				vx = -EYE_WALKING_SPEED;
				vy = -EYE_WALKING_SPEED;
				nx = -1;
				SetState(EYE_STATE_WALKING_LEFT);
			}
		} 
		while (x != target_x)
		{
			if (x < target_x)
			{
				vx = EYE_WALKING_SPEED;
				nx = 1;
				SetState(EYE_STATE_WALKING_RIGHT);
			}
			if (x > target_x)
			{
				vx = -EYE_WALKING_SPEED;
				nx = -1;
				SetState(EYE_STATE_WALKING_LEFT);
			}
		}
		while (x = target_x)
		{
			if (y < target_y)
			{
				ny = 1;
				vy = EYE_WALKING_SPEED;
				SetState(EYE_STATE_WALKING_DOWN);
			}
			if (y > target_y)
			{
				ny = -1;
				vy = -EYE_WALKING_SPEED;
				SetState(EYE_STATE_WALKING_UP);
			}
		}

	}
} */

void CEye::SetPlayer(CTank* tank)
{
	this->tank = &(*tank);
}

void CEye::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
