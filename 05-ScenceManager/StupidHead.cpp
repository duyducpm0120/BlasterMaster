#include "StupidHead.h"
#include "Brick.h"
#include "Utils.h"
void CStupidHead::Fall()
{
	state = STUPIDHEAD_STATE_FALL;
	vx = 0;
	vy = 2*STUPIDHEAD_CLIMBING_SPEED;
	ani = STUPIDHEAD_ANI_CLIMBING_TOP_LEFT;

}
void CStupidHead::ChangeDirection()
{
	float vx0 = vx, vy0 = vy;
	vx = vy0;
	vy = vx0;
}
void CStupidHead::ChangeDirectionInverse()
{
	float vx0 = vx, vy0 = vy;
	vx = -vy0;
	vy = -vx0;
}
void CStupidHead::HandleIsTouchBrick()
{
	if (isTouchBrick == false) {
		if (state == STUPIDHEAD_STATE_CLIMBING_RIGHT) {
			if (this->vy > 0) {
				ani = STUPIDHEAD_ANI_CLIMBING_TOP_LEFT;
				state = STUPIDHEAD_STATE_CLIMBING_TOP;
				ChangeDirection();
				this->y -= DEVIATIONS;
			}
			else if (this->vy < 0) {
				ani = STUPIDHEAD_ANI_CLIMBING_BOT_RIGHT;
				state = STUPIDHEAD_STATE_CLIMBING_BOT;
				ChangeDirectionInverse();
				this->y += DEVIATIONS;
			}
		}
		else if (state == STUPIDHEAD_STATE_CLIMBING_LEFT) {
			if (vy > 0) {

				ani = STUPIDHEAD_ANI_CLIMBING_TOP_RIGHT;
				state = STUPIDHEAD_STATE_CLIMBING_TOP;
				ChangeDirectionInverse();
				this->y -=DEVIATIONS;
			}
			else if (vy < 0) {
				ani = STUPIDHEAD_ANI_CLIMBING_BOT_LEFT;
				state = STUPIDHEAD_STATE_CLIMBING_BOT;
				ChangeDirection();
				this->y += DEVIATIONS;
			}
		}
		else if (state == STUPIDHEAD_STATE_CLIMBING_TOP) {
			if (vx > 0) {

				ani = STUPIDHEAD_ANI_CLIMBING_LEFT_LEFT;
				state = STUPIDHEAD_STATE_CLIMBING_LEFT;
				ChangeDirectionInverse();
				this->x -= DEVIATIONS;
			}
			else if (vx < 0) {

				ani = STUPIDHEAD_ANI_CLIMBING_RIGHT_RIGHT;
				state = STUPIDHEAD_STATE_CLIMBING_RIGHT;
				ChangeDirection();
				this->x += DEVIATIONS;
			}
		}
		else if (state == STUPIDHEAD_STATE_CLIMBING_BOT) {
			if (vx > 0) {

				ani = STUPIDHEAD_ANI_CLIMBING_LEFT_RIGHT;
				state = STUPIDHEAD_STATE_CLIMBING_LEFT;
				ChangeDirection();
				this->x -= DEVIATIONS;
			}
			else if (vx < 0) {

				ani = STUPIDHEAD_ANI_CLIMBING_RIGHT_LEFT;
				state = STUPIDHEAD_STATE_CLIMBING_RIGHT;
				ChangeDirection();
				this->x += DEVIATIONS;
			}
		}
	}
}
CStupidHead::CStupidHead()
{
	nx = 1;
	ny = 0;
	isEnemy = true;
	damage = 1;
	health = 10000;
	nx = -1;
	state = STUPIDHEAD_STATE_CLIMBING_TOP;
	vx = STUPIDHEAD_CLIMBING_SPEED;
	vy = 0;
	isTouchBrick = true;
}

void CStupidHead::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	isEnemy = true;
	left = x;
	top = y;
	right = x + STUPIDHEAD_BBOX_WIDTH;
	bottom = y + STUPIDHEAD_BBOX_HEIGHT;
}

void CStupidHead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//
	// TO-DO: make sure STUPIDHEAD can interact with the world and to each of them too!
	// 
	CGameObject::Update(dt,coObjects);
	if (this->state != STUPIDHEAD_STATE_CLIMBING_BOT && target != NULL && abs(this->x - target->x) <= 3 && this->y < target->y)
		Fall();
	if (this->vy == 0)
		this->ny = 0;
	else if (this->vy > 0)
		this->ny = 1;
	else
		this->ny = -1;


	if (this->vx == 0)
		this->nx = 0;
	else if (this->vx > 0)
		this->nx = 1;
	else
		this->nx = -1;



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	vector<LPCOLLISIONEVENT> coCollidingEvents;
	CalcColliding(coObjects, coCollidingEvents);

	isTouchBrick = false;
	for (int i = 0; i < coCollidingEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollidingEvents[i];
		if (dynamic_cast<CBrick*>(e->obj)) {
			isTouchBrick = true;
		}
	}

	HandleIsTouchBrick();

	x += dx;
	y += dy;
	float min_tx, min_ty;
	float nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];
		if (dynamic_cast<CBrick*>(e->obj)) {
			isTouchBrick = true;
			if (state == STUPIDHEAD_STATE_CLIMBING_TOP && e->ny == 0 && e->nx != 0 && (abs(this->y - e->obj->y) < 16)) {
				if (e->nx == -1) {
					ChangeDirection();
					ani = STUPIDHEAD_ANI_CLIMBING_RIGHT_LEFT;
					state = STUPIDHEAD_STATE_CLIMBING_RIGHT;
				}
				else if (e->nx == 1) {
					ChangeDirectionInverse();
					ani = STUPIDHEAD_ANI_CLIMBING_LEFT_RIGHT;
					state = STUPIDHEAD_STATE_CLIMBING_LEFT;
				}
			}
			else if (state == STUPIDHEAD_STATE_CLIMBING_BOT && e->ny == 0 && e->nx != 0 && (abs(this->y - e->obj->y) < 16)) {
				if (e->nx == -1) {
					ChangeDirectionInverse();
					ani = STUPIDHEAD_ANI_CLIMBING_RIGHT_RIGHT;
					state = STUPIDHEAD_STATE_CLIMBING_RIGHT;
				}
				else if (e->nx == 1) {
					ChangeDirection();
					ani = STUPIDHEAD_ANI_CLIMBING_LEFT_LEFT;
					state = STUPIDHEAD_STATE_CLIMBING_LEFT;
				}
			}
			else if (state == STUPIDHEAD_STATE_CLIMBING_LEFT && e->ny != 0 && e->nx == 0 && (abs(this->x - e->obj->x) < 16)) {
				if (e->ny == -1)
				{
					ChangeDirection();
					ani = STUPIDHEAD_ANI_CLIMBING_BOT_RIGHT;
					state = STUPIDHEAD_STATE_CLIMBING_BOT;
				}
				else if (e->ny == 1)
				{
					ChangeDirectionInverse();
					ani = STUPIDHEAD_ANI_CLIMBING_TOP_LEFT;
					state = STUPIDHEAD_STATE_CLIMBING_TOP;
				}
			}
			else if (state == STUPIDHEAD_STATE_CLIMBING_RIGHT && e->ny != 0 && e->nx == 0 && (abs(this->x - e->obj->x) < 16)) {
				if (e->ny == 1) {
					ChangeDirection();
					ani = STUPIDHEAD_ANI_CLIMBING_TOP_RIGHT;
					state = STUPIDHEAD_STATE_CLIMBING_TOP;
				}
				else if (e->ny == -1) {
					ChangeDirectionInverse();
					ani = STUPIDHEAD_ANI_CLIMBING_BOT_LEFT;
					state = STUPIDHEAD_STATE_CLIMBING_BOT;
				}
			}
			else if (state == STUPIDHEAD_STATE_FALL) {
				vx = 0;
				vy = vy / 2;
				if (this->x > target->x) {
					ChangeDirectionInverse();
					ani = STUPIDHEAD_ANI_CLIMBING_BOT_LEFT;
					state = STUPIDHEAD_STATE_CLIMBING_BOT;
				}
				else {
					ChangeDirection();
					ani = STUPIDHEAD_ANI_CLIMBING_BOT_RIGHT;
					state = STUPIDHEAD_STATE_CLIMBING_BOT;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CStupidHead::Render()
{
	animation_set->at(ani)->Render(x, y);

}

void CStupidHead::SetState(int state)
{
	CGameObject::SetState(state);
}