#pragma once
#include "GameObject.h"
#include "HealthBar.h"
#include "Tank.h"
#include "Player.h"
using namespace std;

//#define MAX_TEXTLENGHT_SCORE					6
//#define MAX_TEXTLENGHT_REMAININGTIME			4
//#define MAX_TEXTLENGHT_STAGE					2
//#define MAX_TEXTLENGHT_MANA						2
//#define MAX_TEXTLENGHT_LIVE						2

#define ARTICULAR_GUNPOWER_HEALTHBAR_Y					5
#define ARTICULAR_PLAYER_HEALTHBAR_Y				110

class HUD
{
	float x, y;

	LPANIMATION_SET UIanimationSet;

	HealthBar* playerHB;
	HealthBar* gunHB;

public:
	HUD(int initPlayerHealth = 8, int initGunHealth = 8);
	~HUD();

	void Update(float x, float y, int currentPlayerHealth, int currentGunHealth);
	void Render(CPlayer* playerInfo);
};