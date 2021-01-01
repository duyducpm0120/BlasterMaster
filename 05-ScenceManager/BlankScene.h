#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "Scence.h"
#include "Game.h"
#include "Utils.h"
#define BLANK_SCENE_EXIST_TIME	10

class BlankScene : public CScene
{
private:
	int ExistTime;
	int nextSceneId;
	int lastSceneId;
public:

	BlankScene();
	BlankScene(int id, LPCWSTR filePath);
	virtual void Update(DWORD dt);
	virtual void Render();
	void Load();
	void Unload();
	void SetNextSceneId(int nextSceneId) { this->nextSceneId = nextSceneId;	};
	void SetLastSceneId(int lastSceneId) { this->lastSceneId = lastSceneId; };
};

