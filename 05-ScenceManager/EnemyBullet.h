#pragma once
#include "Bullet.h"
#define BULLET_STATE_FLYING_DOWN 400
#define BULLET_STATE_UNDEF 500
#define BULLET_STATE_BOSS 500
#define BULLET_STATE_ROLLING 600
class CEnemyBullet:public CBullet
{
	float CaclDistance();
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CEnemyBullet();
	CEnemyBullet(int state);
	
};

