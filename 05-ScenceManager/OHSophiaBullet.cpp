#include "OHSophiaBullet.h"

void COHSophiaBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (type) {
	
		case 
	
	}
}

void COHSophiaBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void COHSophiaBullet::Render()
{
}

void COHSophiaBullet::SetState(int state)
{
}

COHSophiaBullet::COHSophiaBullet(int level, int state, int type): CBullet(level, state)
{
	this->type = type;
	this->level = level;
	damage = level;
	this->state = state;
}



