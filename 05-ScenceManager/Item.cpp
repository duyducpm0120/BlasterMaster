#include "Item.h"

CItem::CItem(int type)
{
	this->type = type;
}

void CItem::Render()
{
	int ani;
	switch (type) {
	case ITEM_TYPE_HEALTH:
		ani = ITEM_ANI_HEALTH;
		break;
	case ITEM_TYPE_POWER:
		ani = ITEM_ANI_POWER;
		break;
	case ITEM_TYPE_ENABLE_ROCKET:
		ani = ITEM_ANI_ENABLE_ROCKET;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	count++;
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += ITEM_GRAVITY;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rtx = 0;
		float rty = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rtx, rty);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;


		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ITEM_BBOX_WIDTH;
	b = y + ITEM_BBOX_HEIGHT;
}
