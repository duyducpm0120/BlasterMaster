#include "Firework.h"
#include "Game.h"
#include "Animations.h"

Firework::Firework()
{
	count = 0;
	ani_set = CAnimationSets::GetInstance()->Get(FIREWORK_ANIMATION_SET_ID);
}

void Firework::Update(DWORD dt)
{
	count++;
}

void Firework::Render()
{
	if (count <= FIREWORK_EXIST_TIME) {
			ani_set->at(0)->Render(this->x, this->y, 50);
	}
	else
		Firework::~Firework();
}

Firework::~Firework()
{
}
