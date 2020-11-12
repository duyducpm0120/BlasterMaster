#include "Tank.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Goomba.h"
#include "Utils.h"
#include "Golem.h"

CTank:: CTank(float x, float y)  : CGameObject()
{
	untouchable = 0;
	SetState(TANK_STATE_IDLE_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CTank::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += TANK_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != TANK_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > TANK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


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
				if (e->ny < 0)
				{
					if (golem->GetState() != GOLEM_STATE_DIE)
					{
						/*golem->SetState(GOLEM_STATE_DIE);*/
						SetState(TANK_STATE_DIE);
						vy = -TANK_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (golem->GetState() != GOLEM_STATE_DIE)
						{

							SetState(TANK_STATE_DIE);
						}
					}
				}
				
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			
			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	DebugOut(L"\n \n  vy: %d \t \n", state);
}

void CTank::Render()
{
	int ani;
	switch (state)
	{
	case TANK_STATE_WALKING_RIGHT:
		ani = TANK_ANI_WALKING_RIGHT;
		break;
	case TANK_STATE_WALKING_LEFT:
		ani = TANK_ANI_WALKING_LEFT;
		break;
	case TANK_STATE_JUMP_IDLE_LEFT:
		ani = TANK_ANI_JUMP_IDLE_LEFT;
		break;
	case TANK_STATE_JUMP_IDLE_RIGHT:
		ani = TANK_ANI_JUMP_IDLE_RIGHT;
		break;
	case TANK_STATE_IDLE_RIGHT:
		ani = TANK_ANI_IDLE_RIGHT;
		break;
	case TANK_STATE_IDLE_LEFT:
		ani = TANK_ANI_IDLE_LEFT;
		break;
	case TANK_STATE_DIE:
		ani = TANK_ANI_IDLE_LEFT;
		break;
	case TANK_STATE_JUMP_LEFT:
		ani = TANK_ANI_JUMP_LEFT;
		break;
	case TANK_STATE_JUMP_RIGHT:
		ani = TANK_ANI_JUMP_RIGHT;
		break;
	}


	int alpha = 255;
	if (untouchable) alpha = 128;
	/*if ((ani == TANK_ANI_WALKING_RIGHT || ani == TANK_ANI_IDLE_RIGHT || state == TANK_STATE_JUMP || state == TANK_STATE_DIE) && nx == 1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);*/
	animation_set->at(ani)->Render(x, y, alpha);
	//RenderBoundingBox();

}

void CTank::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TANK_STATE_WALKING_RIGHT:
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_WALKING_LEFT:
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_JUMP_IDLE_LEFT:
		nx = -1;
		vy = -TANK_JUMP_SPEED_Y;
		break;
	case TANK_STATE_JUMP_IDLE_RIGHT:
		nx = 1;
		vy = -TANK_JUMP_SPEED_Y;
		break;
	case TANK_STATE_IDLE_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case TANK_STATE_IDLE_LEFT:
		vx = 0;
		nx = -1;
		break;
	case TANK_STATE_DIE:
		vy = -TANK_DIE_DEFLECT_SPEED;
		break;
	case TANK_STATE_JUMP_LEFT:
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_JUMP_RIGHT:
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	}

}

void CTank::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TANK_BBOX_WIDTH;
	bottom = y + TANK_BBOX_HEIGHT;
}
void CTank::Reset()
{
	SetState(TANK_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}