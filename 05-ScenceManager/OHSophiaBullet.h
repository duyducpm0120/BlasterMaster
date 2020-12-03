#pragma once
#include "Bullet.h"

#define OHSOPHIABULLET_SPEED 0.5f


#define OHSOPHIABULLET_BBOX_WIDTH 8
#define OHSOPHIABULLET_BBOX_HEIGHT 8

#define OHSOPHIABULLET_FLYING_SPACE 100

#define OHSOPHIABULLET_STATE_FLYING_LEFT 100
#define OHSOPHIABULLET_STATE_FLYING_RIGHT 200
#define	OHSOPHIABULLET_STATE_FLYING_UP	300
#define OHSOPHIABULLET_STATE_FLYING_DOWN 400



#define OHSOPHIABULLET_ANI_LV1	0
#define OHSOPHIABULLET_ANI_LV2	1


#define OHSOPHIABULLET_TYPE_STRAIGHT	0
#define OHSOPHIABULLET_TYPE_ROUNDED		1
#define OHSOPHIABULLET_TYPE_SIN			2

struct Point
{
	float x;
	float y;

};

class COHSophiaBullet : public CBullet
{
	int type;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	Point point1, point2, point3, point4;
	
public:
	COHSophiaBullet(int level, int state, int type);
	void SetAnchorPoint();
	void SetState(int state);
};

