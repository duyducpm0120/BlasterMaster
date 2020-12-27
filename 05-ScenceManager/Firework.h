#pragma once
#include "Animations.h"


#define FIREWORK_ANI	0


#define FIREWORK_EXIST_TIME	2000
#define FIREWORK_ANIMATION_SET_ID	22

class Firework
{
	float x, y;
	int width;
	int height;
	DWORD dt;
	int ani;
	LPANIMATION_SET ani_set;
	int count;
public:
	void Setposition(float x, float y) { this->x = x; this->y = y; };
	Firework();
	void Update(DWORD dt);
	void Render();
	~Firework();
};

