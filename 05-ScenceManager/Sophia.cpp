#include "SOPHIA.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Utils.h"
#include "Golem.h"
#include "Item.h"
#include "Flame.h"
CSophia::CSophia(float x, float y)
{
	nx = -1;
	bulletLevel = 1;
	health = 8;
	damage = 1;
	isJumping = false;
	health = SOPHIA_START_HEALTH;
	damage = SOPHIA_START_DAMAGE;
	untouchable = 0;
	state = SOPHIA_STATE_IDLE_LEFT;
	SetState(SOPHIA_STATE_IDLE_LEFT);
	sophia_width = SOPHIA_BBOX_WIDTH;
	sophia_height = SOPHIA_BBOX_HEIGHT;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SOPHIA_STATE_DIE)
		return;
	if (damage > 4)
		bulletLevel = 2;
	else
		bulletLevel = 1;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (vy >= 0.0f && vy <= 0.35f)
		isJumping = false;

	// Simple fall down
	vy += SOPHIA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	if (state != SOPHIA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	/*if (GetTickCount() - untouchable_start > SOPHIA_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}*/

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
		if (ny != 0) vy = 0.00f;


		/*if (vy == 0)
			if (nx == -1)
				SetState( SOPHIA_STATE_IDLE_LEFT);
			else
				SetState( SOPHIA_STATE_IDLE_RIGHT);*/
				/*if(vy>0)
					DebugOut(L"vy: %f \t",vy);*/


					// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->IsEnemy()) {
				health -= e->obj->GetDamage();
				vx -= 0.3f;
				//vy -= 0.3f;
				if (health <= 0)
					visible = false;
			}
			else if (dynamic_cast<CItem*>(e->obj)) {
				switch (dynamic_cast<CItem*>(e->obj)->GetType())
				{
				case ITEM_TYPE_HEALTH:
				{
					if (health < 8 && health >0)
						health++;
					(e->obj)->visible = false;
					break;
				}
				case ITEM_TYPE_POWER:
				{
					if (damage < 8 && damage >0)
						damage++;
					(e->obj)->visible = false;
					break;
				}
				case ITEM_TYPE_ENABLE_ROCKET:
				{
					dynamic_cast<CItem*>(e->obj)->visible = false;
					break;
				}
				}
			}
			else if (dynamic_cast<CFlame*>(e->obj)) {
				health--;
				//vx -= (vx + 0.3f);
				vy -= 0.4f;
				if (health <= 0)
					visible = false;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	DebugOut(L"\n \n  vy: %f \t \n", vy);
}

void CSophia::Render()
{

	int ani;
	switch (state)
	{
		/*case SOPHIA_STATE_DIE:
			ani = SOPHIA_ANI_DIE;
			break;*/
	case SOPHIA_STATE_WALKING_RIGHT:
		ani = SOPHIA_ANI_WALKING_RIGHT;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		ani = SOPHIA_ANI_WALKING_LEFT;
		break;
	case SOPHIA_STATE_IDLE_RIGHT:
		ani = SOPHIA_ANI_IDLE_RIGHT;
		break;
	case SOPHIA_STATE_IDLE_LEFT:
		ani = SOPHIA_ANI_IDLE_LEFT;
		break;
	case SOPHIA_STATE_JUMP:
		if (nx = -1)
			ani = SOPHIA_ANI_IDLE_LEFT;
		else
			ani = SOPHIA_ANI_IDLE_RIGHT;
		break;
	}


	int alpha = 255;
	if (untouchable) alpha = 128;
	/*if ((ani == SOPHIA_ANI_WALKING_RIGHT || ani == SOPHIA_ANI_IDLE_RIGHT || state == SOPHIA_STATE_JUMP || state == SOPHIA_STATE_DIE) && nx == 1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);*/
	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();

}

void CSophia::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SOPHIA_STATE_WALKING_RIGHT:
		isJumping = false;
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		isJumping = false;
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_IDLE_RIGHT:
		isJumping = false;
		vx = 0;
		nx = 1;
		break;
	case SOPHIA_STATE_IDLE_LEFT:
		isJumping = false;
		vx = 0;
		nx = -1;
		break;
	case SOPHIA_STATE_DIE:
		//vy = -SOPHIA_DIE_DEFLECT_SPEED;
		//y -= 23;
		break;
	case SOPHIA_STATE_JUMP:
		vy -= SOPHIA_JUMP_SPEED_Y;
		break;
	}

}

void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + sophia_width;
	bottom = y + sophia_height;
}
void CSophia::SetDimension(int width, int height)
{
	this->sophia_width = width;
	this->sophia_height = height;
}
void CSophia::GetDimension(int& width, int& height)
{
	width = this->sophia_width;
	height = this->sophia_height;
}
void CSophia::Reset()
{
	SetState(SOPHIA_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}