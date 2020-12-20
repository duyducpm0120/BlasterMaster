#include "Choose.h"

CChoose::CChoose()
{
	isEnemy = false;
}

void CChoose::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CChoose::Update()
{
}

void CChoose::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CHOOSE_BBOX_WIDTH;
	b = y + CHOOSE_BBOX_HEIGHT;
}

