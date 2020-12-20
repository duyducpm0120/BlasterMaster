#pragma once
#include "GameObject.h"

#define CHOOSE_BBOX_WIDTH  36
#define CHOOSE_BBOX_HEIGHT 16

class CChoose : public CGameObject
{
private:

public:
	CChoose();
	virtual void Render();
	virtual void Update();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};