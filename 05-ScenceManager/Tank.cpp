#include "Tank.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Utils.h"
#include "Golem.h"
#include "Item.h"
#include "Flame.h"
#include "Rocket.h"
#include "Brick.h"
#include "Bullet.h"
#include "Sophia.h"
CTank:: CTank(float x, float y) 
{
	bulletLevel = 1;
	enableRocket = true;
	isJumping = false;
	untouchable = 0;
	SetState(TANK_STATE_IDLE_RIGHT);
	tank_width = TANK_NORMAL_WIDTH;
	tank_height = TANK_NORMAL_HEIGHT;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	untouchableTime = 0;
}

void CTank::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == TANK_STATE_DIE)
		return;
	if (damage > 4)
		bulletLevel = 2;
	else
		bulletLevel = 1;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += TANK_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPCOLLISIONEVENT> coCollisoningEvents;

	
	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	CalcColliding(coObjects, coCollisoningEvents);
	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		if (dynamic_cast<CBrick*>(e->obj))
		{
		}
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
		
		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CSophia*>(e->obj)) {
				x += dx ;
				y += dy + ny * 0.4; ;
			}
			else {
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + (ny < 0 ? ny : 0) * 0.4f;
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			
		}*/

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + (ny < 0 ? ny : 0) * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


			if (e->obj->IsEnemy()) {	
				if (untouchableTime == 0) {
					health -= e->obj->GetDamage();
					untouchableTime = 1;
				}
				
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
							health ++;
						(e->obj)->visible = false;
						break;
					}
					case ITEM_TYPE_POWER:
					{
						if(damage < 8 && damage >0)
							damage++;
						(e->obj)->visible = false;
						break;
					}
					case ITEM_TYPE_ENABLE_ROCKET:
					{
						enableRocket = true;
						dynamic_cast<CItem*>(e->obj)->visible = false;
						break;
					}
				}
			}
			else if (dynamic_cast<CFlame*>(e->obj)) {
				if (untouchableTime == 0) {
					health--;
					untouchableTime = 1;
				}
				if (health <= 0)
					visible = false;
			}
			else if (dynamic_cast<CSophia*>(e->obj)) {
					x += - nx * 0.4f;
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame* game = CGame::GetInstance();				
				game->SwitchScene(p->GetSceneId());
			}
			
			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];	
	DebugOut(L"\n \n  Result size: %d \t \n", coEventsResult.size());
	HandleUntouchableTime();

}

void CTank::Render()
{
	
	int ani;
	switch (state)
	{
	/*case TANK_STATE_DIE:
		ani = TANK_ANI_DIE;
		break;*/
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
	case TANK_STATE_JUMP_LEFT:
		ani = TANK_ANI_JUMP_LEFT;
		break;
	case TANK_STATE_JUMP_RIGHT:
		ani = TANK_ANI_JUMP_RIGHT;
		break;
	case TANK_STATE_UPING_GUN_LEFT:
		ani = TANK_ANI_UPING_GUN_LEFT;
		break;
	case TANK_STATE_UPING_GUN_RIGHT:
		ani = TANK_ANI_UPING_GUN_RIGHT;
		break;
	case TANK_STATE_UP_GUN_LEFT:
		ani = TANK_ANI_UP_GUN_LEFT;
		break;
	case TANK_STATE_UP_GUN_RIGHT:
		ani = TANK_ANI_UP_GUN_RIGHT;
		break;
	case TANK_STATE_UP_GUN_WALKING_LEFT:
		ani = TANK_ANI_UP_GUN_WALKING_LEFT;
		break;
	case TANK_STATE_UP_GUN_WALKING_RIGHT:
		ani = TANK_ANI_UP_GUN_WALKING_RIGHT;
		break;
	}


	int alpha = 255;	
	/*if ((ani == TANK_ANI_WALKING_RIGHT || ani == TANK_ANI_IDLE_RIGHT || state == TANK_STATE_JUMP || state == TANK_STATE_DIE) && nx == 1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);*/
	if(untouchableTime % 10 < 5)
		animation_set->at(ani)->Render(x, y, alpha);
	else
		animation_set->at(ani)->Render(x, y, 50);

	
	//RenderBoundingBox();

}

void CTank::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TANK_STATE_WALKING_RIGHT:
		isJumping = false;
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_WALKING_LEFT:
		isJumping = false;
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_JUMP_IDLE_LEFT:
		isJumping = false;
		nx = -1;
		vy = -TANK_JUMP_SPEED_Y;
		break;
	case TANK_STATE_JUMP_IDLE_RIGHT:
		isJumping = true;
		nx = 1;
		vy = -TANK_JUMP_SPEED_Y;
		break;
	case TANK_STATE_IDLE_RIGHT:
		isJumping = false;
		vx = 0;
		nx = 1;
		break;
	case TANK_STATE_IDLE_LEFT:
		isJumping = false;
		vx = 0;
		nx = -1;
		break;
	case TANK_STATE_DIE:
		//vy = -TANK_DIE_DEFLECT_SPEED;
		y -= 23;
		break;
	case TANK_STATE_JUMP_LEFT:
		isJumping = true;
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_JUMP_RIGHT:
		isJumping = true;
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_UP_GUN_WALKING_LEFT:
		isJumping = false;
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_UP_GUN_WALKING_RIGHT:
		isJumping = false;
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_UP_GUN_LEFT:
		isJumping = false;
		vx = 0;
		nx = -1;
		break;
	case TANK_STATE_UP_GUN_RIGHT:
		isJumping = false;
		vx = 0;
		nx = 1;
		break;
	}

}

void CTank::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + tank_width;
	bottom = y + tank_height;
}
void CTank::SetDimension(int width, int height)
{
	this->tank_width = width;
	this->tank_height = height;
}
void CTank::GetDimension(int& width, int& height)
{
	width = this->tank_width;
	height = this->tank_height;
}
void CTank::Reset()
{
	SetState(TANK_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CTank::HandleUntouchableTime()
{
	if (untouchableTime == 0)
		return;
	untouchableTime += 1;
	if (untouchableTime == TANK_UNTOUCHABLE_TIME)
		untouchableTime = 0;
}
