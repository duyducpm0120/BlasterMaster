#pragma once
#include "GameObject.h"
#include "Player.h"

#pragma region define

#define	SOPHIA_START_HEALTH	7
#define SOPHIA_START_DAMAGE	1

#define SOPHIA_WALKING_SPEED		0.1f 
//0.1f
#define SOPHIA_JUMP_SPEED_Y		0.35f
#define SOPHIA_JUMP_DEFLECT_SPEED 0.35f
#define SOPHIA_GRAVITY			0.001f
#define SOPHIA_DIE_DEFLECT_SPEED	 0.35f

#define	SOPHIA_STATE_IDLE_LEFT		0
#define SOPHIA_STATE_IDLE_RIGHT		100
#define SOPHIA_STATE_WALKING_LEFT		200
#define SOPHIA_STATE_WALKING_RIGHT	300
#define	SOPHIA_STATE_CLIMBING_LADDER	400

#define SOPHIA_STATE_DIE				800








#define	SOPHIA_ANI_IDLE_LEFT			0
#define SOPHIA_ANI_IDLE_RIGHT			1
#define SOPHIA_ANI_WALKING_LEFT		2
#define SOPHIA_ANI_WALKING_RIGHT		3
#define SOPHIA_ANI_CLIMBING_LADDER	4

#define SOPHIA_ANI_DIE				5





#define SOPHIA_BBOX_WIDTH	8
#define	SOPHIA_BBOX_HEIGHT	16


#define SOPHIA_UNTOUCHABLE_TIME 3000



#pragma endregion




class CSophia : public CPlayer
{
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of SOPHIA at scene
	float start_y;
	int SOPHIA_width;
	int SOPHIA_height;
	int bulletLevel;
	bool isJumping;
public:
	CSophia(float x = 0.0f, float y = 0.0f);
	int GetHealth() { return this->health; }
	int GetDamage() { return this->damage; }
	int GetBulletLevel() { return bulletLevel; };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void SetDimension(int width, int height);
	void GetDimension(int& width, int& height);
	void Reset();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
