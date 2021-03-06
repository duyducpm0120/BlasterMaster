#include "OHSophia.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Utils.h"
#include "Golem.h"
#include "Item.h"
#include "Flame.h"
#include "Rocket.h"
#include "Brick.h"
#include "Bullet.h"
#include "Sophia.h"
#include "AutoRunPortal.h"
#include "PlayScence.h"
#include "EnemyBullet.h"
#include "OHSophiaBullet.h"
#include "Sound.h"
#include "Boss.h"
COHSophia::COHSophia(float x, float y)
{
	isPlayer = true;
	bulletLevel = 1;
	enableRocket = true;
	isJumping = false;
	untouchable = 0;
	SetState(OHSOPHIA_STATE_IDLE_RIGHT);
	OHSophia_width = OHSOPHIA_WIDTH;
	OHSophia_height = OHSOPHIA_HEIGHT;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	untouchableTime = 0;
}

void COHSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == OHSOPHIA_STATE_DIE)
		return;
	if (damage > 4)
		bulletLevel = 2;
	else
		bulletLevel = 1;
	if (damage == 8) {
		bulletFullPower = true;
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	//vy += OHSOPHIA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPCOLLISIONEVENT> coCollisoningEvents;


	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	CalcColliding(coObjects, coCollisoningEvents);
	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		if (dynamic_cast<CBrick*>(e->obj))
		{

		}
		else if (dynamic_cast<CEnemyBullet*>(e->obj))
		{
			if (untouchableTime == 0) {
				health -= e->obj->GetDamage();
				untouchableTime = 1;
			}
			e->obj->visible = false;
			//vy -= 0.3f;
			if (health <= 0)
				visible = false;
		}
		else if (dynamic_cast<CFlame*>(e->obj)) {
			if (untouchableTime == 0) {
				health--;
				untouchableTime = 1;
			}
			if (health <= 0)
				visible = false;
		}
	}


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


			if (e->obj->IsEnemy() && !dynamic_cast<CBoss*>(e->obj)) {
				if (untouchableTime == 0) {
					health -= e->obj->GetDamage();
					untouchableTime = 1;
					e->obj->visible = false;
				}
				if (dynamic_cast<CEnemyBullet*>(e->obj))
					e->obj->visible = false;
				//vy -= 0.3f;
				if (health <= 0)
					visible = false;
			}
			else if (dynamic_cast<CItem*>(e->obj)) {
				Sound::GetInstance()->Play("PickingItems", 0, 1);
				switch (dynamic_cast<CItem*>(e->obj)->GetType())
				{
				case ITEM_TYPE_HEALTH:
					if (health < 8 && health >0)
						health++;
					(e->obj)->visible = false;
					break;

				case ITEM_TYPE_THUNDER:

					if (damage < 8 && damage >0)
						damage++;
					(e->obj)->visible = false;
					break;

				case ITEM_TYPE_ENABLE_ROCKET:

					enableRocket = true;
					dynamic_cast<CItem*>(e->obj)->visible = false;
					break;

				case ITEM_TYPE_POWER:
					damage += 4;
					if (damage >= 8)
						damage = 8;
					dynamic_cast<CItem*>(e->obj)->visible = false;
					break;

				}
			}
			else if (dynamic_cast<CFlame*>(e->obj)) {
				if (untouchableTime == 0) {
					health--;
					untouchableTime = 1;
				}
				if (health <= 0)
					visible = false;
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame* game = CGame::GetInstance();
				game->SwitchToScene(p->GetSceneId());
			}
			else if (dynamic_cast<CAutoRunPortal*>(e->obj))
			{
				CAutoRunPortal* p = dynamic_cast<CAutoRunPortal*>(e->obj);
				CGame* game = CGame::GetInstance();
				float movX, movY;
				dynamic_cast<CAutoRunPortal*>(e->obj)->GetMovingSpace(movX, movY);
				if (dynamic_cast<CPlayScene*>(game->GetCurrentScene())->IsCameraAutorun() == false)
					dynamic_cast<CPlayScene*>(game->GetCurrentScene())->SetCameraAutorunTarget(movX, movY);
			}


		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	HandleUntouchableTime();
}

void COHSophia::Render()
{

	int ani;
	switch (state)
	{
		/*case OHSOPHIA_STATE_DIE:
			ani = OHSOPHIA_ANI_DIE;
			break;*/
	case OHSOPHIA_STATE_IDLE_RIGHT:
		ani = OHSOPHIA_ANI_IDLE_RIGHT;
		break;
	case OHSOPHIA_STATE_IDLE_LEFT:
		ani = OHSOPHIA_ANI_IDLE_LEFT;
		break;
	case OHSOPHIA_STATE_WALKING_LEFT:
		ani = OHSOPHIA_ANI_WALKING_LEFT;
		break;
	case OHSOPHIA_STATE_WALKING_RIGHT:
		ani = OHSOPHIA_ANI_WALKING_RIGHT;
		break;
	case OHSOPHIA_STATE_IDLE_DOWN:
		ani = OHSOPHIA_ANI_IDLE_DOWN;
		break;
	case OHSOPHIA_STATE_IDLE_UP:
		ani = OHSOPHIA_ANI_IDLE_UP;
		break;
	case OHSOPHIA_STATE_WALKING_UP:
		ani = OHSOPHIA_ANI_WALKING_UP;
		break;
	case OHSOPHIA_STATE_WALKING_DOWN:
		ani = OHSOPHIA_ANI_WALKING_DOWN;
		break;

	}


	int alpha = 255;
	/*if ((ani == OHSOPHIA_ANI_WALKING_RIGHT || ani == OHSOPHIA_ANI_IDLE_RIGHT || state == OHSOPHIA_STATE_JUMP || state == OHSOPHIA_STATE_DIE) && nx == 1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);*/
	if (untouchableTime % 10 < 5)
		animation_set->at(ani)->Render(x, y, alpha);
	else
		animation_set->at(ani)->Render(x, y, 50);


	//RenderBoundingBox();

}

void COHSophia::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case OHSOPHIA_STATE_WALKING_RIGHT:
		isJumping = false;
		vx = OHSOPHIA_WALKING_SPEED;
		vy = 0;
		nx = 1;
		break;
	case OHSOPHIA_STATE_WALKING_LEFT:
		isJumping = false;
		vx = -OHSOPHIA_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case OHSOPHIA_STATE_IDLE_RIGHT:
		isJumping = false;
		vx = 0;
		vy = 0;
		nx = 1;
		break;
	case OHSOPHIA_STATE_IDLE_LEFT:
		isJumping = false;
		vx = 0;
		vy = 0;
		nx = -1;
		break;

	case OHSOPHIA_STATE_WALKING_UP:
		isJumping = false;
		vy = -OHSOPHIA_WALKING_SPEED;
		vx = 0;
		nx = 1;
		break;
	case OHSOPHIA_STATE_WALKING_DOWN:
		isJumping = false;
		vx = 0;
		vy = OHSOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case OHSOPHIA_STATE_IDLE_UP:
		isJumping = false;
		vx = 0;
		vy = 0;
		nx = 1;
		break;
	case OHSOPHIA_STATE_IDLE_DOWN:
		isJumping = false;
		vx = 0;
		vy = 0;
		nx = -1;
		break;
	case OHSOPHIA_STATE_DIE:
		//vy = -OHSOPHIA_DIE_DEFLECT_SPEED;
		y -= 23;
		break;

	}

}

void COHSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + OHSophia_width;
	bottom = y + OHSophia_height;
}
void COHSophia::SetDimension(int width, int height)
{
	this->OHSophia_width = width;
	this->OHSophia_height = height;
}
void COHSophia::GetDimension(int& width, int& height)
{
	width = this->OHSophia_width;
	height = this->OHSophia_height;
}
void COHSophia::Reset()
{
	SetState(OHSOPHIA_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void COHSophia::HandleUntouchableTime()
{
	if (untouchableTime == 0)
		return;
	untouchableTime += 1;
	if (untouchableTime == OHSOPHIA_UNTOUCHABLE_TIME)
		untouchableTime = 0;
}

void COHSophia::CallSecondWeapon()
{
}

void COHSophia::Shot()
{
	Sound::GetInstance()->Play("PlayerFireOverWorld", 0, 1);
	vector<LPGAMEOBJECT>* objects = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetObjects();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	if (GetBulletLevel() == 1 && bulletFullPower == 0) {
		if (state == OHSOPHIA_STATE_WALKING_LEFT || state == OHSOPHIA_STATE_IDLE_LEFT)
		{
			COHSophiaBullet* bullet;
			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetStartPositon(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);

		}
		else if (state == OHSOPHIA_STATE_WALKING_RIGHT || state == OHSOPHIA_STATE_IDLE_RIGHT)
		{
			COHSophiaBullet* bullet;
			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);

		}
		else if (state == OHSOPHIA_STATE_WALKING_UP || state == OHSOPHIA_STATE_IDLE_UP) {

			COHSophiaBullet* bullet;

			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1);
			bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);

		}
		else if (state == OHSOPHIA_STATE_WALKING_DOWN || state == OHSOPHIA_STATE_IDLE_DOWN) {
			COHSophiaBullet* bullet;

			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, GetBulletLevel() - 1 + bulletFullPower, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
			bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);
		}
	}
	else if (bulletLevel == 2 && bulletFullPower == 0) {
	
		if (state == OHSOPHIA_STATE_WALKING_LEFT || state == OHSOPHIA_STATE_IDLE_LEFT)
		{
			COHSophiaBullet* bullet;
			for (int i = 0; i < 3; i++) {

				if (i == 0) {
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				}
				else if (i == 1)
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_ROUNDED, 1);
				else
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_ROUNDED, -1);
				float x1, y1;
				GetPosition(x1, y1);
				bullet->SetPosition(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetStartPositon(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
		}
		else if (state == OHSOPHIA_STATE_WALKING_RIGHT || state == OHSOPHIA_STATE_IDLE_RIGHT)
		{
			COHSophiaBullet* bullet;
			for (int i = 0; i < 3; i++) {

				if (i == 0) {
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				}
				else if (i == 1)
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_ROUNDED, 1);
				else
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_ROUNDED, -1);
				float x1, y1;
				GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
		}
		else if (state == OHSOPHIA_STATE_WALKING_UP || state == OHSOPHIA_STATE_IDLE_UP) {

			COHSophiaBullet* bullet;
			for (int i = 0; i < 3; i++) {

				if (i == 0) {
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				}
				else if (i == 1)
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_ROUNDED, 1);
				else
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_ROUNDED, -1);
				float x1, y1;
				GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
		}
		else if (state == OHSOPHIA_STATE_WALKING_DOWN || state == OHSOPHIA_STATE_IDLE_DOWN) {
			COHSophiaBullet* bullet;
			for (int i = 0; i < 3; i++) {

				if (i == 0) {
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				}
				else if (i == 1)
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_ROUNDED, 1);
				else
					bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_ROUNDED, -1);
				float x1, y1;
				GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}

		}
	}
	else if (bulletLevel == 2 && bulletFullPower == 1) {
		if (state == OHSOPHIA_STATE_WALKING_LEFT || state == OHSOPHIA_STATE_IDLE_LEFT)
		{
			COHSophiaBullet* bullet;

			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_SIN, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetStartPositon(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);

		}
		else if (state == OHSOPHIA_STATE_WALKING_RIGHT || state == OHSOPHIA_STATE_IDLE_RIGHT)
		{
			COHSophiaBullet* bullet;
			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_SIN, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);

		}
		else if (state == OHSOPHIA_STATE_WALKING_UP || state == OHSOPHIA_STATE_IDLE_UP) {

			COHSophiaBullet* bullet;

			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_SIN, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1);
			bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);

		}
		else if (state == OHSOPHIA_STATE_WALKING_DOWN || state == OHSOPHIA_STATE_IDLE_DOWN) {
			COHSophiaBullet* bullet;

			bullet = new COHSophiaBullet(GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_SIN, 1);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
			bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
			bullet->SetAnchorPoint();
			LPANIMATION_SET ani_set = animation_sets->Get(21);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);
		}
	}
	
}
