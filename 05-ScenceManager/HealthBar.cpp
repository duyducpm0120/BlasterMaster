#include "HealthBar.h"

HealthBar::HealthBar(int maxHealth, int isGun)
{
	maxPlayerHealth = maxHealth;
	currentPlayerHealth = maxHealth;
	this->isGun = isGun;

	for (int i = 0; i < currentPlayerHealth; i++)
	{
		LPANIMATION_SET aniSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
		health.push_back(aniSet);
	}
}

HealthBar::~HealthBar()
{}

void HealthBar::Update(int currentHealth, float X, float Y)
{
	currentPlayerHealth = currentHealth;
	if (currentHealth <= 0) return;
	x = X;
	y = Y;
}

void HealthBar::Render()
{
	if (currentPlayerHealth <= 0) return;
	else
	{
		for (int i = 0; i < currentPlayerHealth; i++)
		{
			if (isGun)
			{
				health[i]->at(HEALTH_TYPE_GUN_UNIT)->Render(x-2, y + 56 - i * HEALTH_SPACE_UNIT,255);
			}
			else
			{
				health[i]->at(HEALTH_TYPE_PLAYER_UNIT)->Render( x-2, y + 56 - i * HEALTH_SPACE_UNIT,255);
			}
		}
	}
}
