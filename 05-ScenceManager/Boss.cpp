#include "Boss.h"
#include "Utils.h"
CBoss::CBoss() :
	BigClawLeft(18),
	BigClawRight(18)
	
{
	count = 0;
	damage = 1;
	health = 100;
	nx = -1;
	SetState(Boss_STATE_WALKING_LEFT);
	vx = -Boss_WALKING_SPEED;
	vy = Boss_WALKING_SPEED;
	LeftClawTargetRandomMovingObject1 = new BossClawSection();
	LeftClawTargetRandomMovingObject1->SetPosition(x + 60 + 7 * 15, y + 30 + 7 * 15);

	LeftClawTargetRandomMovingObject1->vx = 0;
	LeftClawTargetRandomMovingObject1->vy = ARM_SPEED;
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->setMovingSpace(80, 15);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->setStartPosition(x + 60 + 7 * 15, y + 30 + 7 * 15);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->setTarget(NULL);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->setnI(7);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->setBaseBoss(this);

	LeftClawTargetRandomMovingObject2 = new BossClawSection();
	LeftClawTargetRandomMovingObject2->SetPosition(x + 60 + 7 * 15, y + 30 + 7 * 15);
	LeftClawTargetRandomMovingObject2->vx = 0;
	LeftClawTargetRandomMovingObject2->vy = ARM_SPEED;
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->setMovingSpace(15, 80);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->setStartPosition(x + 60 + 7 * 15, y + 30 + 7 * 15);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->setTarget(NULL);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->setnI(7);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->setBaseBoss(this);

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


	if (vx < 0 && x < (startX - 200)) {
		x = startX - 200;
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
		this->LeftArm[i].SetBossLastMove(dx, dy);
		//LeftArm[i].FollowBoss();
		this->LeftArm[i].Update(dt, coObjects);
		
	}
	
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->SetBossLastMove(dx, dy);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject1)->FollowBoss();
	LeftClawTargetRandomMovingObject1->Update(dt, coObjects);
	
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->SetBossLastMove(dx, dy);
	dynamic_cast<BossClawSection*> (LeftClawTargetRandomMovingObject2)->FollowBoss();
	LeftClawTargetRandomMovingObject2->Update(dt, coObjects);
	count++;
	if (count % 300 == 0) {

		srand(time(NULL));
		int n = rand() % 2;
		if (n == 1)
		{
			BigClawLeft.setTarget(LeftClawTargetRandomMovingObject1);
		}
		else
			BigClawLeft.setTarget(LeftClawTargetRandomMovingObject2);
	}
	BigClawLeft.SetBossLastMove(dx, dy);
	//BigClawLeft.FollowBoss();
	BigClawLeft.Update(dt, coObjects);
	
	//BigClawRight.Update(dt, coObjects);
}


void CBoss::Render()
{
	int ani;
	
	ani = Boss_ANI_WALKING_LEFT;
	animation_set->at(ani)->Render(x, y);
	RenderArms();
	RenderBoundingBox();
	LeftClawTargetRandomMovingObject1->RenderBoundingBox();
	LeftClawTargetRandomMovingObject2->RenderBoundingBox();
	
}

void CBoss::init()
{
	for (int i = 0; i < 4; i++)
	{
		this->LeftArm[i].setTarget(&this->LeftArm[i + 1]);
		this->LeftArm[i].setnI(i);
		LeftArm[i].setBaseBoss(this);
		LeftArm[i].SetPosition(x + 60 + i * 15, y+30 + i * 15);
	}
	this->LeftArm[4].setTarget(&this->BigClawLeft);
	this->LeftArm[4].setnI(4);
	LeftArm[4].setBaseBoss(this);
	LeftArm[4].SetPosition(x + 60 + 4 * 15, y + 30 + 4 * 15);
	this->BigClawLeft.setTarget(LeftClawTargetRandomMovingObject1);
	this->BigClawLeft.setnI(5);
	BigClawLeft.setBaseBoss(this);
	BigClawLeft.SetPosition(x + 60 + 5 * 15, y + 30 + 5 * 15);
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
	this->vx = ARM_SPEED;
	this->vy = 0.05 * 3;
	this->target = NULL;
	angle = 0;
}

CBoss::BossClawSection::BossClawSection()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(17));
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
		x =  this->startX + MovingSpaceX* cos(angle);
		y = this->startY + MovingSpaceY * sin(angle);
		//SetPosition(x, y);
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

void CBoss::BossClawSection::setMovingSpace(int x, int y)
{
	MovingSpaceX = x;
	MovingSpaceY = y;
}

void CBoss::BossClawSection::FollowBoss()
{
	x += bossLastMoveX;
	y += bossLastMoveY;
	startX += bossLastMoveX;
	startY += bossLastMoveY;
}
