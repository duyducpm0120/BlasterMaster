#include "Eye.h"
#include "Game.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
#include "Vec2.h"
void CEye::Shoot()
{
	this->SetState(EYE_STATE_SHOOTING);
	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	float px, py;
	((CPlayScene*)scence)->GetPlayer()->GetPosition(px, py);
	CEnemyBullet* bullet1 = new CEnemyBullet(BULLET_STATE_UNDEF);
	Vec2 speed = Vec2(px - x, py - y);
	speed = speed.Normalize() * BULLET_SPEED;
	bullet1->SetSpeed(speed.x, speed.y);
	bullet1->SetPosition(this->x, this->y);
	bullet1->SetStartPositon(this->x, this->y);

	objects->push_back(bullet1);
}
void CEye::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYE_BBOX_WIDTH;
	bottom = y + EYE_BBOX_HEIGHT;
}

void CEye::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure EYE can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;


	if (vx < 0 && x < (startX - 100)) {
		x = startX - 100;
		SetState(EYE_STATE_WALKING_RIGHT);
		nx = 1;
		vx = EYE_WALKING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -EYE_WALKING_SPEED;
		SetState(EYE_STATE_WALKING_LEFT);
	}
	if (vy < 0 && y < (startY - 50)) {
		y = startY - 50;
		vy = EYE_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -EYE_WALKING_SPEED;
	}

	counter += dt;
	if (counter >= 1000)
	{
		Shoot();
		counter = 0;

	}
}

void CEye::Render()
{

	int ani;
	if (state == EYE_STATE_SHOOTING)	ani = EYE_ANI_SHOOTING;
	else ani = EYE_ANI_WALKING;
	animation_set->at(ani)->Render(x, y);
}

CEye::CEye()
{
	isEnemy = true;
	damage = 1;
	health = 3;
	nx = -1;
	SetState(EYE_STATE_WALKING_LEFT);
	vx = -EYE_WALKING_SPEED;
	vy = EYE_WALKING_SPEED;
}

void CEye::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	
	case EYE_STATE_WALKING_LEFT:

		break;
	case EYE_STATE_WALKING_RIGHT:
		nx = 1;
		vx = EYE_WALKING_SPEED;
	}
}

void CEye::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}