#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"

class CScene
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;
	int scene_width;
	int scene_height;
	bool isLoaded;

public: 
	CScene() {};
	CScene(int id, LPCWSTR filePath);
	bool IsLoaded() { return isLoaded; };
	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
	void GetSceneWidth(int &w);
	void GetSceneHeight(int& h);

};
typedef CScene * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) :CKeyEventHandler() { scence = s; }
	
};