#include "Blink.h"

void CBlink::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLINK_BBOX_WIDTH;
	bottom = y + BLINK_BBOX_HEIGHT;

	
}

void CBlink::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->x += 100;
	if (this->x >= this->startX )
	{
		this->x = this->startX;
	}
}

void CBlink::Render()
{
	int ani;
	if (state == BLINK_STATE_CLOAKED)
		ani = BLINK_ANI_CLOAKED;
	else
		ani = BLINK_ANI_UNCLOAKED;


	animation_set->at(ani)->Render(x, y);
}

CBlink::CBlink()
{
	this->state = BLINK_STATE_CLOAKED;
}

void CBlink::SetState(int state)
{
	this->state = state;
}


void CBlink::SetStartPosition(float x, float y)
{
	this->startX = x;
	this->startY = y;
}
