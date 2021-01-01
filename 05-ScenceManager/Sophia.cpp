#include "SOPHIA.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Utils.h"
#include "Golem.h"
#include "Item.h"
#include "Flame.h"
#include "Tank.h"
#include "Ladder.h"
CSophia::CSophia(float x, float y)
{
	isPlayer = true;
	nx = -1;
	bulletLevel = 1;
	//health = 8;
	//damage = 1;
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
	untouchableTime = 0;
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
	if (isTouchLadder == false)
		isClimbing = false;
	// Simple fall down
	if(!isClimbing)
		vy += SOPHIA_GRAVITY * dt;
	isTouchTank = false;	
	isTouchLadder = false;
	//isClimbing = false;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPCOLLISIONEVENT> coCollisoningEvents;
	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	CalcColliding(coObjects, coCollisoningEvents);

	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		if (dynamic_cast<CTank*>(e->obj))
			isTouchTank = true;
		else if (dynamic_cast<CLadder*>(e->obj))
		{
			isTouchLadder = true;
			climbingPositionX = e->obj->x + 2;
			if (y < e->obj->y)
				climbingPositionY = e->obj->y;
			else if (y > e->obj->y)
				climbingPositionY = e->obj->y + 192;
		}
		if (dynamic_cast<CPortal*>(e->obj))
		{
			CPortal* p = dynamic_cast<CPortal*>(e->obj);
			CGame* game = CGame::GetInstance();
			game->SwitchToScene(p->GetSceneId());
		}
	}

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

		
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CPortal*>(e->obj))
			{
				if (e->ny != -1 && e->ny != 1) {
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame* game = CGame::GetInstance();
					game->SwitchToScene(p->GetSceneId());
				}
			}
			else if (dynamic_cast<CTank*>(e->obj)) {
				isTouchTank = true;
			}
			else if (dynamic_cast<CLadder*>(e->obj)) {
				isTouchLadder = true;
				climbingPositionX = e->obj->x + 2;
				if (y < e->obj->y)
					climbingPositionY = e->obj->y;
				else if (y > e->obj->y)
					climbingPositionY = e->obj->y + 192;
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
				case ITEM_TYPE_THUNDER:
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
				if (untouchableTime == 0) {
					health-=2;
					untouchableTime = 1;
				}
				if (health <= 0)
					visible = false;
			}
			else {
				if (e->obj->IsEnemy()) {
					if (untouchableTime == 0) {
						health -= e->obj->GetDamage();
						vx -= 0.3f;
						untouchableTime = 1;
						e->obj->visible = false;
					}

					//vy -= 0.3f;
					if (health <= 0)
						visible = false;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//DebugOut(L"\n \n  touchtank: %d \t \n", isTouchTank);
	//DebugOut(L"\n \n  touchladder: %d \t \n", isTouchLadder);
	HandleUntouchableTime();
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
		if (nx == 1)
			ani = SOPHIA_ANI_IDLE_RIGHT;
		else if(nx == -1)
			ani = SOPHIA_ANI_IDLE_LEFT;
		break;
	case SOPHIA_STATE_CLIMBING_IDLE:
		ani = SOPHIA_ANI_CLIMBING_IDLE;
		break;
	case SOPHIA_STATE_CLIMBING_LADDER:
		ani = SOPHIA_ANI_CLIMBING_LADDER;
		break;
	}


	int alpha = 255;
	//if (untouchable) alpha = 128;
	if (untouchableTime % 10 < 5)
		animation_set->at(ani)->Render(x, y, 255);
	else
		animation_set->at(ani)->Render(x, y, 50);


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
	case SOPHIA_STATE_CLIMBING_LADDER:
		vx = 0;
		vy = 0;
		isClimbing = true;
		break;
	case SOPHIA_STATE_CLIMBING_IDLE:
		vx = 0;
		vy = 0;
		isClimbing = true;
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
void CSophia::CallSecondWeapon()
{
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

void CSophia::HandleUntouchableTime()
{

	if (untouchableTime == 0)
		return;
	untouchableTime += 1;
	if (untouchableTime == SOPHIA_UNTOUCHABLE_TIME)
		untouchableTime = 0;
}

void CSophia::GetClimbingPosition(float& x, float& y)
{
	x = climbingPositionX;
	y = climbingPositionY;
}
