#include "EnemyBullet.h"
#include "Player.h"
#include "Brick.h"
#include "GameObject.h"
#include "PlayScence.h"
#include "Vec2.h"
float CEnemyBullet::CaclDistance()
{
	Vec2 pos = Vec2(x, y);
	Vec2 old = Vec2(startPositionX, startPositionY);

	return (pos - old).GetLength();
}
void CEnemyBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == BULLET_STATE_FLYING_LEFT || state == BULLET_STATE_FLYING_RIGHT) {
		right = x + 10;
		bottom = y + 10;
	}
	else
	{
		right = x + 10;
		bottom = y + 10;
	}
}

void CEnemyBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	else if (state == BULLET_STATE_FLYING_RIGHT) {
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
	else if (state == BULLET_STATE_FLYING_UP)
	{
		SetSpeed(0.0f, -BULLET_SPEED);
		if (y < (startPositionY - BULLET_FLYING_SPACE)) {
			visible = false;
		}
		else
		{
			x += dx;
			y += dy;
		}
		
	}
	else if (state == BULLET_STATE_UNDEF|| state==BULLET_STATE_BOSS)
	{
		if (this->CaclDistance() >= 100) this->visible = false;
		else {
			x += dx;
			y += dy;
		}
			
		
	}

	else //Flying Down
	{
		SetSpeed(0.0f, BULLET_SPEED);
		if (y > (startPositionY + BULLET_FLYING_SPACE)) {
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


	vector<LPCOLLISIONEVENT> coCollisoningEvents;
	CalcColliding(coObjects, coCollisoningEvents);

	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		//if (dynamic_cast<CPlayer*>(e->obj)) {
		//	e->obj->TakeDamage(this->damage);				//Destroy every enemy
		//	this->visible = false;
		//}
		//else if (dynamic_cast<CBrick*>(e->obj)) {
		//	this->visible = false;
		//}
	}


	float min_tx, min_ty, nx = 0, ny;
	float rtx = 0;
	float rty = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rtx, rty);
	x += dx;
	y += dy;
	// block 
	//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//y += min_ty * dy + ny * 0.4f;


	if (nx != 0) vx = 0;
	if (ny != 0) vy = 0;

	// Collision logic with Goombas
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CPlayer*>(e->obj)) {
			e->obj->TakeDamage(this->damage);				//Destroy every enemy
			this->visible = false;
		}
		else if (dynamic_cast<CBrick*>(e->obj)) {
			this->visible = false;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CEnemyBullet::Render()
{
	if (state == BULLET_STATE_BOSS) animation_set->at(1)->Render(x, y);
	else
	animation_set->at(0)->Render(x, y);
	
}

CEnemyBullet::CEnemyBullet()
{
	damage = 1;

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(61));
}

CEnemyBullet::CEnemyBullet(int state)
{
	damage = 1;

	isEnemy = true;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(61));
	this->state = state;
}



