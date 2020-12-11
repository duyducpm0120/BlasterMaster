#include "Blink.h"
#include <time.h> 
#include "Utils.h"
void CBlink::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLINK_BBOX_WIDTH;
	bottom = y + BLINK_BBOX_HEIGHT;

	
}

void CBlink::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	
	BlinkCounter += dt;
	if (BlinkCounter >= 1000)
	{
		srand(time(NULL));
		int n = (rand() % 3);

		if (n ==0)
		{
			BlinkFoward();
		}
		else if (n==1)
		{
			BlinkRight();
		}
		else 
		{
			BlinkLeft();
		}
		BlinkCounter = 0;
	}
	
	if (BlinkIncsCounter >= 3)
	{
		BlinkBack();
		BlinkIncsCounter = 0;
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

void CBlink::BlinkFoward()
{
	this->SetState(BLINK_STATE_UNCLOAKED);
	this->y += BLINK_DISTANCE;

	BlinkIncsCounter++;
}

void CBlink::BlinkBack()
{
	this->SetState(BLINK_STATE_CLOAKED);
	this->x = this->startX;
	this->y = this->startY;
	
}

void CBlink::BlinkLeft()
{
	this->SetState(BLINK_STATE_UNCLOAKED);
	this->x -= BLINK_DISTANCE;
	BlinkIncsCounter++;
	
}

void CBlink::BlinkRight()
{
	this->SetState(BLINK_STATE_UNCLOAKED);
	this->x += BLINK_DISTANCE;
	BlinkIncsCounter++;
	
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
