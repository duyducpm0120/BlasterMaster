#include "Skull.h"
#include "Game.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
CSkull::CSkull()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED);
}

void CSkull::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + SKULL_BBOX_WIDTH;

	if (state == SKULL_STATE_DIE)
		bottom = y + SKULL_BBOX_HEIGHT_DIE;
	else
		bottom = y + SKULL_BBOX_HEIGHT;
}

void CSkull::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure SKULL can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
	
	if (vx < 0 && x < (startX - 80) ) {
		x = startX - 80; vx = -vx;
		SetState(SKULL_STATE_WALKING_RIGHT_MOUTH_CLOSED);
		
	}

	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		SetState(SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED);
	}

	counter += dt;
	if (counter >= 1000)
	{
		Shoot();
		counter = 0;
		ToggleMouth();

	}
}

void CSkull::Render()
{
	int ani;
	if (state == SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED)
		ani = SKULL_ANI_WALKING_LEFT_CLOSED;
	else if (state == SKULL_ANI_WALKING_LEFT_OPENED)
		ani = SKULL_ANI_WALKING_LEFT_OPENED;
	else if (state == SKULL_ANI_WALKING_RIGHT_OPENED)
		ani = SKULL_ANI_WALKING_RIGHT_OPENED;
	else
		ani = SKULL_ANI_WALKING_RIGHT_CLOSED;


	/*if (state == SKULL_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CSkull::Shoot()
{

	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	CEnemyBullet* bullet1 = new CEnemyBullet(BULLET_STATE_ROLLING);
	bullet1->SetSpeed(0, BULLET_SPEED/4);
	bullet1->SetPosition(this->x+15, this->y+15);
	bullet1->SetStartPositon(this->x, this->y);

	objects->push_back(bullet1);
}

void CSkull::ToggleMouth()
{
	if (state == SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED) SetState(SKULL_STATE_WALKING_LEFT_MOUTH_OPENED);
	else if (state == SKULL_STATE_WALKING_LEFT_MOUTH_OPENED) SetState(SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED);
	else if (state == SKULL_STATE_WALKING_RIGHT_MOUTH_OPENED) SetState(SKULL_STATE_WALKING_RIGHT_MOUTH_CLOSED);
	else if (state == SKULL_STATE_WALKING_RIGHT_MOUTH_CLOSED) SetState(SKULL_STATE_WALKING_RIGHT_MOUTH_CLOSED);
}


void CSkull::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKULL_STATE_DIE:
		//y += SKULL_BBOX_HEIGHT - SKULL_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case SKULL_STATE_WALKING_LEFT_MOUTH_CLOSED:
		nx = -1;
		vx = -SKULL_WALKING_SPEED;
		break;
	case SKULL_STATE_WALKING_RIGHT_MOUTH_CLOSED:
		nx = 1;
		vx = SKULL_WALKING_SPEED;
		break;
	case SKULL_STATE_WALKING_LEFT_MOUTH_OPENED:
		nx = -1;
		vx = 0;
		break;
	case SKULL_STATE_WALKING_RIGHT_MOUTH_OPENED:
		nx = -1;
		vx = 0;
		break;
	}
	
}

void CSkull::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}