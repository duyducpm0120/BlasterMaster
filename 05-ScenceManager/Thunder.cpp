#include "Thunder.h"

CThunder::CThunder():CGameObject()
{
	count = 0;
	ani_set = CAnimationSets::GetInstance()->Get(71);
	damage = 10;
}

void CThunder::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + THUNDER_HORIZONTAL_BBOX_WIDTH;
	bottom = y + THUNDER_HORIZONTAL_BBOX_HEIGHT;
}

void CThunder::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	count++;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CalcPotentialCollisions(coObjects, coEvents);


	vector<LPCOLLISIONEVENT> coCollisoningEvents;
	CalcColliding(coObjects, coCollisoningEvents);

	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		if ((e->obj)->IsEnemy()) {
			e->obj->TakeDamage(this->damage);				//Destroy every enemy
		}
	}
}

void CThunder::Render()
{
	if (count <= 20) 
		ani_set->at(0)->Render(this->x, this->y, 255);
	else
		visible = false;
}
