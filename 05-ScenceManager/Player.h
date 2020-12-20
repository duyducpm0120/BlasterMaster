#pragma once
#include "GameObject.h"

#pragma region define

#define	PLAYER_START_HEALTH	7
#define PLAYER_START_DAMAGE	1

#define PLAYER_WALKING_SPEED		0.1f 
//0.1f
#define PLAYER_JUMP_SPEED_Y		0.35f
#define PLAYER_JUMP_DEFLECT_SPEED 0.35f
#define PLAYER_GRAVITY			0.001f
#define PLAYER_DIE_DEFLECT_SPEED	 0.35f

#define	PLAYER_STATE_IDLE_LEFT		0
#define PLAYER_STATE_IDLE_RIGHT		100
#define PLAYER_STATE_WALKING_LEFT		200
#define PLAYER_STATE_WALKING_RIGHT	300
#define PLAYER_STATE_JUMP_IDLE_LEFT	400
#define PLAYER_STATE_JUMP_IDLE_RIGHT	500
#define PLAYER_STATE_JUMP_LEFT		600
#define PLAYER_STATE_JUMP_RIGHT		700

#define PLAYER_STATE_DIE				800

#define PLAYER_STATE_UPING_GUN_LEFT		900
#define PLAYER_STATE_UPING_GUN_RIGHT	1000
#define	PLAYER_STATE_UP_GUN_WALKING_LEFT	1100
#define PLAYER_STATE_UP_GUN_WALKING_RIGHT	1200
#define PLAYER_STATE_DOWN_GUN_IDLE_LEFT	1300
#define PLAYER_STATE_DOWN_GUN_IDLE_RIGHT	1400
#define PLAYER_STATE_UP_GUN_LEFT	1500	
#define	PLAYER_STATE_UP_GUN_RIGHT	1600






#define	PLAYER_ANI_IDLE_LEFT			0
#define PLAYER_ANI_IDLE_RIGHT			1
#define PLAYER_ANI_WALKING_LEFT		2
#define PLAYER_ANI_WALKING_RIGHT		3
#define PLAYER_ANI_JUMP_IDLE_LEFT		4
#define PLAYER_ANI_JUMP_IDLE_RIGHT	5
#define PLAYER_ANI_JUMP_LEFT			6
#define PLAYER_ANI_JUMP_RIGHT			7

#define PLAYER_ANI_DIE				8

#define PLAYER_ANI_UPING_GUN_LEFT	9
#define PLAYER_ANI_UPING_GUN_RIGHT	10
#define	PLAYER_ANI_UP_GUN_WALKING_LEFT	11
#define PLAYER_ANI_UP_GUN_WALKING_RIGHT	12
#define PLAYER_STATE_DOWN_GUN_IDLE_LEFT	13
#define PLAYER_STATE_DOWN_GUN_IDLE_RIGHT	14
#define PLAYER_ANI_UP_GUN_LEFT	15
#define	PLAYER_ANI_UP_GUN_RIGHT	16





#define PLAYER_NORMAL_WIDTH	26
#define	PLAYER_NORMAL_HEIGHT	18	
#define PLAYER_UP_GUN_WIDHT	26
#define PLAYER_UP_GUN_HEIGHT	34

#define PLAYER_UNTOUCHABLE_TIME 3000



#pragma endregion




class CPlayer : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of PLAYER at scene
	float start_y;
	int PLAYER_width;
	int PLAYER_height;
	bool isJumping;
	bool enableRocket;
	int bulletLevel;
protected:
	int untouchableTime;
public:
	virtual bool GetEnableRocket() { return  enableRocket; };
	CPlayer(float x = 0.0f, float y = 0.0f);
	virtual int GetHealth() { return this->health; }
	virtual int GetDamage() { return this->damage; }
	virtual int GetBulletLevel() { return bulletLevel; };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	virtual bool IsJumping() { return isJumping; };
	virtual void SetDimension(int width, int height);
	virtual void GetDimension(int& width, int& height);
	void Reset();

	void SetUntouchableTime(int time) { untouchableTime = time; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
