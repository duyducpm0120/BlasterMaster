#pragma once
#include "GameObject.h"
#include	"Player.h"

#pragma region define

#define	OHSOPHIA_START_HEALTH	7
#define OHSOPHIA_START_DAMAGE	5

#define OHSOPHIA_WALKING_SPEED		0.1f 
//0.1f
#define OHSOPHIA_GRAVITY			0.001f

#define	OHSOPHIA_STATE_IDLE_LEFT		0
#define OHSOPHIA_STATE_IDLE_RIGHT		100
#define OHSOPHIA_STATE_IDLE_UP			200
#define	OHSOPHIA_STATE_IDLE_DOWN		300
#define OHSOPHIA_STATE_WALKING_LEFT		400
#define OHSOPHIA_STATE_WALKING_RIGHT	500
#define OHSOPHIA_STATE_WALKING_UP		600
#define	OHSOPHIA_STATE_WALKING_DOWN		700


#define OHSOPHIA_STATE_DIE				800







#define	OHSOPHIA_ANI_IDLE_LEFT		0
#define OHSOPHIA_ANI_IDLE_RIGHT		1
#define OHSOPHIA_ANI_IDLE_UP			2
#define	OHSOPHIA_ANI_IDLE_DOWN		3
#define OHSOPHIA_ANI_WALKING_LEFT		4
#define OHSOPHIA_ANI_WALKING_RIGHT	5
#define OHSOPHIA_ANI_WALKING_UP		6
#define	OHSOPHIA_ANI_WALKING_DOWN		7

#define OHSOPHIA_WIDTH	24
#define OHSOPHIA_HEIGHT	32

#define OHSOPHIA_UNTOUCHABLE_TIME 200


#pragma endregion




class COHSophia : public CPlayer
{
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of Tank at scene
	float start_y;
	int OHSophia_width;
	int OHSophia_height;
	bool isJumping;
	bool enableRocket;
	int bulletLevel;
	int untouchableTime;
public:
	bool GetEnableRocket() { return  enableRocket; };
	COHSophia(float x = 0.0f, float y = 0.0f);
	int GetHealth() { return this->health; }
	int GetDamage() { return this->damage; }
	int GetBulletLevel() { return bulletLevel; };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	bool IsJumping() { return isJumping; };
	void SetDimension(int width, int height);
	void GetDimension(int& width, int& height);
	void Reset();
	void HandleUntouchableTime();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
