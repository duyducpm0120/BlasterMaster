#include "Rocket.h"
CRocket::CRocket()
{
	nx = -1;
	SetState(ROCKET_STATE_WALKING_LEFT);
}

void CRocket::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ROCKET_BBOX_WIDTH;
	bottom = y + ROCKET_BBOX_HEIGHT;
}

void CRocket::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Golem can interact with the world and to each of them too!
	// 
	CatchObject();
	if (GetDistance(object) == 0) {
		this->visible = false;
		return;
	};
	x += dx;
	y += dy;

	if (target_x > x) {
		if (target_y > y) {
			vx = ROCKET_WALKING_SPEED;
			vy = ROCKET_WALKING_SPEED;
			nx = 1;
			SetState(ROCKET_STATE_WALKING_RIGHT);
		}
		else
		{
			vx = ROCKET_WALKING_SPEED;
			vy = -ROCKET_WALKING_SPEED;
			nx = 1;
			SetState(ROCKET_STATE_WALKING_RIGHT);
		}
	}
	else
	{
		if (target_y > y) {
			vx = -ROCKET_WALKING_SPEED;
			vy = ROCKET_WALKING_SPEED;
			nx = -1;
			SetState(ROCKET_STATE_WALKING_LEFT);
		}
		else
		{
			vx = -ROCKET_WALKING_SPEED;
			vy = -ROCKET_WALKING_SPEED;
			nx = -1;
			SetState(ROCKET_STATE_WALKING_LEFT);
		}
	}


}

void CRocket::Render()
{
	int ani;
	if (state == ROCKET_STATE_WALKING_LEFT)
		ani = ROCKET_ANI_WALKING_LEFT;
	else
		ani = ROCKET_ANI_WALKING_RIGHT;


	/*if (state == ROCKET_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CRocket::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ROCKET_STATE_DIE:
		//y += ROCKET_BBOX_HEIGHT - ROCKET_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case ROCKET_STATE_WALKING_LEFT:
		break;
	case ROCKET_STATE_WALKING_RIGHT:
		break;
	}
}

void CRocket::CatchObject()
{
	object->GetPosition(this->target_x, this->target_y);
}

void CRocket::SetTargetObject(CGameObject* object)
{
	this->object = &(*object);
}

