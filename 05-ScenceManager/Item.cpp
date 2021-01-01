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
	case ITEM_TYPE_THUNDER:
		ani = ITEM_ANI_THUNDER;
		break;
	case ITEM_TYPE_ENABLE_ROCKET:
		ani = ITEM_ANI_ENABLE_ROCKET;
		break;
	case ITEM_TYPE_POWER:
		ani = ITEM_ANI_POWER;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	count++;
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += ITEM_GRAVITY;
}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ITEM_BBOX_WIDTH;
	b = y + ITEM_BBOX_HEIGHT;
}
