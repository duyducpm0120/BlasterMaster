#include "Choose.h"
#include "Utils.h"
CChoose::CChoose()
{
	count = 0;
	isEnemy = false;
}

void CChoose::Render()
{
		if (count % 10 < 5)
			animation_set->at(0)->Render(this->x, this->y, 255);
		/*else
			animation_set->at(0)->Render(this->x, this->y, 15);*/
}

void CChoose::Update(DWORD dt)
{
	count++;
	if (count == 100)
		count = 0;
	DebugOut(L"count: %d \n",count);
}

void CChoose::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CHOOSE_BBOX_WIDTH;
	b = y + CHOOSE_BBOX_HEIGHT;
}

