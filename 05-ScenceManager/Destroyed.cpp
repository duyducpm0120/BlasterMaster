#include "Destroyed.h"
CDestroyed::CDestroyed(int type) :CGameObject()
{
	this->type = type;
	count = 0;
	switch (type) {
	case DESTROYED_TYPE_BULLET:	
		width = DESTROYED_BULLET_BBOX_WIDTH;
		height = DESTROYED_BULLET_BBOX_HEIGHT;
		break;
	case DESTROYED_TYPE_OBJECT:
		width = DESTROYED_OBJECT_BBOX_WIDTH;
		height = DESTROYED_OBJECT_BBOX_HEIGHT;
		break;
	case DESTROYED_TYPE_TANK:
		width = DESTROYED_TANK_BBOX_WIDTH;
		height = DESTROYED_TANK_BBOX_HEIGHT;
		break;
	case DESTROYED_TYPE_OHSOPHIA:
		width = DESTROYED_OHSOPHIA_BBOX_WIDTH;
		height = DESTROYED_OHSOPHIA_BBOX_HEIGHT;
		break;
	case DESTROYED_TYPE_SOPHIA:
		width = DESTROYED_SOPHIA_BBOX_WIDTH;
		height = DESTROYED_SOPHIA_BBOX_HEIGHT;
		break;
	}
}


void CDestroyed::Render()
{
	int ani ;
	switch (type) {
	case DESTROYED_TYPE_BULLET:
		ani = DESTROYED_ANI_BULLET;
		break;
	case DESTROYED_TYPE_OBJECT:
		ani = DESTROYED_ANI_OBJECT;
		break;
	case DESTROYED_TYPE_TANK:
		ani = DESTROYED_ANI_TANK;
		break;
	case DESTROYED_TYPE_SOPHIA:
		ani = DESTROYED_ANI_SOPHIA;
		break;
	case DESTROYED_TYPE_OHSOPHIA:
		ani = DESTROYED_ANI_OHSOPHIA;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	count++;
	if (type != DESTROYED_TYPE_SOPHIA && type != DESTROYED_TYPE_OHSOPHIA) {
		if (count == 5)
			visible = false;
	}
	else if (type == DESTROYED_TYPE_SOPHIA){
		if (count == 50)
			visible = false;
	}
	else if (type == DESTROYED_TYPE_OHSOPHIA) {
		if(animation_set->at(4)->GetFrame()==3)
			visible = false;
	}
	//RenderBoundingBox();
}

void CDestroyed::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CDestroyed::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + DESTROYED_BULLET_BBOX_WIDTH;
	b = y + DESTROYED_BULLET_BBOX_HEIGHT;
}