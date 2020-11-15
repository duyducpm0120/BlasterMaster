#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	switch (id)
	{
	case 1:
		scene_width = 1024;
		scene_height = 238;
		break;
	case 2:
		scene_width = 513;
		scene_height = 1277;
		break;
	default:
		break;
	}

}

void CScene::GetSceneWidth(int& w)
{
	w = this->scene_width;
}

void CScene::GetSceneHeight(int& h)
{
	h = this->scene_height;
}
