#include "Enemy_Flower.h"

void CEnemy_Flower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLOWER_BBOX_WIDTH;

	bottom = y + FLOWER_BBOX_HEIGHT;
}

void CEnemy_Flower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CEnemy_Flower::Render()
{
	int ani = FLOWER_ANI_LIVE;
	animations[ani]->Render(x, y,1);

}
