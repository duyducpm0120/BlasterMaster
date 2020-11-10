#include "Bullet.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;	
	bottom = y + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x > startPositionX + BULLET_FLYING_SPACE)
		return;
	else
		x += dx;
}

void CBullet::Render()
{
	int ani;
	if (direct == -1) {
		ani = BULLET_ANI_FLYING;
		animations[ani]->Render(x, y, -1);
	}
	else
		animations[0]->Render(x, y, 1);

}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	return;
}

void CBullet::SetStartPositon(float x, float y, int dirt)
{
	startPositionX = x;
	startPositionY = y;
	direct = dirt;
}
