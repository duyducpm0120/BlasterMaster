#include "BlankScene.h"

BlankScene::BlankScene()
{
	
}

BlankScene::BlankScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	ExistTime = 0;
	lastSceneId = 1;
	nextSceneId = 2;
}

void BlankScene::Update(DWORD dt)
{
	if (nextSceneId != 14) {
		ExistTime++;
		if (ExistTime == BLANK_SCENE_EXIST_TIME) {
			ExistTime = 0;
			CGame::GetInstance()->SetCurrentSceneId(lastSceneId);
			CGame::GetInstance()->SwitchScene(nextSceneId);
		}
	}
}

void BlankScene::Render()
{
}

void BlankScene::Load()
{
}

void BlankScene::Unload()
{
}
