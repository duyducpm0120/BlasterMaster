#pragma once
#include "GameObject.h"
#include "Vec2.h"
#include <cmath>
#define BOSS_WALKING_SPEED 0.05f;

#define BOSS_BBOX_WIDTH 60
#define BOSS_BBOX_HEIGHT 60
#define BOSS_BBOX_HEIGHT_DIE 9

#define BOSS_STATE_WALKING 100
#define BOSS_STATE_INJURED	200
#define BOSS_STATE_DIE 300

#define BOSS_ANI_WALKING 0
#define BOSS_ANI_INJURED	1	
#define BOSS_ANI_DIE	2

#define NUM_OF_FIREWORK_APPEAR	15	
#define	INJURED_STATE_TIME	30

class CBoss : public CGameObject
{
private:
	int nx;
	float startX;
	float startY;
	int die_state_time;
	int injured_state_time;
	static int ani ;
	static int  alpha;
	int numOfFirework = 0;
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
	
public:
	class BossClawSection : public CGameObject {
		Vec2 startPoint;
		Vec2 endPoint;
		float Angle;
	public:
		void setStartPoint(Vec2 sp);
		Vec2 getEndpoint() { return endPoint; };
		float getAngle() { return this->Angle; };
		static float constexpr SectionLength = 20;
		void calculateEndpoint();
		void Follow(float x, float y);
		void Follow(Vec2& target);
		void Follow(BossClawSection& target); // Follow the start point of target
		BossClawSection(int anisetid);
		BossClawSection();
		BossClawSection(Vec2 start, float angle);
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
		virtual void Render();

	};

	CBoss();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
	void getLeftClawPos(float& x, float& y);
	void getRightClawPos(float& x, float& y);
	void HandleInjuredState();
	void HandleDieState();
private: 
	Vec2 Target1 = Vec2(350,150);
	Vec2 Target2 = Vec2(410,150);
	Vec2 Speed = Vec2(2.0f ,2.0f);
	Vec2 Speed2 = Vec2(2.0f ,2.0f);
	void updateTarget1();
	void updateTarget2();
	void Init();
	BossClawSection BigClawLeft;
	BossClawSection BigClawRight;
	BossClawSection LeftArm[4];
	BossClawSection RightArm[4];
	void Shoot();
	int counter = 0;
	int counter1 = 0;
	int counter2 = 300;
	int counter3 = 600;
	int counter4 = 900;
};