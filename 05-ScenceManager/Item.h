#pragma once
#include "GameObject.h"

#define ITEM_TYPE_HEALTH	0
#define ITEM_TYPE_THUNDER		1
#define	ITEM_TYPE_ENABLE_ROCKET	2
#define ITEM_TYPE_POWER	3

#define ITEM_BBOX_WIDTH  20
#define ITEM_BBOX_HEIGHT	20


#define ITEM_ANI_HEALTH	0
#define ITEM_ANI_THUNDER		1
#define	ITEM_ANI_ENABLE_ROCKET	2
#define ITEM_ANI_POWER	3

#define ITEM_GRAVITY	0.0f

class CItem : public CGameObject
{
private:
	int type; 
	int width;
	int height;
	int count;
public:
	CItem(int type);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetType() { return type; };
};

