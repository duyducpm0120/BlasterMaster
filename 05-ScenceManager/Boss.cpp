#include "Boss.h"

CBoss::CBoss() :
	BigClawLeft(18),
	BigClawRight(18)
{
	
	damage = 1;
	health = 3;
	nx = -1;
	SetState(Boss_STATE_WALKING_LEFT);
	vx = -Boss_WALKING_SPEED;
	vy = Boss_WALKING_SPEED;
}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + Boss_BBOX_WIDTH;

	if (state == Boss_STATE_DIE)
		bottom = y + Boss_BBOX_HEIGHT_DIE;
	else
		bottom = y + Boss_BBOX_HEIGHT;
}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure Boss can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;


	if (vx < 0 && x < (startX - 100)) {
		x = startX - 100;
		SetState(Boss_STATE_WALKING_RIGHT);
		nx = 1;
		vx = Boss_WALKING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -Boss_WALKING_SPEED;
		SetState(Boss_STATE_WALKING_LEFT);
	}
	if (vy < 0 && y < (startY - 50)) {
		y = startY - 50;
		vy = Boss_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -Boss_WALKING_SPEED;
	}
	this->BigClawLeft.x = this->x-60;
	this->BigClawLeft.y = this->y;
	this->BigClawRight.x = this->x + 60;
	this->BigClawRight.y = this->y;
}


void CBoss::Render()
{
	int ani;
	
		ani = Boss_ANI_WALKING_LEFT;



	/*if (state == Boss_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
	BigClawLeft.Render();
	BigClawRight.Render();
}

void CBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case Boss_STATE_DIE:
		//y += Boss_BBOX_HEIGHT - Boss_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case Boss_STATE_WALKING_LEFT:

		break;
	case Boss_STATE_WALKING_RIGHT:
		nx = 1;
		vx = Boss_WALKING_SPEED;
	}
}

void CBoss::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}

CBoss::BossClawSection::BossClawSection(int anisetid)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(anisetid));
}

void CBoss::BossClawSection::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CBoss::BossClawSection::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CBoss::BossClawSection::Render()
{
	animation_set->at(0)->Render(x, y);
}
