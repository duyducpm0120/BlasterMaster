#pragma once
#include "Animations.h"

#define TEXT_TYPE_ROCKETUNLOCKED	0
#define TEXT_TYPE_THUNDERUNLOCKED	1



#define TEXT_ANI_ROCKETUNLOCKED	0
#define TEXT_ANI_THUNDERUNLOCKED	1

#define TEXT_EXIST_TIME	50

class Text
{
	float x, y;
	int type;
	int width;
	int height;
	DWORD dt;
	int ani;
	LPANIMATION_SET ani_set;
	int count;
public:
	void Setposition(float x, float y) { this->x = x; this->y = y; };
	Text() = default;
	Text(int type);
	void Update(DWORD dt);
	void Render();
	~Text();
};

