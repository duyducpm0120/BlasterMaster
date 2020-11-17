#include "Butterfly.h"
#include "Golem.h"
CButterfly::CButterfly()
{
	nx = -1;
	SetState(BUTERFLY_STATE_WALKING_LEFT);
}

void CGolem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BUTERFLY_BBOX_WIDTH;
	bottom = y + BUTERFLY_BBOX_HEIGHT;
}

void CGolem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Golem can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 50) {
		x = 50; vx = -vx;
		SetState(BUTERFLY_STATE_WALKING_RIGHT);
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
		SetState(BUTERFLY_STATE_WALKING_LEFT);
	}
}

void CGolem::Render()
{
	int ani;
	if (state == BUTERFLY_STATE_WALKING_LEFT)
		ani = BUTERFLY_ANI_WALKING_LEFT;
	else
		ani = BUTERFLY_ANI_WALKING_RIGHT;


	/*if (state == BUTERFLY_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGolem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BUTERFLY_STATE_DIE:
		//y += BUTERFLY_BBOX_HEIGHT - BUTERFLY_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case BUTERFLY_STATE_WALKING_LEFT:
		nx = -1;
		vx = -BUTERFLY_WALKING_SPEED;
		break;
	case BUTERFLY_STATE_WALKING_RIGHT:
		nx = 1;
		vx = BUTERFLY_WALKING_SPEED;
	}
}
