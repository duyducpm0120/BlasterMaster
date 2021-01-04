#include "Blink.h"
#include <time.h> 
#include "Utils.h"
#include "Sound.h"
void CBlink::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLINK_BBOX_WIDTH;
	bottom = y + BLINK_BBOX_HEIGHT;
}

void CBlink::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (health <=0)visible = false;
	BlinkCounter += dt;
	if (BlinkCounter >= 500)
	{
		srand(rand()%100);
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
	if (BlinkCounter == 1)
		Sound::GetInstance()->Play("Blink", 0, 1);
	if (BlinkIncsCounter >= 4)
	{
		BlinkBack();
		BlinkIncsCounter = 0;
		Sound::GetInstance()->Play("Blink", 0, 1);
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
	this->ToggleState();
	this->y += BLINK_DISTANCE;

	BlinkIncsCounter++;
}

void CBlink::BlinkBack()
{
	this->ToggleState();
	this->x = this->startX;
	this->y = this->startY;
	
}

void CBlink::BlinkLeft()
{
	this->ToggleState();
	this->x -= BLINK_DISTANCE;
	BlinkIncsCounter++;
	
}

void CBlink::BlinkRight()
{
	this->ToggleState();
	this->x += BLINK_DISTANCE;
	BlinkIncsCounter++;
	
}

void CBlink::ToggleState()
{
	if (state == BLINK_STATE_CLOAKED) state = BLINK_STATE_UNCLOAKED;
	else if (state == BLINK_STATE_UNCLOAKED) state = BLINK_STATE_CLOAKED;

}



CBlink::CBlink()
{
	damage = 3;
	health = 2;
	nx = -1;
	isEnemy = true;
	vy = 0;
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
