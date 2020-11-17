#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Tile.h"
#include "Tank.h"
#include "Bullet.h"
#include "Butterfly.h"
#include "PlayScenceKeyHandler.h"
#include "Rocket.h"
using namespace std;

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CTank* tank = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_UP:
		tank->y += 16.0f;
		tank->SetDimension(TANK_NORMAL_WIDTH, TANK_NORMAL_HEIGHT);
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CTank* tank = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (tank->GetState() == TANK_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_UP)) {
		tank->nx = 1;
		if (tank->vy >= 0.05f) {
				tank->SetState(TANK_STATE_JUMP_RIGHT);
		}
		else 
			tank->SetState(TANK_STATE_WALKING_RIGHT);		
	}
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_UP)) {
		tank->nx = -1;
		if (tank->vy >= 0.05f) {
				tank->SetState(TANK_STATE_JUMP_LEFT);
		}
		else
			tank->SetState(TANK_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_UP)) {
		if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_RIGHT)) {
			tank->SetState(TANK_STATE_UP_GUN_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_LEFT)) {
			tank->SetState(TANK_STATE_UP_GUN_WALKING_LEFT);
		}
		else {
			if (tank->nx == -1)
				tank->SetState(TANK_STATE_UP_GUN_LEFT);
			else
				tank->SetState(TANK_STATE_UP_GUN_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_SPACE)) {
		//if(tank->vy == )
	}
	else
	{	
		if (tank->nx == -1)
			tank->SetState(TANK_STATE_IDLE_LEFT);
		else
			tank->SetState(TANK_STATE_IDLE_RIGHT);

	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	//CMario mario = ((CPlayScene)scence)->GetPlayer();
	CTank* tank = ((CPlayScene*)scence)->GetPlayer();
	vector<LPGAMEOBJECT> *objects = ((CPlayScene*)scence)->GetObjects();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = objects->at(0);
	switch (KeyCode)
	{
	case DIK_X: 		
		int width1, height1;
		tank->GetDimension(width1, height1);
		if (height1 == TANK_NORMAL_HEIGHT)
		{
			if (tank->nx == -1)
			{

				CBullet* bullet = new CBullet(2, BULLET_STATE_FLYING_LEFT);
				float x1, y1;
				tank->GetPosition(x1, y1);
				bullet->SetPosition(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				bullet->SetStartPositon(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
			else
			{
				CBullet* bullet = new CBullet(2, BULLET_STATE_FLYING_RIGHT);
				float x1, y1;
				tank->GetPosition(x1, y1);
				bullet->SetPosition(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				bullet->SetStartPositon(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
		}
		else {
			CBullet* bullet = new CBullet(2, BULLET_STATE_FLYING_UP);
			float x1, y1;
			tank->GetPosition(x1, y1);
			bullet->SetPosition(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
			bullet->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
			LPANIMATION_SET ani_set = animation_sets->Get(6);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);
		}

		break;
	case DIK_C:
		int width3, height3;
		tank->GetDimension(width3, height3);
		if (height3 == TANK_NORMAL_HEIGHT)
		{
			if (tank->nx == -1)
			{

				CBullet* bullet = new CBullet(1, BULLET_STATE_FLYING_LEFT);
				float x1, y1;
				tank->GetPosition(x1, y1);
				bullet->SetPosition(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				bullet->SetStartPositon(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
			else
			{
				CBullet* bullet = new CBullet(1, BULLET_STATE_FLYING_RIGHT);
				float x1, y1;
				tank->GetPosition(x1, y1);
				bullet->SetPosition(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				bullet->SetStartPositon(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
		}
		else {
			CBullet* bullet = new CBullet(1, BULLET_STATE_FLYING_UP);
			float x1, y1;
			tank->GetPosition(x1, y1);
			bullet->SetPosition(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
			bullet->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
			LPANIMATION_SET ani_set = animation_sets->Get(6);
			bullet->SetAnimationSet(ani_set);
			objects->push_back(bullet);
		}

		break;
	case DIK_V:
		int width4, height4;
		tank->GetDimension(width4, height4);
		if (height4 == TANK_NORMAL_HEIGHT)
		{
			if (tank->nx == -1)
			{

				CRocket* rocket = new CRocket();
				for (int i = 0; i < objects->size(); i++) {
					if (objects->at(i)->IsEnemy()) {
						rocket->SetTargetObjects(((CPlayScene*)scence)->GetObjects());
						float x1, y1;
						tank->GetPosition(x1, y1);
						rocket->SetPosition(x1 - ROCKET_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
						LPANIMATION_SET ani_set = animation_sets->Get(8);
						rocket->SetAnimationSet(ani_set);
						objects->push_back(rocket);
						break;
					}
				}
			}
			else
			{
				CRocket* rocket = new CRocket();
				for (int i = 0; i < objects->size(); i++) {
					if (objects->at(i)->IsEnemy()) {
						rocket->SetTargetObjects(((CPlayScene*)scence)->GetObjects());
						float x1, y1;
						tank->GetPosition(x1, y1);
						rocket->SetPosition(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
						//bullet->SetStartPositon(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
						LPANIMATION_SET ani_set = animation_sets->Get(8);
						rocket->SetAnimationSet(ani_set);
						objects->push_back(rocket);
						break;
					}
				}
				
			}
		}
		else {
			CRocket* rocket = new CRocket();
			for (int i = 0; i < objects->size(); i++) {
				if (objects->at(i)->IsEnemy()) {
					rocket->SetTargetObjects(((CPlayScene*)scence)->GetObjects());
					float x1, y1;
					tank->GetPosition(x1, y1);
					rocket->SetPosition(x1 + (TANK_UP_GUN_WIDHT - ROCKET_BBOX_WIDTH) / 2, y1 - ROCKET_BBOX_HEIGHT + 8);
					//rocket->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
					LPANIMATION_SET ani_set = animation_sets->Get(8);
					rocket->SetAnimationSet(ani_set);
					objects->push_back(rocket);
					break;
				}
			}
		}
		break;
	case DIK_SPACE:
		if (tank->vy <= 0.05f && tank->vy >= 0) {
			if (tank->nx == -1)
				tank->SetState(TANK_STATE_JUMP_IDLE_LEFT);
			else
				tank->SetState(TANK_STATE_JUMP_IDLE_RIGHT);
		}
		break;
	case DIK_A:
		tank->SetState(TANK_STATE_IDLE_RIGHT);
		tank->SetPosition(100.0f, 0.0f);
		tank->SetSpeed(0, 0);
		break;
	case DIK_UP:
		int width2, height2;
		tank->GetDimension(width2, height2);
		if (height2 == TANK_NORMAL_HEIGHT)
			tank->y -= 16.0f;
		tank->SetDimension(TANK_UP_GUN_WIDHT, TANK_UP_GUN_HEIGHT);
		if (tank->nx == -1)
			tank->SetState(TANK_STATE_UPING_GUN_LEFT);
		else
			tank->SetState(TANK_STATE_UPING_GUN_RIGHT);
		break;
	}
}