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
				if (angle >= 2*3.14 || angle <-2*3.14)
					visible = false;
				angle += ALPHA* direct * dt ;
				if (direct == 1) {
					x = point1.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point1.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
				else {
				
					x = point3.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point3.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
				
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_RIGHT) {
				if (angle >= 2 * 3.14 || angle < -2 * 3.14)
					visible = false;
				angle += ALPHA * direct * dt;
				if (direct == -1) {
					x = point1.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point1.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
				else {

					x = point3.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point3.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}

			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_UP) {
				if (angle >= 2 * 3.14 || angle < -2 * 3.14)
					visible = false;
				angle += ALPHA * direct * dt;
				if (direct == -1) {
					x = point4.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point4.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
				else {

					x = point2.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point2.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_DOWN) {
				if (angle >= 2 * 3.14 || angle < -2 * 3.14)
					visible = false;
				angle += ALPHA * direct * dt;
				if (direct == -1) {
					x = point2.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point2.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
				else {

					x = point4.x + OHSOPHIA_RADIUS_SPACE * cos(angle);
					y = point4.y + OHSOPHIA_RADIUS_SPACE * sin(angle);
				}
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
	this->point1.x = startPositionX;
	this->point1.y = startPositionY - OHSOPHIABULLET_FLYING_SPACE/2;

	this->point2.x = startPositionX + OHSOPHIABULLET_FLYING_SPACE/2;
	this->point2.y = startPositionY ;

	this->point3.x = startPositionX;
	this->point3.y = startPositionY + OHSOPHIABULLET_FLYING_SPACE/2;

	this->point4.x = startPositionX - OHSOPHIABULLET_FLYING_SPACE/2;
	this->point4.y = startPositionY;

}

void COHSophiaBullet::SetState(int state)
{
	CGameObject::SetState(state);
}

void COHSophiaBullet::SetAngle()
{
	float pi = 2 * acos(0.0);
	if (state == OHSOPHIABULLET_STATE_FLYING_LEFT) {
		if (direct == 1)
			this->angle = pi / 2;
		else
			this->angle = -pi / 2;
	}
	else if (state == OHSOPHIABULLET_STATE_FLYING_RIGHT) {
		if (direct == 1)
			this->angle = -pi / 2;
		else
			this->angle = pi / 2;
	}
	else if (state == OHSOPHIABULLET_STATE_FLYING_UP) {
		if (direct == 1)
			this->angle = pi;
		else
			this->angle = 0;
	}
	else if (state == OHSOPHIABULLET_STATE_FLYING_DOWN) {
		if (direct == 1)
			this->angle = 0;
		else
			this->angle = pi;
	}
}

COHSophiaBullet::COHSophiaBullet(int level, int state, int type, int direct): CBullet(level, state)
{
	this->direct = direct;
	this->type = type;
	this->level = level;
	damage = level;
	this->state = state;
	SetAngle();
}



