#include "Boss.h"
#include "Utils.h"
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
	Init();
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
	if (vy < 0 && y < (startY - 100)) {
		y = startY - 100;
		vy = Boss_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -Boss_WALKING_SPEED;
	}
	
	this->BigClawRight.x = this->x + 60;
	this->BigClawRight.y = this->y;
	
	this->BigClawLeft.Follow(200,300);
	LeftArm[3].Follow(BigClawLeft);
	LeftArm[3].calculateEndpoint();
	for (int i = 2; i >=0 ;  i--)
	{
		LeftArm[i].Follow(LeftArm[i + 1]);
		this->LeftArm[i].calculateEndpoint();
	}
	

	this->LeftArm[0].setStartPoint(Vec2(x, y));
	this->LeftArm[0].calculateEndpoint();
	for (int i = 1; i < 4; i++)
	{
		this->LeftArm[i].setStartPoint(this->LeftArm[i - 1].getEndpoint());
		this->LeftArm[i].calculateEndpoint();

	}
	this->BigClawLeft.setStartPoint(LeftArm[3].getEndpoint());
	this->BigClawLeft.calculateEndpoint();
	//DebugOut(L"Claw end x, %d ", this->BigClawLeft.getEndpoint().x);
	//DebugOut(L"Claw end y, %d ", this->BigClawLeft.getEndpoint().y);
	

}


void CBoss::Render()
{
	int ani;
	
		ani = Boss_ANI_WALKING_LEFT;

		animation_set->at(0)->Render(x,y);

	/*if (state == Boss_STATE_DIE) {
		return;
	}*/

	

	//RenderBoundingBox();
	BigClawLeft.Render();
	BigClawRight.Render();
	for (int i = 0; i < 4; i++)
	{
		LeftArm[i].Render();
	}
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

void CBoss::Init()
{
	for (int i = 0; i < 4; i++)
	{
		this->LeftArm[i].setStartPoint(Vec2(120 + i * 15, 120 + i * 15));
		this->LeftArm[i].calculateEndpoint();
	}
}

void CBoss::BossClawSection::setStartPoint(Vec2 sp)
{
	this->startPoint = sp;
}

void CBoss::BossClawSection::calculateEndpoint()
{
	float delx = SectionLength * cos(Angle);
	float dely = SectionLength * sin(Angle);
	
	
	

	this->endPoint = Vec2(delx, dely) + startPoint;
	this->x = this->endPoint.x;
	this->y = this->endPoint.y;

}

void CBoss::BossClawSection::Follow(float x, float y)
{
	
	Vec2 Target = Vec2(x, y);
	DebugOut(L"Target x %f ", x);
	DebugOut(L"Target y %f ", y);
	Vec2 Direction = Target - this->startPoint;
	DebugOut(L"Direction x %f, ", Direction.x);
	DebugOut(L"Direction  y %f, ", Direction.y);
	Angle = atan2(Direction.y, Direction.x);
	DebugOut(L"Angle %f", Angle);
	this->startPoint = Target + Direction.GetNormalized() * SectionLength * -1;
	

}

void CBoss::BossClawSection::Follow(BossClawSection& target)
{
	Follow(target.startPoint.x, target.startPoint.y);
	
}

CBoss::BossClawSection::BossClawSection(int anisetid)
{
	this->x = 120;
	this->y = 240;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(anisetid));
	this->Angle = 0;
	this->setStartPoint(Vec2(120, 240));
	this->calculateEndpoint();
	
	
}
CBoss::BossClawSection::BossClawSection()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(17));
	this->Angle = 0;
	this->startPoint = Vec2(200, 30);
	this->calculateEndpoint();
	
}

CBoss::BossClawSection::BossClawSection(Vec2 start, float angle)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(17));
	this->Angle = angle;
	this->startPoint = start;
	this->calculateEndpoint();

}

void CBoss::BossClawSection::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 15;
	bottom = y + 15;
}

void CBoss::BossClawSection::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CBoss::BossClawSection::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();

}


