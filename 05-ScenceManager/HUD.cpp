#include "HUD.h"

HUD::HUD(int initPlayerHealth, int initGunHealth)
{
	UIanimationSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
	gunHB = new HealthBar(initGunHealth, true);
	playerHB = new HealthBar(initPlayerHealth, false);
}

HUD::~HUD()
{
}

void HUD::Update(float x, float y, int currentPlayerHealth, int currentGunHealth)
{
	this->x = x;
	this->y = y;
	gunHB = new HealthBar(currentGunHealth, true);
	playerHB = new HealthBar(currentPlayerHealth, false);

	gunHB->Update(currentGunHealth, x + 4, y + 7.8); // Hard code -> Dieu chinh khoang cach cac unit mau cua Gun #LKP
	playerHB->Update(currentPlayerHealth, x + 3, y + 112.5); // Hard code -> Dieu chinh khoang cach cac unit mau cua Player #LKP
}

void HUD::Render(CTank* playerInfo)
{
	UIanimationSet->at(HEALTH_TYPE_GUN_NULL)->Render(x, y + ARTICULAR_GUNPOWER_HEALTHBAR_Y,255);
	UIanimationSet->at(HEALTH_TYPE_PLAYER_NULL)->Render(x, y + ARTICULAR_PLAYER_HEALTHBAR_Y,255);
	gunHB->Render();
	playerHB->Render();
}
