#include "PLAYER.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Goomba.h"
#include "Utils.h"
#include "Golem.h"
#include "Item.h"
#include "Flame.h"
CPlayer::CPlayer(float x, float y) : CGameObject()
{
	bulletLevel = 1;
	enableRocket = true;
	isJumping = false;
	health = PLAYER_START_HEALTH;
	damage = PLAYER_START_DAMAGE;
	untouchable = 0;
	SetState(PLAYER_STATE_IDLE_RIGHT);
	PLAYER_width = PLAYER_NORMAL_WIDTH;
	PLAYER_height = PLAYER_NORMAL_HEIGHT;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PLAYER_STATE_DIE)
		return;
	if (damage > 4)
		bulletLevel = 2;
	else
		bulletLevel = 1;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += PLAYER_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	if (state != PLAYER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	/*if (GetTickCount() - untouchable_start > PLAYER_UNTOUCHABLE_TIME)
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
				SetState( PLAYER_STATE_IDLE_LEFT);
			else
				SetState( PLAYER_STATE_IDLE_RIGHT);*/
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
					enableRocket = true;
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
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame* game = CGame::GetInstance();
				game->SetPlayerHealth(this->health);
				game->SetPlayerPower(this->damage);
				game->SwitchScene(p->GetSceneId());
			}


		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//DebugOut(L"\n \n  state: %d \t \n", state);
}

void CPlayer::Render()
{

	int ani;
	switch (state)
	{
		/*case PLAYER_STATE_DIE:
			ani = PLAYER_ANI_DIE;
			break;*/
	case PLAYER_STATE_WALKING_RIGHT:
		ani = PLAYER_ANI_WALKING_RIGHT;
		break;
	case PLAYER_STATE_WALKING_LEFT:
		ani = PLAYER_ANI_WALKING_LEFT;
		break;
	case PLAYER_STATE_JUMP_IDLE_LEFT:
		ani = PLAYER_ANI_JUMP_IDLE_LEFT;
		break;
	case PLAYER_STATE_JUMP_IDLE_RIGHT:
		ani = PLAYER_ANI_JUMP_IDLE_RIGHT;
		break;
	case PLAYER_STATE_IDLE_RIGHT:
		ani = PLAYER_ANI_IDLE_RIGHT;
		break;
	case PLAYER_STATE_IDLE_LEFT:
		ani = PLAYER_ANI_IDLE_LEFT;
		break;
	case PLAYER_STATE_JUMP_LEFT:
		ani = PLAYER_ANI_JUMP_LEFT;
		break;
	case PLAYER_STATE_JUMP_RIGHT:
		ani = PLAYER_ANI_JUMP_RIGHT;
		break;
	case PLAYER_STATE_UPING_GUN_LEFT:
		ani = PLAYER_ANI_UPING_GUN_LEFT;
		break;
	case PLAYER_STATE_UPING_GUN_RIGHT:
		ani = PLAYER_ANI_UPING_GUN_RIGHT;
		break;
	case PLAYER_STATE_UP_GUN_LEFT:
		ani = PLAYER_ANI_UP_GUN_LEFT;
		break;
	case PLAYER_STATE_UP_GUN_RIGHT:
		ani = PLAYER_ANI_UP_GUN_RIGHT;
		break;
	case PLAYER_STATE_UP_GUN_WALKING_LEFT:
		ani = PLAYER_ANI_UP_GUN_WALKING_LEFT;
		break;
	case PLAYER_STATE_UP_GUN_WALKING_RIGHT:
		ani = PLAYER_ANI_UP_GUN_WALKING_RIGHT;
		break;
	}


	int alpha = 255;
	if (untouchable) alpha = 128;
	/*if ((ani == PLAYER_ANI_WALKING_RIGHT || ani == PLAYER_ANI_IDLE_RIGHT || state == PLAYER_STATE_JUMP || state == PLAYER_STATE_DIE) && nx == 1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);*/
	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();

}

void CPlayer::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PLAYER_STATE_WALKING_RIGHT:
		isJumping = false;
		vx = PLAYER_WALKING_SPEED;
		nx = 1;
		break;
	case PLAYER_STATE_WALKING_LEFT:
		isJumping = false;
		vx = -PLAYER_WALKING_SPEED;
		nx = -1;
		break;
	case PLAYER_STATE_JUMP_IDLE_LEFT:
		isJumping = false;
		nx = -1;
		vy = -PLAYER_JUMP_SPEED_Y;
		break;
	case PLAYER_STATE_JUMP_IDLE_RIGHT:
		isJumping = true;
		nx = 1;
		vy = -PLAYER_JUMP_SPEED_Y;
		break;
	case PLAYER_STATE_IDLE_RIGHT:
		isJumping = false;
		vx = 0;
		nx = 1;
		break;
	case PLAYER_STATE_IDLE_LEFT:
		isJumping = false;
		vx = 0;
		nx = -1;
		break;
	case PLAYER_STATE_DIE:
		//vy = -PLAYER_DIE_DEFLECT_SPEED;
		y -= 23;
		break;
	case PLAYER_STATE_JUMP_LEFT:
		isJumping = true;
		vx = -PLAYER_WALKING_SPEED;
		nx = -1;
		break;
	case PLAYER_STATE_JUMP_RIGHT:
		isJumping = true;
		vx = PLAYER_WALKING_SPEED;
		nx = 1;
		break;
	case PLAYER_STATE_UP_GUN_WALKING_LEFT:
		isJumping = false;
		vx = -PLAYER_WALKING_SPEED;
		nx = -1;
		break;
	case PLAYER_STATE_UP_GUN_WALKING_RIGHT:
		isJumping = false;
		vx = PLAYER_WALKING_SPEED;
		nx = 1;
		break;
	case PLAYER_STATE_UP_GUN_LEFT:
		isJumping = false;
		vx = 0;
		nx = -1;
		break;
	case PLAYER_STATE_UP_GUN_RIGHT:
		isJumping = false;
		vx = 0;
		nx = 1;
		break;
	}

}

void CPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PLAYER_width;
	bottom = y + PLAYER_height;
}
void CPlayer::SetDimension(int width, int height)
{
	this->PLAYER_width = width;
	this->PLAYER_height = height;
}
void CPlayer::GetDimension(int& width, int& height)
{
	width = this->PLAYER_width;
	height = this->PLAYER_height;
}
void CPlayer::Reset()
{
	SetState(PLAYER_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}