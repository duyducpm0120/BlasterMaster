#include "Rocket.h"
#include <algorithm>
#include "Golem.h"
#include "Game.h"
#include "Butterfly.h"
CRocket::CRocket()
{
	nx = 1;
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

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed


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

		// block 
		x += min_tx * dx + nx * 0.02f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.02f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0.00f;


		/*if (vy == 0)
			if (nx == -1)
				SetState( TANK_STATE_IDLE_LEFT);
			else
				SetState( TANK_STATE_IDLE_RIGHT);*/
				/*if(vy>0)
					DebugOut(L"vy: %f \t",vy);*/


					// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGolem*>(e->obj))
			{
				CGolem* golem = dynamic_cast<CGolem*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (state != TANK_STATE_DIE)
					if (e->ny < 0)
					{
						if (golem->GetState() != GOLEM_STATE_DIE)
						{
							/*golem->SetState(GOLEM_STATE_DIE);*/
							SetState(TANK_STATE_DIE);
							//vy = -TANK_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{						
							if (golem->GetState() != GOLEM_STATE_DIE)
							{

								SetState(TANK_STATE_DIE);
							}
					}

			}
			
			else if (dynamic_cast<CButterfly*>(e->obj))
			{
				CButterfly* butter = dynamic_cast<CButterfly*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				butter->visible = false;
				this->visible = false;

			}


		}
	}

	// clean up collision events
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

void CRocket::CatchObject()
{
	object->GetPosition(this->target_x, this->target_y);
}

void CRocket::SetTargetObject(CGameObject* object)
{
	this->object = &(*object);
}

