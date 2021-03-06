#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	isLoaded = false;
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
	case 3:
		scene_width = 513;
		scene_height = 270;
		break;
	case 4:
		scene_width = 512;
		scene_height = 1008;
		break;
	case 5:
		scene_width = 513;
		scene_height = 1759;
		break;
	case 6:
		scene_width = 505;
		scene_height = 238;
		break;
	case 7:
		scene_width = 513;
		scene_height = 525;
		break;
	case 8:
		scene_width = 496;
		scene_height = 238;
		break;
	case 9:
		scene_width = 256;
		scene_height = 512;
		break;
	case 10:
		scene_width = 1024;
		scene_height = 1568;
		break;
	default:
		scene_width = 256;
		scene_height = 256;
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
