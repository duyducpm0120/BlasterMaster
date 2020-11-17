#pragma once
#include "GameObject.h"

#define DESTROYED_TYPE_BULLET	1
#define DESTROYED_TYPE_OBJECT	2
#define	 DESTROYED_TYPE_TANK	3

#define DESTROYED_BULLET_BBOX_WIDTH  16
#define DESTROYED_BULLET_BBOX_HEIGHT 16

#define DESTROYED_OBJECT_BBOX_WIDTH	24
#define	DESTROYED_OBJECT_BBOX_HEIGHT	24

#define	DESTROYED_TANK_BBOX_WIDTH	64
#define	DESTROYED_TANK_BBOX_HEIGHT	48

#define	DESTROYED_ANI_BULLET	0
#define DESTROYED_ANI_OBJECT	1
#define	DESTROYED_ANI_TANK	2


class CDestroyed : public CGameObject
{
private:
	int type; // 1 = bullet  2 = object 3 = tank
	int width;
	int height;
	int count;
public:
	CDestroyed(int type);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

