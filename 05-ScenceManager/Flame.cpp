#include "Flame.h"

void CFlame::Render()
{
	return;
	//RenderBoundingBox();
}

void CFlame::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FLAME_BBOX_WIDTH;
	b = y + FLAME_BBOX_HEIGHT;
}


