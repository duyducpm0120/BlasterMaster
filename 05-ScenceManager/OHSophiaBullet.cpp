#include "OHSophiaBullet.h"
#include "Brick.h"

void COHSophiaBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	damage = level * 2;
	CGameObject::Update(dt);
	switch (type) {

		case OHSOPHIABULLET_TYPE_STRAIGHT: {
			if (state == OHSOPHIABULLET_STATE_FLYING_LEFT) {
				vy = 0;
				vx = -OHSOPHIABULLET_SPEED*2/3;
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
				vx = OHSOPHIABULLET_SPEED * 2 / 3;
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
				vy = -OHSOPHIABULLET_SPEED * 2 / 3;
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
				vy = OHSOPHIABULLET_SPEED * 2 / 3;
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
				angle += ALPHA* direct * dt ;
				radius_increase += 0.1;
				if (direct == 1) {
					x = point1.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point1.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}
				else {
				
					x = point3.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point3.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}
				
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_RIGHT) {
				angle += ALPHA * direct * dt;
				radius_increase+=0.1;
				if (direct == -1) {
					x = point1.x + (OHSOPHIA_RADIUS_SPACE + radius_increase) * cos(angle);
					y = point1.y + (OHSOPHIA_RADIUS_SPACE + radius_increase) * sin(angle);
				}
				else {

					x = point3.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point3.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}

			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_UP) {
				angle += ALPHA * direct * dt;
				radius_increase += 0.1;
				if (direct == -1) {
					x = point4.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point4.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}
				else {

					x = point2.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point2.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_DOWN) {
				angle += ALPHA * direct * dt;
				radius_increase += 0.1;
				if (direct == -1) {
					x = point2.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point2.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}
				else {

					x = point4.x + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * cos(angle);
					y = point4.y + (OHSOPHIA_RADIUS_SPACE + radius_increase ) * sin(angle);
				}
			}
			break;	
		}
		case OHSOPHIABULLET_TYPE_SIN: {
			angle += ALPHA * direct * dt * 100;
			if (state == OHSOPHIABULLET_STATE_FLYING_LEFT) {
				if (x < startPositionX - OHSOPHIABULLET_FLYING_SPACE)
					visible = false;
				else
					x -= OHSOPHIABULLET_SPEED * dt;

				y = startPositionY + OHSOPHIA_RADIUS_SPACE * sin(angle);

			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_RIGHT) {
				if (x > startPositionX + OHSOPHIABULLET_FLYING_SPACE)
					visible = false;
				else
					x += OHSOPHIABULLET_SPEED * dt;

				y = startPositionY + OHSOPHIA_RADIUS_SPACE * sin(angle);
			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_UP) {
				if (y < startPositionY - OHSOPHIABULLET_FLYING_SPACE)
					visible = false;
				else
					y += -OHSOPHIABULLET_SPEED * dt;

				x = startPositionX + OHSOPHIA_RADIUS_SPACE * cos(angle);

			}
			else if (state == OHSOPHIABULLET_STATE_FLYING_DOWN) {
				if (y > startPositionY + OHSOPHIABULLET_FLYING_SPACE)
					visible = false;
				else
					y += OHSOPHIABULLET_SPEED * dt;

				x = startPositionX + OHSOPHIA_RADIUS_SPACE * cos(angle);
			}
			break; 
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CalcPotentialCollisions(coObjects, coEvents);


	vector<LPCOLLISIONEVENT> coCollisoningEvents;
	CalcColliding(coObjects, coCollisoningEvents);

	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		if ((e->obj)->IsEnemy()) {
			e->obj->TakeDamage(this->damage);				//Destroy every enemy
			this->visible = false;

		}
		else if (dynamic_cast<CBrick*>(e->obj)) {
			this->visible = false;
		}

	}


	float min_tx, min_ty, nx = 0, ny;
	float rtx = 0;
	float rty = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rtx, rty);
	x += dx;
	y += dy;
	// block 
	//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//y += min_ty * dy + ny * 0.4f;


	if (nx != 0) vx = 0;
	if (ny != 0) vy = 0;

	// Collision logic with Goombas
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (e->obj->IsEnemy()) {
			e->obj->TakeDamage(this->damage);				//Destroy every enemy
			this->visible = false;
		}
		else if (dynamic_cast<CBrick*>(e->obj)) {
			this->visible = false;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
	this->point1.y = startPositionY - OHSOPHIA_RADIUS_SPACE;

	this->point2.x = startPositionX + OHSOPHIA_RADIUS_SPACE;
	this->point2.y = startPositionY ;

	this->point3.x = startPositionX;
	this->point3.y = startPositionY + OHSOPHIA_RADIUS_SPACE;

	this->point4.x = startPositionX - OHSOPHIA_RADIUS_SPACE;
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
	this->damage = level;
	this->state = state;
	radius_increase = 0;
	vx = 0 ;
	vy = 0;
	SetAngle();
}



