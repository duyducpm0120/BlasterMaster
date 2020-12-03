#include "OHSophiaBullet.h"

void COHSophiaBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	damage = level * 2;
	CGameObject::Update(dt);
	switch (type) {

		case OHSOPHIABULLET_TYPE_STRAIGHT: {
			if (state == OHSOPHIABULLET_STATE_FLYING_LEFT) {
				vy = 0;
				vx = -OHSOPHIABULLET_SPEED;
				if (x < startPositionX - OHSOPHIABULLET_FLYING_SPACE)
				{
					visible = false;
					return;
				}
				else {
					x += dx;
					y += dy;
				}
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_RIGHT) {
				vy = 0;
				vx = OHSOPHIABULLET_SPEED;
				if (x > startPositionX + OHSOPHIABULLET_FLYING_SPACE)
				{
					visible = false;
					return;
				}
				else {
					x += dx;
					y += dy;
				}
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_UP) {
				vx = 0;
				vy = -OHSOPHIABULLET_SPEED;
				if (y < startPositionY - OHSOPHIABULLET_FLYING_SPACE)
				{
					visible = false;
					return;
				}
				else {
					x += dx;
					y += dy;
				}
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_DOWN) {
				vx = 0;
				vy = OHSOPHIABULLET_SPEED;
				if (y > startPositionY + OHSOPHIABULLET_FLYING_SPACE)
				{
					visible = false;
					return;
				}
				else {
					x += dx;
					y += dy;
				}
			}
			break;
		}
		case OHSOPHIABULLET_TYPE_ROUNDED: {

			break;
		}
		case OHSOPHIABULLET_TYPE_SIN: {

			break;
		}
	}
}

void COHSophiaBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + OHSOPHIABULLET_BBOX_WIDTH;
	bottom = y + OHSOPHIABULLET_BBOX_HEIGHT;
}

void COHSophiaBullet::Render()
{
	int ani;
	if (level == 1)
		ani = OHSOPHIABULLET_ANI_LV1;
	else
		ani = OHSOPHIABULLET_ANI_LV2;
	animation_set->at(ani)->Render(x, y, 255);
}

void COHSophiaBullet::SetAnchorPoint()
{
	this->point1.x = x - OHSOPHIABULLET_FLYING_SPACE/2;
	this->point1.y = y - OHSOPHIABULLET_FLYING_SPACE/2;

	this->point2.x = x + OHSOPHIABULLET_FLYING_SPACE/2;
	this->point2.y = y - OHSOPHIABULLET_FLYING_SPACE/2;

	this->point3.x = x + OHSOPHIABULLET_FLYING_SPACE/2;
	this->point3.y = y + OHSOPHIABULLET_FLYING_SPACE/2;

	this->point4.x = x - OHSOPHIABULLET_FLYING_SPACE/2;
	this->point4.y = y + OHSOPHIABULLET_FLYING_SPACE/2;

}

void COHSophiaBullet::SetState(int state)
{
	CGameObject::SetState(state);
}

COHSophiaBullet::COHSophiaBullet(int level, int state, int type): CBullet(level, state)
{
	this->type = type;
	this->level = level;
	damage = level;
	this->state = state;
}



