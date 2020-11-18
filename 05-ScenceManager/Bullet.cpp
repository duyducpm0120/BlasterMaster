#include "Bullet.h"
#include "Goomba.h"
#include "Destroyed.h"
#include "PlayScence.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == BULLET_STATE_FLYING_LEFT || state == BULLET_STATE_FLYING_RIGHT) {
		right = x + BULLET_HORIZONTAL_BBOX_WIDTH;
		bottom = y + BULLET_HORIZONTAL_BBOX_HEIGHT;
	}
	else
	{
		right = x + BULLET_VERTICAL_BBOX_WIDTH;
		bottom = y + BULLET_VERTICAL_BBOX_HEIGHT;
	}
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (state == BULLET_STATE_FLYING_LEFT) {
		SetSpeed(-BULLET_SPEED, 0.0f);
		if (x < startPositionX - BULLET_FLYING_SPACE)
		{
			visible = false;
			return;

		}
		else {
			x += dx;
			y += dy;
		}

	}
	else if(state== BULLET_STATE_FLYING_RIGHT) {
		SetSpeed(BULLET_SPEED, 0.0f);
		if (x > startPositionX + BULLET_FLYING_SPACE) {
			visible = false;
			return;

		}
		else
		{
			x += dx;
			y += dy;
		}
	}
	else
	{
		SetSpeed(0.0f, -BULLET_SPEED);
		if (y < startPositionY - BULLET_FLYING_SPACE) {
			visible = false;
		}
		else
		{
			x += dx;
			y += dy;
		}
		
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rtx = 0;
		float rty = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny,rtx,rty);

		// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->IsEnemy()) {
				e->obj->visible = false;				//Destroy every enemy
				this->visible = false;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CBullet::Render()
{
	int ani;
	ani = BULLET_ANI_FLYING_LEFT_LV1;
	if (state == BULLET_STATE_FLYING_UP) {
		if (level == 1)
			ani = BULLET_ANI_FLYING_UP_LV1;
		else
			ani = BULLET_ANI_FLYING_UP_LV2;
	}
	else if (state == BULLET_STATE_FLYING_LEFT) {
		if (level == 1)
			ani = BULLET_ANI_FLYING_LEFT_LV1;
		else
			ani = BULLET_ANI_FLYING_LEFT_LV2;
	}
	else if (state == BULLET_STATE_FLYING_RIGHT) {
		if (level == 1)
			ani = BULLET_ANI_FLYING_RIGHT_LV1;
		else
			ani = BULLET_ANI_FLYING_RIGHT_LV2;
	}
	animation_set->at(ani)->Render(x, y, 255);

}

CBullet::CBullet(int level , int state) : CGameObject()
{
	this->level = level;
	this->state = state;
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	return;
}

void CBullet::SetStartPositon(float x, float y)
{
	startPositionX = x;
	startPositionY = y;
}

int CBullet::GetState()
{
	return this->state;
}


