#pragma once
#include "GameObject.h"

#define FLAME_BBOX_WIDTH  16
#define FLAME_BBOX_HEIGHT 16

class CFlame : public CGameObject
{
private:

public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};