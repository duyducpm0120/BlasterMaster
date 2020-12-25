#include "Flyingclaw.h"
#include "Game.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
#include "Vec2.h"
void CFlyingclaw::Shoot()
{
	this->SetState(FLYINGCLAW_STATE_SHOOTING);
	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	float px, py;
	((CPlayScene*)scence)->GetPlayer()->GetPosition(px, py);
	CEnemyBullet* bullet1 = new CEnemyBullet(BULLET_STATE_UNDEF);
	Vec2 speed = Vec2(px - x, py - y);
	speed = speed.Normalize() * (BULLET_SPEED / 2);
	bullet1->SetSpeed(speed.x, speed.y);
	bullet1->SetPosition(this->x, this->y);
	bullet1->SetStartPositon(this->x, this->y);

	objects->push_back(bullet1);
}
void CFlyingclaw::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLYINGCLAW_BBOX_WIDTH;
	bottom = y + FLYINGCLAW_BBOX_HEIGHT;
}

void CFlyingclaw::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure FLYINGCLAW can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;


	if (vx < 0 && x < (startX - 60)) {
		x = startX - 60;
		SetState(FLYINGCLAW_STATE_WALKING_RIGHT);
		nx = 1;
		vx = FLYINGCLAW_WALKING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -FLYINGCLAW_WALKING_SPEED;
		SetState(FLYINGCLAW_STATE_WALKING_LEFT);
	}
	if (vy < 0 && y < (startY - 40)) {
		y = startY - 40;
		vy = FLYINGCLAW_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -FLYINGCLAW_WALKING_SPEED;
	}

	counter += dt;
	if (counter >= 1000)
	{
		Shoot();
		counter = 0;

	}
}

void CFlyingclaw::Render()
{

	int ani;
	if (state == FLYINGCLAW_STATE_SHOOTING)	ani = FLYINGCLAW_ANI_SHOOTING;
	else ani = FLYINGCLAW_ANI_WALKING;
	animation_set->at(ani)->Render(x, y);
}

CFlyingclaw::CFlyingclaw()
{
	isEnemy = true;
	damage = 1;
	health = 3;
	nx = -1;
	SetState(FLYINGCLAW_STATE_WALKING_LEFT);
	vx = -FLYINGCLAW_WALKING_SPEED;
	vy = FLYINGCLAW_WALKING_SPEED;
}

void CFlyingclaw::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	case FLYINGCLAW_STATE_WALKING_LEFT:

		break;
	case FLYINGCLAW_STATE_WALKING_RIGHT:
		nx = 1;
		vx = FLYINGCLAW_WALKING_SPEED;
	}
}

void CFlyingclaw::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}