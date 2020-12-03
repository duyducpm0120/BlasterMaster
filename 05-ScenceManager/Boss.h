#pragma once
#include "GameObject.h"

#define Boss_WALKING_SPEED 0.03f;

#define Boss_BBOX_WIDTH 60
#define Boss_BBOX_HEIGHT 60
#define Boss_BBOX_HEIGHT_DIE 9

#define Boss_STATE_WALKING_LEFT 100
#define Boss_STATE_WALKING_RIGHT 200
#define Boss_STATE_DIE 300

#define Boss_ANI_WALKING_LEFT 0
#define Boss_ANI_WALKING_RIGHT 1
#define Boss_ANI_DIE	

#define ARM_SPEED 0.05f;

class CBoss : public CGameObject
{
	int nx;
	float startX;
	float startY;
	int count;

	
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void init();
	
private: 
	class BossClawSection: public CGameObject{
	private:
		CGameObject* target;
		int nI;
		float angle;
		float startX;
		float startY;
		int MovingSpaceX;
		int MovingSpaceY;
		CBoss* baseBoss;
		float bossLastMoveX;
		float bossLastMoveY;

	public:
		BossClawSection(int anisetid);
		BossClawSection();
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
		virtual void Render();
		void Follow();
		void setTarget(CGameObject* target);
		void setMovingSpace(int x, int y);
		void setStartPosition(float x, float y) { this->startX = x; this->startY = y; };
		void setnI(int i) { this->nI = i; };
		void setBaseBoss(CBoss* boss) { this->baseBoss = boss; };
		void FollowBoss();
		void SetBossLastMove(float x, float y) {
			bossLastMoveX = x;

			bossLastMoveY = y;
		};
	};
public:

	CBoss();
	CGameObject* LeftClawTargetRandomMovingObject1;
	CGameObject* LeftClawTargetRandomMovingObject2;
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
private: 
	void RenderArms();
	BossClawSection BigClawLeft;
	BossClawSection BigClawRight;
	BossClawSection LeftArm[5];
	
};