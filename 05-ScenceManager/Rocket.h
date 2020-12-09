#pragma once
#include "GameObject.h"
#include "Tank.h"

#define ROCKET_WALKING_SPEED 0.07f;

#define ROCKET_BBOX_WIDTH 15
#define ROCKET_BBOX_HEIGHT 6

#define ROCKET_STATE_WALKING_LEFT 100
#define ROCKET_STATE_WALKING_RIGHT 200
#define ROCKET_STATE_DIE 300

#define ROCKET_ANI_WALKING_LEFT 0
#define ROCKET_ANI_WALKING_RIGHT 1

class CRocket : public CGameObject
{
	int nx;
	float target_x, target_y;
	CGameObject* targetObject;
	vector<LPGAMEOBJECT> *objects;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CRocket();
	void findTarget();
	void SetTargetObjects(vector <LPGAMEOBJECT> *objects);
	virtual void SetState(int state);
	void CatchTargetObject();
	void SetTargetObject(CGameObject *object);
};