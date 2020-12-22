#pragma once
#include "GameObject.h"

#define CHOOSE_BBOX_WIDTH  36
#define CHOOSE_BBOX_HEIGHT 16

class CChoose : public CGameObject
{
private:
	int count;
public:
	CChoose();
	virtual void Render();
	void Update(DWORD dt);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};