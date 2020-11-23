#include "Jason.h"
#include <algorithm>
#include "debug.h"

#include "Game.h"

#include "Goomba.h"
#include "Enemy_Flower.h"
#include "debug.h"

void CJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += JASON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != JASON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > JASON_UNTOUCHABLE_TIME)
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						DebugOut(L"Collided");
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -JASON_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							
							SetState(JASON_STATE_DIE);
						}
					}
				}
			}


			//flower		
			if (dynamic_cast<CEnemy_Flower*>(e->obj)) // if e->obj is Goomba 
			{
				CEnemy_Flower* goomba = dynamic_cast<CEnemy_Flower*>(e->obj);
				
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
								SetState(JASON_STATE_DIE);
						}
					}
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CJason::Render()
{
	int ani;
	if (state == JASON_STATE_DIE)
		ani = JASON_ANI_DIE;
	else if (state == JASON_STATE_JUMP) {
		if (nx == 1)
			ani = JASON_ANI_IDLE_RIGHT;
		else
			ani = JASON_ANI_IDLE_LEFT;
	}
	else
	{		
			if (vx == 0)
			{
				if (nx > 0) ani = JASON_ANI_IDLE_RIGHT;
				else ani = JASON_ANI_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = JASON_ANI_WALKING_RIGHT;
			else ani = JASON_ANI_WALKING_LEFT;
		}
		

	int alpha = 255;
	if (untouchable) alpha = 128;
	if ((ani == JASON_ANI_WALKING_RIGHT || ani == JASON_ANI_IDLE_RIGHT || state == JASON_STATE_JUMP || state == JASON_STATE_DIE ) && nx==1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);
	DebugOut(L"[INFO] vy: %f\n", vy);
}

void CJason::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case JASON_STATE_WALKING_RIGHT:
		vx = JASON_WALKING_SPEED;
		nx = 1;
		break;
	case JASON_STATE_WALKING_LEFT:
		vx = -JASON_WALKING_SPEED;
		nx = -1;
		break;
	case JASON_STATE_JUMP:
		vy = -JASON_JUMP_SPEED_Y;
	case JASON_STATE_IDLE_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case JASON_STATE_IDLE_LEFT:
		vx = 0;
		nx = -1;
		break;
	case JASON_STATE_DIE:
		vy = -JASON_DIE_DEFLECT_SPEED;
		break;
	}
}

void CJason::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + JASON_BBOX_WIDTH;
	bottom = y + JASON_BBOX_HEIGHT;
}
