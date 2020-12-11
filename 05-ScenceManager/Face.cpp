#include "Face.h"
CFace::CFace()
{
	damage = 1;
	health = 2;
	nx = -1;
	vy = 0;
	SetState(FACE_STATE_WALKING_UP);
}

void CFace::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + FACE_BBOX_WIDTH;

	if (state == FACE_STATE_DIE)
		bottom = y + FACE_BBOX_HEIGHT_DIE;
	else
		bottom = y + FACE_BBOX_HEIGHT;
}

void CFace::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (health <= 0)
		this->visible = false;
	//
	// TO-DO: make sure FACE can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vy < 0 && y < (startY - 80)) {
		y = startY - 80; vy = -vy;
		SetState(FACE_STATE_WALKING_DOWN);
	}

	if (vy > 0 && y > startY) {
		y = startY; vy = -vy;
		SetState(FACE_STATE_WALKING_UP);
	}
}

void CFace::Render()
{
	int ani;
	if (state == FACE_STATE_WALKING_UP)
		ani = FACE_ANI_WALKING_UP;
	else
		ani = FACE_ANI_WALKING_DOWN;


	/*if (state == FACE_STATE_DIE) {
		return;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CFace::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FACE_STATE_DIE:
		//y += FACE_BBOX_HEIGHT - FACE_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case FACE_STATE_WALKING_UP:
		ny = -1;
		vy = -FACE_WALKING_SPEED;
		break;
	case FACE_STATE_WALKING_DOWN:
		ny = 1;
		vy = FACE_WALKING_SPEED;
	}
}

void CFace::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}
