#pragma once
#include "GameObject.h"

#pragma region define

#define TANK_WALKING_SPEED		0.15f 
//0.1f
#define TANK_JUMP_SPEED_Y		0.5f
#define TANK_JUMP_DEFLECT_SPEED 0.2f
#define TANK_GRAVITY			0.002f
#define TANK_DIE_DEFLECT_SPEED	 0.5f

#define	TANK_STATE_IDLE_LEFT		0
#define TANK_STATE_IDLE_RIGHT		100
#define TANK_STATE_WALKING_LEFT		200
#define TANK_STATE_WALKING_RIGHT	300
#define TANK_STATE_JUMP_IDLE_LEFT	400
#define TANK_STATE_JUMP_IDLE_RIGHT	500
#define TANK_STATE_JUMP_LEFT		600
#define TANK_STATE_JUMP_RIGHT		700

#define TANK_STATE_DIE				800

#define	TANK_ANI_IDLE_LEFT			0
#define TANK_ANI_IDLE_RIGHT			1
#define TANK_ANI_WALKING_LEFT		2
#define TANK_ANI_WALKING_RIGHT		3
#define TANK_ANI_JUMP_IDLE_LEFT		4
#define TANK_ANI_JUMP_IDLE_RIGHT	5
#define TANK_ANI_JUMP_LEFT			6
#define TANK_ANI_JUMP_RIGHT			7

#define TANK_BBOX_WIDTH  26
#define TANK_BBOX_HEIGHT 18


#define TANK_UNTOUCHABLE_TIME 3000



#pragma endregion




class CTank : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of Tank at scene
	float start_y;
public:
	CTank(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
