#pragma once
#include "GameObject.h"
#include	"Player.h"

#pragma region define

#define	TANK_START_HEALTH	7
#define TANK_START_DAMAGE	5

#define TANK_WALKING_SPEED		0.1f 
//0.1f
#define TANK_JUMP_SPEED_Y		0.35f
#define TANK_JUMP_DEFLECT_SPEED 0.35f
#define TANK_GRAVITY			0.001f
#define TANK_DIE_DEFLECT_SPEED	 0.35f

#define	TANK_STATE_IDLE_LEFT		0
#define TANK_STATE_IDLE_RIGHT		100
#define TANK_STATE_WALKING_LEFT		200
#define TANK_STATE_WALKING_RIGHT	300
#define TANK_STATE_JUMP_IDLE_LEFT	400
#define TANK_STATE_JUMP_IDLE_RIGHT	500
#define TANK_STATE_JUMP_LEFT		600
#define TANK_STATE_JUMP_RIGHT		700

#define TANK_STATE_DIE				800

#define TANK_STATE_UPING_GUN_LEFT		900
#define TANK_STATE_UPING_GUN_RIGHT	1000
#define	TANK_STATE_UP_GUN_WALKING_LEFT	1100
#define TANK_STATE_UP_GUN_WALKING_RIGHT	1200
#define TANK_STATE_DOWN_GUN_IDLE_LEFT	1300
#define TANK_STATE_DOWN_GUN_IDLE_RIGHT	1400
#define TANK_STATE_UP_GUN_LEFT	1500	
#define	TANK_STATE_UP_GUN_RIGHT	1600






#define	TANK_ANI_IDLE_LEFT			0
#define TANK_ANI_IDLE_RIGHT			1
#define TANK_ANI_WALKING_LEFT		2
#define TANK_ANI_WALKING_RIGHT		3
#define TANK_ANI_JUMP_IDLE_LEFT		4
#define TANK_ANI_JUMP_IDLE_RIGHT	5
#define TANK_ANI_JUMP_LEFT			6
#define TANK_ANI_JUMP_RIGHT			7

#define TANK_ANI_DIE				8

#define TANK_ANI_UPING_GUN_LEFT	9
#define TANK_ANI_UPING_GUN_RIGHT	10
#define	TANK_ANI_UP_GUN_WALKING_LEFT	11
#define TANK_ANI_UP_GUN_WALKING_RIGHT	12
#define TANK_STATE_DOWN_GUN_IDLE_LEFT	13
#define TANK_STATE_DOWN_GUN_IDLE_RIGHT	14
#define TANK_ANI_UP_GUN_LEFT	15
#define	TANK_ANI_UP_GUN_RIGHT	16


#define TANK_NORMAL_WIDTH	26
#define	TANK_NORMAL_HEIGHT	18
#define TANK_UP_GUN_WIDHT	26
#define TANK_UP_GUN_HEIGHT	34

#define TANK_UNTOUCHABLE_TIME 200


#pragma endregion




class CTank : public CPlayer
{
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of Tank at scene
	float start_y;
	int tank_width;
	int tank_height;
	bool isJumping;
	bool enableRocket;
	int bulletLevel;
	int untouchableTime;
public:
	bool GetEnableRocket() { return  enableRocket; };
	CTank(float x = 0.0f, float y = 0.0f);
	int GetHealth() { return this->health; }
	int GetDamage() { return this->damage; }
	int GetBulletLevel() { return bulletLevel; };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	bool IsJumping() { return isJumping; };
	void SetDimension(int width, int height);
	void GetDimension(int &width, int &height);
	void Reset();
	void HandleUntouchableTime();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
