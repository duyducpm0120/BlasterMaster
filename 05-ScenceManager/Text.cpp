#include "Text.h"
#include "Game.h"
#include "Animations.h"

Text::Text(int type)
{
	count = 0;
	ani_set = CAnimationSets::GetInstance()->Get(1);
	this->type = type;
}

void Text::Update(DWORD dt)
{
	count++;
}

void Text::Render()
{	
	if (count <= 100) {
		if(count%10 < 5)
			ani_set->at(type)->Render(this->x, this->y,255);
		else
			ani_set->at(type)->Render(this->x, this->y,50);
	}
		
	else
		Text::~Text();
}

Text::~Text()
{
}
