#include "Ladder.h"


CLadder::CLadder()
{
	width = 16;
	height =208;
}

void CLadder::Render()
{
	//RenderBoundingBox();
}

void CLadder::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}