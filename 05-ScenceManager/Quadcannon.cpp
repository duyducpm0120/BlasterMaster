#include "QUADCANNON.h"
#include "Game.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
CQuadcannon::CQuadcannon()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(QUADCANNON_STATE_SHOOTING);
}

void CQuadcannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + QUADCANNON_BBOX_WIDTH;

	if (state == QUADCANNON_STATE_DIE)
		bottom = y + QUADCANNON_BBOX_HEIGHT_DIE;
	else
		bottom = y + QUADCANNON_BBOX_HEIGHT;
}

void CQuadcannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure QUADCANNON can interact with the world and to each of them too!
	// 
	shootingcount1 += dt;
	shootingcount2 += dt;
	if (shootingcount1 >= 2000) {
		shootingHorizontally();
		shootingcount1 = 0;
	}
	if (shootingcount2 >= 2000)
	{
		shootingVertically();
		shootingcount2 = 0;
	}


}

void CQuadcannon::Render()
{
	int ani;
	if (state == QUADCANNON_STATE_SHOOTING)
		ani = QUADCANNON_ANI_SHOOTING;
	else
		ani = QUADCANNON_ANI_RECHARGING;


	/*if (state == QUADCANNON_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CQuadcannon::shootingVertically()
{
	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	CEnemyBullet* bullet1 = new CEnemyBullet(BULLET_STATE_FLYING_UP);
	bullet1->SetPosition(this->x +13-5  , this->y);
	bullet1->SetStartPositon(this->x + 13 - 5, this->y);
	objects->push_back(bullet1);
	CEnemyBullet* bullet2 = new CEnemyBullet(BULLET_STATE_FLYING_DOWN);
	bullet2->SetPosition(this->x+13-5, this->y+26);
	bullet2->SetStartPositon(this->x + 13 - 5, this->y + 26);
	objects->push_back(bullet2);

}

void CQuadcannon::shootingHorizontally()
{
	CGame* game = CGame::GetInstance();
	CScene* scence = game->GetCurrentScene();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	CEnemyBullet* bullet1 = new CEnemyBullet(BULLET_STATE_FLYING_LEFT);
	bullet1->SetPosition(this->x , this->y+13-5);
	bullet1->SetStartPositon(this->x, this->y + 13-5);
	objects->push_back(bullet1);
	CEnemyBullet* bullet2 = new CEnemyBullet(BULLET_STATE_FLYING_RIGHT);
	bullet2->SetPosition(this->x + 26, this->y + 13-5);
	bullet2->SetStartPositon(this->x + 26, this->y + 13 - 5);
	objects->push_back(bullet2);
}

void CQuadcannon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUADCANNON_STATE_DIE:
		//y += QUADCANNON_BBOX_HEIGHT - QUADCANNON_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case QUADCANNON_STATE_SHOOTING:
		nx = -1;
		vx = 0;
		break;
	case QUADCANNON_STATE_RECHARGING:
		nx = 1;
		vx = 0;
	}
}
void CQuadcannon::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}

