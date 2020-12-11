#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CAutoRunPortal : public CGameObject
{

	int width;
	int height;
	float targetX, targetY;
public:
	CAutoRunPortal(float l, float t, float r, float b, float targetX,float targetY);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void GetMovingSpace(float& moveX, float& moveY) { moveX = targetX - x; moveY = targetY - y; };
};