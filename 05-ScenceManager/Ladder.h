#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CLadder : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
public:
	CLadder();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};