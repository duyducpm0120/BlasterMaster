#include "Tank.h"
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
#include "PlayScence.h"
#include "Thunder.h"
#include "Sound.h"
#include "EnemyBullet.h"
CTank:: CTank(float x, float y) 
{
	isPlayer = true;
	bulletLevel = 1;
	enableRocket = false;
	enableThunder = false;
	untouchable = 0;
	SetState(TANK_STATE_IDLE_RIGHT);
	ani = TANK_ANI_IDLE_RIGHT;
	isJumping = true;
	tank_width = TANK_NORMAL_WIDTH;
	tank_height = TANK_NORMAL_HEIGHT;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	untouchableTime = 0;
}

void CTank::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	DebugOut(L"dt : %d \n",dt);
	if (state == TANK_STATE_DIE)
		return;
	if (damage > 4)
		bulletLevel = 2;
	else
		bulletLevel = 1;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if(isJumping == true)
		vy += TANK_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPCOLLISIONEVENT> coCollisoningEvents;

	
	coEvents.clear();
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	CalcColliding(coObjects, coCollisoningEvents);
	for (int i = 0; i < coCollisoningEvents.size(); i++) {
		LPCOLLISIONEVENT e = coCollisoningEvents[i];
		if (dynamic_cast<CBrick*>(e->obj)) {
			if (e->ny != -1)
				isJumping = true;
			else {
				isJumping = false;
				vy += TANK_GRAVITY * dt;
			}
		}
		else if (dynamic_cast<CEnemyBullet*>(e->obj)) {
			if (untouchableTime == 0) {
				health -= e->obj->GetDamage();
				untouchableTime = 1;
				e->obj->visible = false;
			}

			//vy -= 0.3f;
			if (health <= 0)
				visible = false;
		}
		
		if (!dynamic_cast<CBrick*>(e->obj)) {

				isJumping = true;
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
		
		x += min_tx * dx + nx * 1.0f;
		y += min_ty * dy + (ny < 0 ? ny : 0) * 0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];		
			if (dynamic_cast<CPortal*>(e->obj))
			{
				if (dynamic_cast<CPortal*>(e->obj)->GetSceneId() != 9 && dynamic_cast<CPortal*>(e->obj)->GetSceneId() != 10) {
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame* game = CGame::GetInstance();
					game->SwitchToScene(p->GetSceneId());
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj)) {				
				if (e->ny != -1)
					isJumping = true;
				else {
					isJumping = false;
					vy += TANK_GRAVITY * dt;
				}
			}
			else if (dynamic_cast<CItem*>(e->obj)) {
				switch (dynamic_cast<CItem*>(e->obj)->GetType())
				{
					case ITEM_TYPE_HEALTH:
					{
						if (health < 8 && health >0)
							health ++;
						(e->obj)->visible = false;
						break;
					}
					case ITEM_TYPE_THUNDER:
					{			
						if (enableThunder == false) {
							enableThunder = true;
							dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->CallNewText(1, e->obj->x - 50, e->obj->y);
						}
						dynamic_cast<CItem*>(e->obj)->visible = false;
						break;
					}
					case ITEM_TYPE_ENABLE_ROCKET:
					{
						if (enableRocket == false) {
							enableRocket = true;							
							dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->CallNewText(0, e->obj->x - 50, e->obj->y);
						}
						dynamic_cast<CItem*>(e->obj)->visible = false;
						break;
					}
				}
				Sound::GetInstance()->Play("PickingItems", 0, 1);
			}
			else if (dynamic_cast<CFlame*>(e->obj)) {
				if (untouchableTime == 0) {
					health--;
					Sound::GetInstance()->Play("PlayerInjured", 0, 1);
					untouchableTime = 1;				
				}
				if (health <= 0)
					visible = false;			
			}
			else if (dynamic_cast<CSophia*>(e->obj)) {
					x += - nx * 0.4f;
			}	
			else if (dynamic_cast<CEnemyBullet*>(e->obj)) {
				if (untouchableTime == 0) {
					health -= e->obj->GetDamage();
					untouchableTime = 1;
					e->obj->visible = false;
				}

				//vy -= 0.3f;
				if (health <= 0)
					visible = false;
			}
			else {
			
				if (e->obj->IsEnemy()) {
					if (untouchableTime == 0) {
						health -= e->obj->GetDamage();
						Sound::GetInstance()->Play("PlayerInjured", 0, 1);
						untouchableTime = 1;
						e->obj->visible = false;
					}

					//vy -= 0.3f;
					if (health <= 0)
						visible = false;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];	
	HandleUntouchableTime();
}

void CTank::Render()
{
	
	switch (state)
	{
	/*case TANK_STATE_DIE:
		ani = TANK_ANI_DIE;
		break;*/
	case TANK_STATE_WALKING_RIGHT:
		ani = TANK_ANI_WALKING_RIGHT;
		break;
	case TANK_STATE_WALKING_LEFT:
		ani = TANK_ANI_WALKING_LEFT;
		break;
	case TANK_STATE_JUMP_IDLE_LEFT:
		ani = TANK_ANI_JUMP_IDLE_LEFT;
		break;
	case TANK_STATE_JUMP_IDLE_RIGHT:
		ani = TANK_ANI_JUMP_IDLE_RIGHT;
		break;
	case TANK_STATE_IDLE_RIGHT:
		ani = TANK_ANI_IDLE_RIGHT;
		break;
	case TANK_STATE_IDLE_LEFT:
		ani = TANK_ANI_IDLE_LEFT;
		break;
	case TANK_STATE_JUMP_LEFT:
		ani = TANK_ANI_JUMP_LEFT;
		break;
	case TANK_STATE_JUMP_RIGHT:
		ani = TANK_ANI_JUMP_RIGHT;
		break;
	case TANK_STATE_UPING_GUN_LEFT:
		ani = TANK_ANI_UPING_GUN_LEFT;
		break;
	case TANK_STATE_UPING_GUN_RIGHT:
		ani = TANK_ANI_UPING_GUN_RIGHT;
		break;
	case TANK_STATE_UP_GUN_LEFT:
		ani = TANK_ANI_UP_GUN_LEFT;
		break;
	case TANK_STATE_UP_GUN_RIGHT:
		ani = TANK_ANI_UP_GUN_RIGHT;
		break;
	case TANK_STATE_UP_GUN_WALKING_LEFT:
		ani = TANK_ANI_UP_GUN_WALKING_LEFT;
		break;
	case TANK_STATE_UP_GUN_WALKING_RIGHT:
		ani = TANK_ANI_UP_GUN_WALKING_RIGHT;
		break;
	}


	int alpha = 255;	
	/*if ((ani == TANK_ANI_WALKING_RIGHT || ani == TANK_ANI_IDLE_RIGHT || state == TANK_STATE_JUMP || state == TANK_STATE_DIE) && nx == 1)
		animations[ani]->Render(x, y, 1, alpha);
	else
		animations[ani]->Render(x, y, -1, alpha);*/
	if(untouchableTime % 10 < 5)
		animation_set->at(ani)->Render(x, y, alpha);
	else
		animation_set->at(ani)->Render(x, y, 50);

	//RenderBoundingBox();

}

void CTank::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TANK_STATE_WALKING_RIGHT:
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_WALKING_LEFT:
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_JUMP_IDLE_LEFT:
		nx = -1;
		vy = -TANK_JUMP_SPEED_Y;
		isJumping = true;
		break;
	case TANK_STATE_JUMP_IDLE_RIGHT:
		isJumping = true;
		nx = 1;
		vy = -TANK_JUMP_SPEED_Y;
		break;
	case TANK_STATE_IDLE_RIGHT:
		//StopFrameId = animation_set->at(ani)->GetFrame();
		vx = 0;
		nx = 1;
		break;
	case TANK_STATE_IDLE_LEFT:
		//StopFrameId = animation_set->at(ani)->GetFrame();
		vx = 0;
		nx = -1;
		break;
	case TANK_STATE_DIE:
		y -= 23;
		break;
	case TANK_STATE_JUMP_LEFT:
		isJumping = true;
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_JUMP_RIGHT:
		isJumping = true;
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_UP_GUN_WALKING_LEFT:
		vx = -TANK_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_STATE_UP_GUN_WALKING_RIGHT:
		vx = TANK_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_STATE_UP_GUN_LEFT:
		vx = 0;
		nx = -1;
		break;
	case TANK_STATE_UP_GUN_RIGHT:
		vx = 0;
		nx = 1;
		break;
	}

}

void CTank::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + tank_width;
	bottom = y + tank_height;
}
void CTank::SetDimension(int width, int height)
{
	this->tank_width = width;
	this->tank_height = height;
}
void CTank::GetDimension(int& width, int& height)
{
	width = this->tank_width;
	height = this->tank_height;
}

void CTank::Reset()
{
	SetState(TANK_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CTank::HandleUntouchableTime()
{
	if (untouchableTime == 0)
		return;
	untouchableTime += 1;
	if (untouchableTime == TANK_UNTOUCHABLE_TIME)
		untouchableTime = 0;
}
void CTank::Shot()
{
	Sound::GetInstance()->Play("PlayerFireUnderWorld", 0, 1);
	int width3, height3;
	GetDimension(width3, height3);
	if (height3 == TANK_NORMAL_HEIGHT)
	{
		if (nx == -1)
		{

			CBullet* bullet = new CBullet(GetBulletLevel(), BULLET_STATE_FLYING_LEFT);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
			bullet->SetStartPositon(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
			LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(6);
			bullet->SetAnimationSet(ani_set);
			dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects()->push_back(bullet);
		}
		else
		{
			CBullet* bullet = new CBullet(GetBulletLevel(), BULLET_STATE_FLYING_RIGHT);
			float x1, y1;
			GetPosition(x1, y1);
			bullet->SetPosition(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
			bullet->SetStartPositon(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
			LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(6);
			bullet->SetAnimationSet(ani_set);
			dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects()->push_back(bullet);
		}
	}
	else {
		CBullet* bullet = new CBullet(GetBulletLevel(), BULLET_STATE_FLYING_UP);
		float x1, y1;
		GetPosition(x1, y1);
		bullet->SetPosition(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
		bullet->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(6);
		bullet->SetAnimationSet(ani_set);
		dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects()->push_back(bullet);
	}
}

void CTank::CallSecondWeapon()
{
	vector< LPGAMEOBJECT> *objects = dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects();
	int width4, height4;
	GetDimension(width4, height4);
	if (secondWeapon == WEAPONS_TYPE_ROCKET) {
		if (GetEnableRocket() && NumOfRocketFired < 2) {
			if (height4 == TANK_NORMAL_HEIGHT)
			{
				if (nx == -1)
				{

					CRocket* rocket = new CRocket();
					for (int i = 0; i < objects->size(); i++) {
						if (objects->at(i)->IsEnemy() && GetDistance(objects->at(i)) < 230) {
							rocket->SetTargetObjects(objects);
							rocket->SetTargetObject(objects->at(i));
							float x1, y1;
							GetPosition(x1, y1);
							rocket->SetPosition(x1 - 15, (y1 - ROCKET_BBOX_HEIGHT) - 15);
							LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(8);
							rocket->SetAnimationSet(ani_set);
							objects->push_back(rocket);
							Sound::GetInstance()->Play("FireRocket", 0, 1);
							NumOfRocketFired++;
							break;
						}
					}
					
				}
				else
				{
					CRocket* rocket = new CRocket();
					for (int i = 0; i < objects->size(); i++) {
						if (objects->at(i)->IsEnemy() && GetDistance(objects->at(i)) < 230) {
							rocket->SetTargetObjects(objects);
							rocket->SetTargetObject(objects->at(i));
							float x1, y1;
							GetPosition(x1, y1);
							rocket->SetPosition(x1 + TANK_NORMAL_WIDTH + 5, (y1 - ROCKET_BBOX_HEIGHT) - 10);
							LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(8);
							rocket->SetAnimationSet(ani_set);
							objects->push_back(rocket);
							Sound::GetInstance()->Play("FireRocket", 0, 1);
							NumOfRocketFired++;
							break;
						}
					}
				}
			}
			else {
				CRocket* rocket = new CRocket();
				for (int i = 0; i < objects->size(); i++) {
					if (objects->at(i)->IsEnemy() && GetDistance(objects->at(i)) < 230) {
						rocket->SetTargetObjects(objects);
						rocket->SetTargetObject(objects->at(i));
						float x1, y1;
						GetPosition(x1, y1);
						rocket->SetPosition(x1 + (TANK_UP_GUN_WIDHT - ROCKET_BBOX_WIDTH) / 2, y1 - ROCKET_BBOX_HEIGHT + 8);
						//rocket->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
						LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(8);
						rocket->SetAnimationSet(ani_set);
						objects->push_back(rocket);
						Sound::GetInstance()->Play("FireRocket", 0, 1);
						NumOfRocketFired++;
						break;
					}
				}
			}
			
		}		
	}
	else if (secondWeapon == WEAPONS_TYPE_THUNDER) {
		if (enableThunder) {
			CThunder* thunder = new CThunder();
			thunder->SetPosition(this->x + tank_width / 2, this->y + tank_height); LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(71);
			thunder->SetAnimationSet(ani_set);
			dynamic_cast<CPlayScene*> (CGame::GetInstance()->GetCurrentScene())->GetObjects()->push_back(thunder);
			Sound::GetInstance()->Play("Thunder", 0, 1);
		}
	}
}
