#include "Boss.h"
#include "Utils.h"
CBoss::CBoss() :
	BigClawLeft(18),
	BigClawRight(18)
	
{
	
	damage = 1;
	health = 100;
	nx = -1;
	SetState(Boss_STATE_WALKING_LEFT);
	vx = -Boss_WALKING_SPEED;
	vy = Boss_WALKING_SPEED;
	LeftClawTargetRandomMovingObject = new BossClawSection();
	LeftClawTargetRandomMovingObject->SetPosition(200, 100);
	LeftClawTargetRandomMovingObject->vx = 0;
	LeftClawTargetRandomMovingObject->vy = ARM_SPEED;
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject)->setTarget(NULL);
	init();
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
	
	
	/*if(LeftClawTargetRandomMovingObject->y < 200)
		LeftClawTargetRandomMovingObject->y += 3;*/


	for (int i = 0; i < 5; i++)
	{
		this->LeftArm[i].Update(dt, coObjects);
	}
	BigClawLeft.Update(dt, coObjects);
	LeftClawTargetRandomMovingObject->Update(dt, coObjects);
	//BigClawRight.Update(dt, coObjects);
}


void CBoss::Render()
{
	int ani;
	
	ani = Boss_ANI_WALKING_LEFT;
	animation_set->at(ani)->Render(x, y);
	RenderArms();
	RenderBoundingBox();
	//LeftClawTargetRandomMovingObject->RenderBoundingBox();
	
}

void CBoss::init()
{
	for (int i = 0; i < 4; i++)
	{
		this->LeftArm[i].setTarget(&this->LeftArm[i + 1]);
		
	}
	this->LeftArm[4].setTarget(&this->BigClawLeft);
	this->BigClawLeft.setTarget(LeftClawTargetRandomMovingObject);
}

void CBoss::RenderArms()
{
	BigClawLeft.Render();
	BigClawRight.Render();
	for (int i = 0; i < 5; i++) LeftArm[i].Render();
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
	this->x = 75;
	this->y = 75;
	this->vx = ARM_SPEED;
	this->vy = 0.05 * 3;
	this->target = NULL;
	angle = 0;
}

CBoss::BossClawSection::BossClawSection()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(17));
	this->x = 0;
	this->y = 0;
	damage = 1;
	this->vx = ARM_SPEED;
	this->vy = ARM_SPEED;
	this->target = NULL;
}

void CBoss::BossClawSection::GetBoundingBox(float& left, float& top, float& right, float& bottom) 
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + 15;
	bottom = y + 15;
}

void CBoss::BossClawSection::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->target != NULL) {
		Follow();
		CGameObject::Update(dt, coObjects);
		x += dx;
		y += dy;
	}
	else
	{
		angle += 0.174444444 * dt / 50;
		x =  200 + 50* cos(angle);
		y = 100 + 50 * sin(angle);
		SetPosition(x, y);
	}
}

void CBoss::BossClawSection::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBoss::BossClawSection::Follow()
{
		if (target->x - 15 > this->x) {
			if (target->y - 15 > this->y) {
				vx = ARM_SPEED;
				vy = ARM_SPEED;
				nx = 1;

			}
			else
			{
				vx = ARM_SPEED;
				vy = -ARM_SPEED;
				nx = 1;

			}
		}
		else
		{
			if (target->y - 15 > this->y) {
				vx = -ARM_SPEED;
				vy = ARM_SPEED;
				nx = -1;

			}
			else
			{
				vx = -ARM_SPEED;
				vy = -ARM_SPEED;
				nx = -1;
			}
		}
}

void CBoss::BossClawSection::setTarget(CGameObject* target)
{
	this->target = target;
}
