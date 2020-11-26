#include "Rocket.h"
#include <algorithm>
#include "Golem.h"
#include "Game.h"
#include "Butterfly.h"
#include "Destroyed.h"
CRocket::CRocket()
{
	damage =9999;
	nx = 1;
	SetState(ROCKET_STATE_WALKING_LEFT);
}

void CRocket::findTarget()
{
	float minDistance = GetDistance(this->targetObject);
	CGameObject* target = this->targetObject;
	int i = 0, check = 0;
	for (; i < objects->size(); i++) {
		if (objects->at(i)->IsEnemy() == true && objects->at(i)->visible == true) {
			if (GetDistance(objects->at(i)) < minDistance)
			{
				target = objects->at(i);
				break;
			}
			check++;
			}
		}
	
	//if (check != 0)
		SetTargetObject(target);
	//else
	//	return;
}

void CRocket::SetTargetObjects(vector <LPGAMEOBJECT> *objects)
{
	this->objects = objects;
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
	int check = 0;
	for (int i = 0; i < objects->size(); i++) {
		if (objects->at(i)->IsEnemy())
			check++;
	}
	if (check == 0)
	{
		this->visible = false;
		return;
	}

	if (this->targetObject->visible == false)
		this->visible = false;
	
	findTarget();
	CatchTargetObject();
	


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
	//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//y += min_ty * dy + ny * 0.4f;
	x += dx;
	y += dy;

	if (nx != 0) vx = 0;
	if (ny != 0) vy = 0.00f;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];
		if (e->obj->IsEnemy()) {
			e->obj->TakeDamage(this->damage);			//Destroy every enemy
			this->visible = false;
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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

void CRocket::CatchTargetObject()
{
	targetObject->GetPosition(this->target_x, this->target_y);

	if (target_x > x-1) {
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
		if (target_y > y-1) {
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

void CRocket::SetTargetObject(CGameObject* object)
{
	this->targetObject = &(*object);
}