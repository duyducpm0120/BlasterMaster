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
	public:
		BossClawSection(int anisetid);
		BossClawSection();
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
		virtual void Render();
		void Follow();
		void setTarget(CGameObject* target);
	};
public:

	CBoss();
	CGameObject* LeftClawTargetRandomMovingObject;
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
private: 
	void RenderArms();
	BossClawSection BigClawLeft;
	BossClawSection BigClawRight;
	BossClawSection LeftArm[5];
	
};