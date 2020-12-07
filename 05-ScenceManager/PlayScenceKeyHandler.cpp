#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "PlayScenceKeyHandler.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Tile.h"
#include "Tank.h"
#include "Bullet.h"
#include "Butterfly.h"
#include "Rocket.h"
#include "Sophia.h"
#include "OHSophia.h"
#include "OHSophiaBullet.h"
using namespace std;

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CPlayer* player = (((CPlayScene*)scence)->GetPlayer());
	if (dynamic_cast<CTank*> (player)) {
		switch (KeyCode)
		{
		case DIK_UP:
			player->y += 16.0f;
			player->SetDimension(TANK_NORMAL_WIDTH, TANK_NORMAL_HEIGHT);
			break;
		case DIK_RIGHT:
			if (player->vy <= 0.05f)
				player->SetState(TANK_STATE_IDLE_RIGHT);
			break;
		case DIK_LEFT:
			if (player->vy <= 0.05f)
				player->SetState(TANK_STATE_IDLE_LEFT);
			break;
		}
	}
	else if (dynamic_cast<CSophia*>(player)) {}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CPlayer* player = (((CPlayScene*)scence)->GetPlayer());
	if (dynamic_cast<CTank*> (player)) {
		// disable control key when Mario die 
		if (player->GetState() == TANK_STATE_DIE) return;

		if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_UP)) {
			player->nx = 1;
			if (player->vy >= 0.05f) {
				player->SetState(TANK_STATE_JUMP_RIGHT);
			}
			else
				player->SetState(TANK_STATE_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_UP)) {
			player->nx = -1;
			if (player->vy >= 0.05f) {
				player->SetState(TANK_STATE_JUMP_LEFT);
			}
			else
				player->SetState(TANK_STATE_WALKING_LEFT);
		}
		else if (game->IsKeyDown(DIK_UP)) {
			if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_RIGHT)) {
				player->SetState(TANK_STATE_UP_GUN_WALKING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_LEFT)) {
				player->SetState(TANK_STATE_UP_GUN_WALKING_LEFT);
			}
			else {
				if (player->nx == -1)
					player->SetState(TANK_STATE_UP_GUN_LEFT);
				else
					player->SetState(TANK_STATE_UP_GUN_RIGHT);
			}
		}
		else if (game->IsKeyDown(DIK_SPACE)) {

		}
		else
		{
			if (player->nx == -1)
				player->SetState(TANK_STATE_IDLE_LEFT);
			else
				player->SetState(TANK_STATE_IDLE_RIGHT);

		}
	}
	else if (dynamic_cast<CSophia*> (player)) {
		if (player->visible == false)	return;
		if (game->IsKeyDown(DIK_RIGHT) ) {
			if (!dynamic_cast<CSophia*> (player)->IsClimbing()) {
				player->nx = 1;
				player->SetState(SOPHIA_STATE_WALKING_RIGHT);
			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (!dynamic_cast<CSophia*> (player)->IsClimbing()) {
				player->nx = -1;
				player->SetState(SOPHIA_STATE_WALKING_LEFT);
			}
		}
		else if (game->IsKeyDown(DIK_UP)) {			
			if (dynamic_cast<CSophia*> (player)->IsClimbing()) {
				player->SetState(SOPHIA_STATE_CLIMBING_LADDER);
				dynamic_cast<CSophia*> (player)->vy = -SOPHIA_WALKING_SPEED;
			}
		}
		else if (game->IsKeyDown(DIK_DOWN)) {
			if (dynamic_cast<CSophia*> (player)->IsClimbing()) {
				player->SetState(SOPHIA_STATE_CLIMBING_LADDER);
				dynamic_cast<CSophia*> (player)->vy = SOPHIA_WALKING_SPEED;
			}
		}
		else if (game->IsKeyDown(DIK_SPACE)) {

			//player->vy -= 0.0001f;			

		}		
		else
		{
			if (player->state == SOPHIA_STATE_WALKING_LEFT)
				player->SetState(SOPHIA_STATE_IDLE_LEFT);
			else if (player->state == SOPHIA_STATE_WALKING_RIGHT)
				player->SetState(SOPHIA_STATE_IDLE_RIGHT);
			else if (player->state == SOPHIA_STATE_CLIMBING_LADDER)
				player->SetState(SOPHIA_STATE_CLIMBING_IDLE);
		}
	
	}
	else if (dynamic_cast<COHSophia*>(player)) {
		if (player->visible == false)	return;
		if (game->IsKeyDown(DIK_RIGHT)) {
			player->SetState(OHSOPHIA_STATE_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT) ) {
			player->SetState(OHSOPHIA_STATE_WALKING_LEFT);
		}
		else if (game->IsKeyDown(DIK_UP)) {
			player->SetState(OHSOPHIA_STATE_WALKING_UP);
		}
		else if (game->IsKeyDown(DIK_DOWN)) {
			player->SetState(OHSOPHIA_STATE_WALKING_DOWN);
		}
		else
		{
			if (player->state == OHSOPHIA_STATE_WALKING_UP)
				player->SetState(OHSOPHIA_STATE_IDLE_UP);
			else if(player->state == OHSOPHIA_STATE_WALKING_DOWN)
				player->SetState(OHSOPHIA_STATE_IDLE_DOWN);
			else if(player->state == OHSOPHIA_STATE_WALKING_LEFT)
				player->SetState(OHSOPHIA_STATE_IDLE_LEFT);
			else if(player->state == OHSOPHIA_STATE_WALKING_RIGHT)
				player->SetState(OHSOPHIA_STATE_IDLE_RIGHT);
		}	
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CTank* tank = new CTank();
	CSophia* sophia = new CSophia();
	CPlayer* player = ((CPlayScene*)scence)->GetPlayer();
	vector<LPGAMEOBJECT>* objects = ((CPlayScene*)scence)->GetObjects();
	vector<LPGAMEOBJECT>* updateObjects = ((CPlayScene*)scence)->GetUpdateObjects();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = objects->at(0);
	switch (KeyCode)
	{
	case DIK_1:
		objects->clear();
		CGame::GetInstance()->SwitchScene(1);
		break;
	case DIK_2:
		objects->clear();
		CGame::GetInstance()->SwitchScene(2);
		break;
	case DIK_3:
		objects->clear();
		CGame::GetInstance()->SwitchScene(3);
		break;
	case DIK_4:
		objects->clear();
		CGame::GetInstance()->SwitchScene(4);
		break;
	case DIK_5:
		objects->clear();
		CGame::GetInstance()->SwitchScene(5);
		break;
	case DIK_6:
		objects->clear();
		CGame::GetInstance()->SwitchScene(6);
		break;
	case DIK_7:
		objects->clear();
		CGame::GetInstance()->SwitchScene(7);
		break;
	case DIK_8:
		objects->clear();
		CGame::GetInstance()->SwitchScene(8);
		break;
	}		// Switch scenes	 


	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	
	if (dynamic_cast<CTank*>(player)) {				 // if player is tank
		switch (KeyCode)
		{
		case DIK_W:
			if (dynamic_cast<CTank*> (player))
			{
				
				sophia->health =player->GetHealth();
				sophia->damage =player->GetDamage();
				sophia->SetPosition(dynamic_cast<CTank*> (((CPlayScene*)scence)->GetPlayer())->x + TANK_NORMAL_WIDTH/2 - SOPHIA_BBOX_WIDTH, dynamic_cast<CTank*> (((CPlayScene*)scence)->GetPlayer())->y - 5);
				LPANIMATION_SET ani_set = animation_sets->Get(15);

				sophia->SetAnimationSet(ani_set);
	
				//dynamic_cast<CTank*> (((CPlayScene*)scence)->GetPlayer())->visible = false;
				
				if (((CPlayScene*)scence)->GetPlayer()->nx == -1)
					((CPlayScene*)scence)->GetPlayer()->SetState(TANK_STATE_IDLE_LEFT);
				else
					((CPlayScene*)scence)->GetPlayer()->SetState(TANK_STATE_IDLE_RIGHT);
				((CPlayScene*)scence)->SetPlayer(sophia);
				((CPlayScene*)scence)->SetHUD(new HUD(player->GetHealth(), player->GetDamage()));
				objects->push_back(sophia);
			}
			break;

		case DIK_Z:
			int width3, height3;
			player->GetDimension(width3, height3);
			if (height3 == TANK_NORMAL_HEIGHT)
			{
				if (player->nx == -1)
				{

					CBullet* bullet = new CBullet(player->GetBulletLevel(), BULLET_STATE_FLYING_LEFT);
					float x1, y1;
					player->GetPosition(x1, y1);
					bullet->SetPosition(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
					bullet->SetStartPositon(x1 - BULLET_HORIZONTAL_BBOX_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
					LPANIMATION_SET ani_set = animation_sets->Get(6);
					bullet->SetAnimationSet(ani_set);
					objects->push_back(bullet);
				}
				else
				{
					CBullet* bullet = new CBullet(player->GetBulletLevel(), BULLET_STATE_FLYING_RIGHT);
					float x1, y1;
					player->GetPosition(x1, y1);
					bullet->SetPosition(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
					bullet->SetStartPositon(x1 + TANK_NORMAL_WIDTH, y1 + TANK_NORMAL_HEIGHT / 2 - 8);
					LPANIMATION_SET ani_set = animation_sets->Get(6);
					bullet->SetAnimationSet(ani_set);
					objects->push_back(bullet);
				}
			}
			else {
				CBullet* bullet = new CBullet(player->GetBulletLevel(), BULLET_STATE_FLYING_UP);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
				bullet->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}

			break;
		case DIK_V:
			int width4, height4;
			player->GetDimension(width4, height4);
			if (player->GetEnableRocket()) {
				if (height4 == TANK_NORMAL_HEIGHT)
				{
					if (player->nx == -1)
					{

						CRocket* rocket = new CRocket();
						for (int i = 0; i < objects->size(); i++) {
							if (objects->at(i)->IsEnemy() && player->GetDistance(objects->at(i)) < 230) {
								rocket->SetTargetObjects(((CPlayScene*)scence)->GetObjects());
								rocket->SetTargetObject(objects->at(i));
								float x1, y1;
								player->GetPosition(x1, y1);
								rocket->SetPosition(x1 - 15, (y1 - ROCKET_BBOX_HEIGHT) - 15);
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
							if (objects->at(i)->IsEnemy() && player->GetDistance(objects->at(i)) < 230) {
								rocket->SetTargetObjects(((CPlayScene*)scence)->GetObjects());
								rocket->SetTargetObject(objects->at(i));
								float x1, y1;
								player->GetPosition(x1, y1);
								rocket->SetPosition(x1 + TANK_NORMAL_WIDTH + 5, (y1 - ROCKET_BBOX_HEIGHT) - 10);
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
						if (objects->at(i)->IsEnemy() && player->GetDistance(objects->at(i)) < 230) {
							rocket->SetTargetObjects(((CPlayScene*)scence)->GetObjects());
							rocket->SetTargetObject(objects->at(i));
							float x1, y1;
							player->GetPosition(x1, y1);
							rocket->SetPosition(x1 + (TANK_UP_GUN_WIDHT - ROCKET_BBOX_WIDTH) / 2, y1 - ROCKET_BBOX_HEIGHT + 8);
							//rocket->SetStartPositon(x1 + (TANK_UP_GUN_WIDHT - BULLET_VERTICAL_BBOX_WIDTH) / 2, y1 - BULLET_VERTICAL_BBOX_HEIGHT + 8);
							LPANIMATION_SET ani_set = animation_sets->Get(8);
							rocket->SetAnimationSet(ani_set);
							objects->push_back(rocket);
							break;
						}
					}
				}
			}
			break;
		case DIK_SPACE:
			if (player->vy <= 0.05f && player->vy >= 0) {
				if (player->nx == -1)
					player->SetState(TANK_STATE_JUMP_IDLE_LEFT);
				else
					player->SetState(TANK_STATE_JUMP_IDLE_RIGHT);
			}
			break;
		case DIK_A:
			player->SetState(TANK_STATE_IDLE_RIGHT);
			player->visible = true;
			player->SetPosition(100.0f, 0.0f);
			player->SetSpeed(0, 0);
			break;
		case DIK_UP:
			int width2, height2;
			player->GetDimension(width2, height2);
			if (height2 == TANK_NORMAL_HEIGHT)
				player->y -= 16.0f;
			player->SetDimension(TANK_UP_GUN_WIDHT, TANK_UP_GUN_HEIGHT);
			if (player->nx == -1)
				player->SetState(TANK_STATE_UPING_GUN_LEFT);
			else
				player->SetState(TANK_STATE_UPING_GUN_RIGHT);
			break;
		}
	}	

	else if (dynamic_cast<CSophia*>(player)) {
		switch (KeyCode)
		{	
		case DIK_SPACE:
			if(player->vy >= 0.0f && player->vy <= 0.35f)
				player->SetState(SOPHIA_STATE_JUMP);
			if (dynamic_cast<CSophia*> (player)->IsClimbing() == true)
				dynamic_cast<CSophia*> (player)->SetIsClimbing(false);
			break;
		case DIK_W:
			if (dynamic_cast<CSophia*> (player)->IsTouchTank())
			{
				tank = ((CPlayScene*)scence)->GetTank();
				tank->health = player->GetHealth();
				tank->damage = player->GetDamage();
				((CPlayScene*)scence)->GetPlayer()->vy -= 0.01;
				//dynamic_cast<CTank*> (((CPlayScene*)scence)->GetPlayer())->visible = false;
				((CPlayScene*)scence)->SetPlayer(&(*tank));
				
				((CPlayScene*)scence)->SetHUD(new HUD(player->GetHealth(), player->GetDamage()));
				
			}
			
			if (dynamic_cast<CSophia*> (player)->IsTouchLadder() && dynamic_cast<CSophia*> (player)->IsClimbing() == false) {
				float x, y;
				dynamic_cast<CSophia*> (player)->GetClimbingPosition(x, y);
				player->SetPosition(x, y);
				dynamic_cast<CSophia*> (player)->SetState(SOPHIA_STATE_CLIMBING_IDLE);
			}
			else if (dynamic_cast<CSophia*> (player)->IsClimbing() == true) {
				dynamic_cast<CSophia*> (player)->SetIsClimbing(false);
				float x = player->x + 13;
				float y = player->y - 2;
				player->SetPosition(x, y);
				player->SetState(SOPHIA_STATE_IDLE_RIGHT);
			}
			break;
		}

	} 
	else if (dynamic_cast<COHSophia*>(player)) {
		switch (KeyCode)
		{
		case DIK_Z:
			if (player->state == OHSOPHIA_STATE_WALKING_LEFT || player->state == OHSOPHIA_STATE_IDLE_LEFT)
			{
				COHSophiaBullet* bullet;

				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetStartPositon(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);

			}
			else if (player->state == OHSOPHIA_STATE_WALKING_RIGHT || player->state == OHSOPHIA_STATE_IDLE_RIGHT)
			{
				COHSophiaBullet* bullet;
				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);

			}
			else if (player->state == OHSOPHIA_STATE_WALKING_UP || player->state == OHSOPHIA_STATE_IDLE_UP) {

				COHSophiaBullet* bullet;

				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);

			}
			else if (player->state == OHSOPHIA_STATE_WALKING_DOWN || player->state == OHSOPHIA_STATE_IDLE_DOWN) {
				COHSophiaBullet* bullet;

				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
			break;
		case DIK_X:
			if (player->state == OHSOPHIA_STATE_WALKING_LEFT || player->state == OHSOPHIA_STATE_IDLE_LEFT)
			{
				COHSophiaBullet* bullet;
				for (int i = 0; i < 3; i++) {

					if (i == 0) {
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
					}
					else if (i == 1)
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_ROUNDED, 1);
					else
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_ROUNDED, -1);
					float x1, y1;
					player->GetPosition(x1, y1);
					bullet->SetPosition(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
					bullet->SetStartPositon(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
					bullet->SetAnchorPoint();
					LPANIMATION_SET ani_set = animation_sets->Get(21);
					bullet->SetAnimationSet(ani_set);
					objects->push_back(bullet);
				}
			}
			else if (player->state == OHSOPHIA_STATE_WALKING_RIGHT || player->state == OHSOPHIA_STATE_IDLE_RIGHT)
			{
				COHSophiaBullet* bullet;
				for (int i = 0; i < 3; i++) {

					if (i == 0) {
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
					}
					else if (i == 1)
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_ROUNDED, 1);
					else
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_ROUNDED, -1);
					float x1, y1;
					player->GetPosition(x1, y1);
					bullet->SetPosition(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
					bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
					bullet->SetAnchorPoint();
					LPANIMATION_SET ani_set = animation_sets->Get(21);
					bullet->SetAnimationSet(ani_set);
					objects->push_back(bullet);
				}
			}
			else if (player->state == OHSOPHIA_STATE_WALKING_UP || player->state == OHSOPHIA_STATE_IDLE_UP) {

				COHSophiaBullet* bullet;
				for (int i = 0; i < 3; i++) {

					if (i == 0) {
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
					}
					else if (i == 1)
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_ROUNDED, 1);
					else
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_ROUNDED, -1);
					float x1, y1;
					player->GetPosition(x1, y1);
					bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1);
					bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1);
					bullet->SetAnchorPoint();
					LPANIMATION_SET ani_set = animation_sets->Get(21);
					bullet->SetAnimationSet(ani_set);
					objects->push_back(bullet);
				}
			}
			else if (player->state == OHSOPHIA_STATE_WALKING_DOWN || player->state == OHSOPHIA_STATE_IDLE_DOWN) {
				COHSophiaBullet* bullet;
				for (int i = 0; i < 3; i++) {

					if (i == 0) {
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_STRAIGHT, 1);
					}
					else if (i == 1)
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_ROUNDED, 1);
					else
						bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_ROUNDED, -1);
					float x1, y1;
					player->GetPosition(x1, y1);
					bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
					bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
					bullet->SetAnchorPoint();
					LPANIMATION_SET ani_set = animation_sets->Get(21);
					bullet->SetAnimationSet(ani_set);
					objects->push_back(bullet);
				}

			}
			break;
		case DIK_C:
			if (player->state == OHSOPHIA_STATE_WALKING_LEFT || player->state == OHSOPHIA_STATE_IDLE_LEFT)
			{
				COHSophiaBullet* bullet;

				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_LEFT, OHSOPHIABULLET_TYPE_SIN, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetStartPositon(x1 - OHSOPHIABULLET_BBOX_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);

			}
			else if (player->state == OHSOPHIA_STATE_WALKING_RIGHT || player->state == OHSOPHIA_STATE_IDLE_RIGHT)
			{
				COHSophiaBullet* bullet;
				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_RIGHT, OHSOPHIABULLET_TYPE_SIN, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH, y1 + OHSOPHIA_HEIGHT / 2 - 2);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);

			}
			else if (player->state == OHSOPHIA_STATE_WALKING_UP || player->state == OHSOPHIA_STATE_IDLE_UP) {

				COHSophiaBullet* bullet;

				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_UP, OHSOPHIABULLET_TYPE_SIN, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);

			}
			else if (player->state == OHSOPHIA_STATE_WALKING_DOWN || player->state == OHSOPHIA_STATE_IDLE_DOWN) {
				COHSophiaBullet* bullet;

				bullet = new COHSophiaBullet(player->GetBulletLevel(), OHSOPHIABULLET_STATE_FLYING_DOWN, OHSOPHIABULLET_TYPE_SIN, 1);
				float x1, y1;
				player->GetPosition(x1, y1);
				bullet->SetPosition(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
				bullet->SetStartPositon(x1 + OHSOPHIA_WIDTH / 4, y1 + OHSOPHIA_HEIGHT);
				bullet->SetAnchorPoint();
				LPANIMATION_SET ani_set = animation_sets->Get(21);
				bullet->SetAnimationSet(ani_set);
				objects->push_back(bullet);
			}
			break;

		}
		
	}
}