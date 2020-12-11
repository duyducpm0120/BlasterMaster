#include "Robot.h"
#include "Game.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
CRobot::CRobot()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(ROBOT_STATE_WALKING_LEFT);
}

void CRobot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + ROBOT_BBOX_WIDTH;

	if (state == ROBOT_STATE_DIE)
		bottom = y + ROBOT_BBOX_HEIGHT_DIE;
	else
		bottom = y + ROBOT_BBOX_HEIGHT;
}

void CRobot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure ROBOT can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < (startX - 80)) {
		x = startX - 80; vx = -vx;
		SetState(ROBOT_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		SetState(ROBOT_STATE_WALKING_LEFT);
	}
	shootCounter += dt;
	if(shootCounter>=1000)
	{ 
		Shoot();
		shootCounter = 0;
	}
	
}

void CRobot::Render()
{
	int ani;
	if (state == ROBOT_STATE_WALKING_LEFT)
		ani = ROBOT_ANI_WALKING_LEFT;
	else
		ani = ROBOT_ANI_WALKING_RIGHT;


	/*if (state == ROBOT_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CRobot::Shoot()
{
	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	CEnemyBullet* bullet;
	if (this->state == ROBOT_STATE_WALKING_LEFT) bullet = new CEnemyBullet(BULLET_STATE_FLYING_LEFT);
	else bullet = new CEnemyBullet(BULLET_STATE_FLYING_RIGHT);
	bullet->SetPosition(this->x + 13 - 5, this->y+5);
	bullet->SetStartPositon(this->x + 13 - 5, this->y+5);
	objects->push_back(bullet);
}

void CRobot::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ROBOT_STATE_DIE:
		//y += ROBOT_BBOX_HEIGHT - ROBOT_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case ROBOT_STATE_WALKING_LEFT:
		nx = -1;
		vx = -ROBOT_WALKING_SPEED;
		break;
	case ROBOT_STATE_WALKING_RIGHT:
		nx = 1;
		vx = ROBOT_WALKING_SPEED;
	}
}

void CRobot::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
