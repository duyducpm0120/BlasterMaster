#pragma once
#include "Animations.h"

#include <vector>

#define HEALTH_TYPE_GUN_NULL			0
#define HEALTH_TYPE_GUN_UNIT			1
#define HEALTH_TYPE_PLAYER_NULL			2
#define HEALTH_TYPE_PLAYER_UNIT			3

#define HEALTH_SPACE_UNIT				8

#define ANIMATION_SET_PLAYERHP	10

class HealthBar
{
public:
	float x, y;
	int currentPlayerHealth, maxPlayerHealth;
	std::vector<LPANIMATION_SET> health;
	bool isGun;
		
	HealthBar(int maxHealth, int isGun);
	~HealthBar();
	void Update(int currentHealth, float X, float Y);
	void Render();

	void SetPosition(float X, float Y) { x = X; y = Y; }
	float Getx() { return x; }
	float Gety() { return y; }
	void ReceivePos(float& x, float& y) { x = this->x; y = this->y; }
	void Setx(float X) { this->x = X; }
	void Sety(float Y) { this->y = Y; }
};

