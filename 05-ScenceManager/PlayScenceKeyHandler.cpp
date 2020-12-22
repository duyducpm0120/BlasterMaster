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
#include "Choose.h"
#include "Sound.h"
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
	CGame* game = CGame::GetInstance();
	switch (KeyCode)
	{
	case DIK_1:		
		CGame::GetInstance()->SwitchScene(1);
		break;
	case DIK_2:		
		CGame::GetInstance()->SwitchScene(2);
		break;
	case DIK_3:		
		CGame::GetInstance()->SwitchScene(3);
		break;
	case DIK_4:		
		CGame::GetInstance()->SwitchScene(4);
		break;
	case DIK_5:		
		CGame::GetInstance()->SwitchScene(5);
		break;
	case DIK_6:	
		CGame::GetInstance()->SwitchScene(6);
		break;
	case DIK_7:		
		CGame::GetInstance()->SwitchScene(7);
		break;
	case DIK_8:		
		CGame::GetInstance()->SwitchScene(8);
		break;
	case DIK_9:
		CGame::GetInstance()->SwitchScene(9);
		break;
	case DIK_0:
		CGame::GetInstance()->SwitchScene(10);
		break;
	case DIK_I:
		CGame::GetInstance()->SwitchScene(12);
		break;
	}		// Switch scenes	 

	if (game->GetCurrentSceneId() == 11) {
		CChoose* choose = dynamic_cast<CPlayScene*> (game->GetCurrentScene())->GetChoose();
		switch (KeyCode) {
		case DIK_RIGHT:
			if (choose->x < 142)
				choose->x += 32;
			else
				choose->x = 78;
			break;
		case DIK_LEFT:
			if (choose->x > 78)
				choose->x -= 32;
			else
				choose->x = 142;
			break;
		}
	}

	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (KeyCode) {
	case DIK_RETURN:
		game->SwitchToSelectWeaponScene();
		break;
	}

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
			dynamic_cast<CTank*>(player)->Shot();
			break;
		case DIK_V:
			dynamic_cast<CTank*>(player)->CallSecondWeapon();
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
			break;
		case DIK_E:
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