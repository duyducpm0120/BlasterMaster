#include "AutoRunPortal.h"


CAutoRunPortal::CAutoRunPortal(float l, float t, float r, float b,  float targetX, float targetY)
{
	this->targetX = targetX;
	this->targetY = targetY;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}

void CAutoRunPortal::Render()
{
	RenderBoundingBox();
}

void CAutoRunPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}