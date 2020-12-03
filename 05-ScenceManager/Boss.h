#pragma once
#include "GameObject.h"
#include "Vec2.h"
#include <cmath>
#define Boss_WALKING_SPEED 0.07f;

#define Boss_BBOX_WIDTH 60
#define Boss_BBOX_HEIGHT 60
#define Boss_BBOX_HEIGHT_DIE 9

#define Boss_STATE_WALKING_LEFT 100
#define Boss_STATE_WALKING_RIGHT 200
#define Boss_STATE_DIE 300

#define Boss_ANI_WALKING_LEFT 0
#define Boss_ANI_WALKING_RIGHT 1
#define Boss_ANI_DIE	2

class CBoss : public CGameObject
{
	int nx;
	float startX;
	float startY;
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
private: 
	class BossClawSection: public CGameObject{
		Vec2 startPoint;
		Vec2 endPoint;
		float Angle;
	public:
		void setStartPoint(Vec2 sp);
		Vec2 getEndpoint() { return endPoint; };
		float getAngle() { return this->Angle; };
		static float constexpr SectionLength = 21;
		void calculateEndpoint();
		void Follow(float x, float y);
		void Follow(BossClawSection& target); // Follow the start point of target
		BossClawSection(int anisetid);
		BossClawSection();
		BossClawSection(Vec2 start, float angle);
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
		virtual void Render();
		
	};
public:

	CBoss();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
	
private: 

	void Init();
	BossClawSection BigClawLeft;
	BossClawSection BigClawRight;
	BossClawSection LeftArm[4];

};