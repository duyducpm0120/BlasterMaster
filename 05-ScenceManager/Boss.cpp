#include "Boss.h"
#include "Utils.h"
#include "Game.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
#include "Vec2.h"
CBoss::CBoss() :
	BigClawLeft(18),
	BigClawRight(19)
	
{
	
	damage = 1;
	health = 30;
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

	if (health <= 0) {
		this->visible = false;
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetEndingCount();
	}
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
	if (vy < 0 && y < (startY - 25)) {
		y = startY - 25;
		vy = Boss_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -Boss_WALKING_SPEED;
	}
	
	updateTarget1();
	updateTarget2();
	this->BigClawLeft.Follow(Target1);
	

	LeftArm[3].Follow(BigClawLeft);
	LeftArm[3].calculateEndpoint();
	for (int i = 2; i >=0 ;  i--)
	{
		LeftArm[i].Follow(LeftArm[i + 1]);
		this->LeftArm[i].calculateEndpoint();
	}
	
	//Rebasing the arm to the boss
	this->LeftArm[0].setStartPoint(Vec2(x, y+20));
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
	
	this->BigClawRight.Follow(Target2);
	RightArm[3].Follow(BigClawRight);
	RightArm[3].calculateEndpoint();
	for (int i = 2; i >= 0; i--)
	{
		RightArm[i].Follow(RightArm[i + 1]);
		this->RightArm[i].calculateEndpoint();
	}

	//Rebasing the arm to the boss
	this->RightArm[0].setStartPoint(Vec2(x+40, y+20));
	this->RightArm[0].calculateEndpoint();
	for (int i = 1; i < 4; i++)
	{
		this->RightArm[i].setStartPoint(this->RightArm[i - 1].getEndpoint());
		this->RightArm[i].calculateEndpoint();

	}
	this->BigClawRight.setStartPoint(RightArm[3].getEndpoint());
	this->BigClawRight.calculateEndpoint();
	counter1 += dt;
	counter2 += dt;
	counter3 += dt;
	counter4 += dt;
	if(counter1>=3000)
	{
		Shoot();
		counter1 = 0;
	}
	if (counter2 >= 3000)
	{
		Shoot();
		counter2 = 0;
	}
	if (counter3 >= 3000)
	{
		Shoot();
		counter3 = 0;
	}
	if (counter4 >= 3000)
	{
		Shoot();
		counter4 = 0;
	}

}


void CBoss::Render()
{
	

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
	for (int i = 0; i < 4; i++)
	{
		RightArm[i].Render();
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

void CBoss::getLeftClawPos(float& x, float& y)
{
	x = this->BigClawLeft.x;
	y = this->BigClawLeft.y;
}

void CBoss::getRightClawPos(float& x, float& y)
{
	x = this->BigClawRight.x;
	y = this->BigClawRight.y;
}

void CBoss::updateTarget1()
{
	
	float left = x - 60;
	float right = x + 30;
	float bottom = y + 90;
	float top = y - 60;
	/*DebugOut(L"Left %f \n", left);
	DebugOut(L"Right %f \n", right);
	DebugOut(L"top %f \n", top);
	DebugOut(L"bottom %f \n", bottom);*/
	Target1 += Speed;
	if (Target1.x >= right)
	{
		Target1.x = right;
		Speed = Vec2(Speed.x * -1, Speed.y);
		
	}

	if (Target1.x <= left)
	{
		Target1.x = left;
		Speed = Speed = Vec2(Speed.x * -1, Speed.y);
		/*DebugOut(L"Speed %f \n", Speed.x);
		DebugOut(L"Speed %f \n", Speed.y);*/
	}
	
	if (Target1.y >= bottom)
	{
		Target1.y = bottom;

		Speed = Vec2(Speed.x , Speed.y*-1);
		/*DebugOut(L"Speed %f \n", Speed.x);
		DebugOut(L"Speed %f \n", Speed.y);*/
	}
	if (Target1.y <=top )
	{
		Target1.y = top;

		Speed = Vec2(Speed.x, Speed.y * -1);
		/*DebugOut(L"Speed %f \n", Speed.x);
		DebugOut(L"Speed %f \n", Speed.y);*/
	}

}

void CBoss::updateTarget2()
{
	float left = x ;
	float right = x + 180;
	float bottom = y + 90;
	float top = y - 60;
	/*DebugOut(L"Left %f \n", left);
	DebugOut(L"Right %f \n", right);
	DebugOut(L"top %f \n", top);
	DebugOut(L"bottom %f \n", bottom);*/
	Target2 += Speed2;
	if (Target2.x >= right)
	{
		Target2.x = right;
		Speed2 = Vec2(Speed2.x * -1, Speed2.y);

	}

	if (Target2.x <= left)
	{
		Target2.x = left;
		Speed2 = Speed2 = Vec2(Speed2.x * -1, Speed2.y);
		/*DebugOut(L"Speed2 %f \n", Speed2.x);
		DebugOut(L"Speed2 %f \n", Speed2.y);*/
	}

	if (Target2.y >= bottom)
	{
		Target2.y = bottom;

		Speed2 = Vec2(Speed2.x, Speed2.y * -1);
	/*	DebugOut(L"Speed2 %f \n", Speed2.x);
		DebugOut(L"Speed2 %f \n", Speed2.y);*/
	}
	if (Target2.y <= top)
	{
		Target2.y = top;

		Speed2 = Vec2(Speed2.x, Speed2.y * -1);
		/*DebugOut(L"Speed2 %f \n", Speed2.x);
		DebugOut(L"Speed2 %f \n", Speed2.y);*/
	}
}

void CBoss::Init()
{
	for (int i = 0; i < 4; i++)
	{
		this->LeftArm[i].setStartPoint(Vec2(x+15, 120 + i * 15));
		this->LeftArm[i].calculateEndpoint();
		this->RightArm[i].setStartPoint(Vec2(x+60 + i * 15, 120 + i * 15));
		this->RightArm[i].calculateEndpoint();

	}
	
}

void CBoss::Shoot()
{
	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	CEnemyBullet* bullet1 = new CEnemyBullet(BULLET_STATE_BOSS);
	
	bullet1->SetSpeed(0.0f, BULLET_SPEED/4);
	bullet1->SetPosition(this->x+20, this->y+20);
	bullet1->SetStartPositon(this->x+20, this->y+20);

	objects->push_back(bullet1);
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
	//DebugOut(L"Target x %f ", x);
	//DebugOut(L"Target y %f ", y);
	Vec2 Direction = Target - this->startPoint;
//	DebugOut(L"Direction x %f, ", Direction.x);
//	DebugOut(L"Direction  y %f, ", Direction.y);
	Angle = atan2(Direction.y, Direction.x);
	//DebugOut(L"Angle %f", Angle);
	this->startPoint = Target + Direction.GetNormalized() * SectionLength * -1;
	

}

void CBoss::BossClawSection::Follow(Vec2& target)
{
	Follow(target.x, target.y);
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

}


