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
			if (state == OHSOPHIABULLET_STATE_FLYING_LEFT) {
				if (angle >= 2 * 3.14)
					visible = false;
				angle -= 0.174444444 * dt / 50;
				x = point1.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
				y = point1.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				//SetPosition(x, y);
				
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_RIGHT) {
				
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_UP) {
				
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_DOWN) {
				
			}
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
	this->point1.x = startPositionX - OHSOPHIABULLET_FLYING_SPACE/2;
	this->point1.y = startPositionY - OHSOPHIABULLET_FLYING_SPACE/2;

	this->point2.x = startPositionX + OHSOPHIABULLET_FLYING_SPACE/2;
	this->point2.y = startPositionY - OHSOPHIABULLET_FLYING_SPACE/2;

	this->point3.x = startPositionX + OHSOPHIABULLET_FLYING_SPACE/2;
	this->point3.y = startPositionY + OHSOPHIABULLET_FLYING_SPACE/2;

	this->point4.x = startPositionX - OHSOPHIABULLET_FLYING_SPACE/2;
	this->point4.y = startPositionY + OHSOPHIABULLET_FLYING_SPACE/2;

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
	float pi = 2 * acos(0.0);
	this->angle = pi/4;
}



