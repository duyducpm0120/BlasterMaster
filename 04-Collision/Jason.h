#pragma once
#include "GameObject.h"

#pragma region define

#define JASON_WALKING_SPEED		0.1f 
//0.1f
#define JASON_JUMP_SPEED_Y		0.5f
#define JASON_JUMP_DEFLECT_SPEED 0.2f
#define JASON_GRAVITY			0.002f
#define JASON_DIE_DEFLECT_SPEED	 0.5f

#define	JASON_STATE_IDLE_RIGHT		-100
#define JASON_STATE_IDLE_LEFT		0
#define JASON_STATE_WALKING_RIGHT	100
#define JASON_STATE_WALKING_LEFT	200
#define JASON_STATE_JUMP			300
#define JASON_STATE_DIE				400

#define JASON_ANI_IDLE_RIGHT		0
#define JASON_ANI_IDLE_LEFT			1

#define JASON_ANI_WALKING_RIGHT		2
#define JASON_ANI_WALKING_LEFT		3


#define JASON_ANI_DIE				4


#define JASON_BBOX_WIDTH  24
#define JASON_BBOX_HEIGHT 32


#define JASON_UNTOUCHABLE_TIME 3000



#pragma endregion




class CJason : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;
public:
	CJason() : CGameObject()
	{
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

