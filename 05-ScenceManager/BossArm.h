#pragma once
#include "GameObject.h"
#include "Boss.h"

#define TYPE_LEFT_CLAW 100
#define TYPE_RIGHT_CLAW 200
class BossArm: public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

private:

	CBoss* Boss;
	int type;
public:
	BossArm(CBoss* boss, int _type);

	
};

