#include "BossArm.h"

void BossArm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 32;
}

void BossArm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->type == TYPE_LEFT_CLAW)
	{
		float x_, y_;
		Boss->getLeftClawPos(x_, y_);
		this->x = x_;
		this->y = y_;
	}
	else if (this->type == TYPE_RIGHT_CLAW)
	{
		float x_, y_;
		Boss->getRightClawPos(x_, y_);
		this->x = x_;
		this->y = y_;
	}

	if (Boss->visible == false) visible = false;
}

void BossArm::Render()
{
	RenderBoundingBox();
}

BossArm::BossArm(CBoss* boss, int _type)
{
	type = _type;
	isEnemy = true;
	damage = 1;
	health = 10000;
	this->Boss = boss;
	x = Boss->x;
	y = Boss->y;
	visible = true;
	
}
