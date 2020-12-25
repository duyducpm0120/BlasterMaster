#pragma once
#include <iostream>
#include <fstream>
#include "IntroScene.h"
#include "Utils.h"
#include "Sound.h"
#include "Textures.h"
#include "Sprites.h"
#define ID_INTRO			12
#define ID_INTROENDING			13

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILE_MAP 7
#define MAX_SCENE_LINE 1024


IntroScene::IntroScene()
{
}

IntroScene::IntroScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	isLoaded = false;
	Load();
	switch (id)
	{
	case ID_INTRO: {
		this->intro_ani_set = CAnimationSets::GetInstance()->Get(Intro_Scene);
		DebugOut(L" \n \n Get ani_set of intro scene succesfully!\n \n \n ");
		Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_INTRO);
		break;
	}
	case ID_INTROENDING: {
		
		this->intro_ani_set = CAnimationSets::GetInstance()->Get(Ending_Scene);
		DebugOut(L" \n \n Get ani_set of intro scene succesfully!\n \n \n ");
		Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_ENDING);

		break;
	}
	default:
		break;
	}
	key_handler = new IntroScenceKeyHandler(this);
}
void IntroScene::Load()
{
	isLoaded = true;
	LoadBaseTextures();
	CGame ::GetInstance()->SetCamPos(0.0f, 0.0f);	//initial camera
}
void IntroScene::LoadBaseTextures()
{
	DebugOut(L"[INFO] Start loading TEXTURES resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);
	
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;				//skip commnet line

		if (line == "[TEXTURES]")								//texture section
		{
			section = SCENE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]")								//sprite section
		{
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]")								//animation section
		{
			
			section = SCENE_SECTION_ANIMATIONS;	
			
			continue;
			
		}
		if (line == "[ANIMATION_SETS]")							//animationset section
		{
			DebugOut(L"[Info] animation set section");
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		DebugOut(L"[Info] animation section %d\n", section);
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading TEXTURES resources %s\n",sceneFilePath);
}


#pragma region keyhandler

void IntroScenceKeyHandler::KeyState(BYTE* states)
{
	if (CGame::GetInstance()->IsKeyDown(DIK_N))
	{
		
	}
}

void IntroScenceKeyHandler::OnKeyDown(int KeyCode)
{
	IntroScene* introScene = dynamic_cast<IntroScene*>(scence);
	if (CGame::GetInstance()->IsKeyDown(DIK_RETURN) && (introScene->GetAnimation() == Intro_Animation_Logo || introScene->GetAnimation() == Intro_Animation_Frog))
		introScene->SetAnimation(Intro_Animation_Car);
}

void IntroScenceKeyHandler::OnKeyUp(int KeyCode)
{
	return;
}
#pragma endregion
#pragma region parseSection
void IntroScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void IntroScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void IntroScene::_ParseSection_ANIMATIONS(string line)
{
	
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time
	
	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}
	
	DebugOut(L"[Info] animation added: %d\n", ani_id);
	CAnimations::GetInstance()->Add(ani_id, ani);
}
void IntroScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
		DebugOut(L"Ani_set push back ani id: %d\n", ani_id);

	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
	DebugOut(L"Added animationset %d \n", ani_set_id);
}

void IntroScene::_ParseSection_CLEARTEXTURES(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	//CTextures::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Texture %d!\n", idClear);
}
void IntroScene::_ParseSection_CLEARSPRITES(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	//CSprites::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Sprite %d!\n", idClear);
}
void IntroScene::_ParseSection_CLEARANIMATIONS(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	//CAnimations::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation %d!\n", idClear);
}
void IntroScene::_ParseSection_CLEARANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	//CAnimationSets::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation Set %d!\n", idClear);
}
void IntroScene::_ParseSection_SCENEFILEPATH(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;
}
#pragma endregion

IntroScene::~IntroScene()
{
}
void IntroScene::Update(DWORD dt)
{
	switch (setAnimation)
	{
	case Intro_Animation_Frog:
	{
		Sound::GetInstance()->Play("Opening", 1, 10000);
		break;
	}
	case Intro_Animation_Car:
	{
		Sound::GetInstance()->Stop("Opening");
		Sound::GetInstance()->Play("CarBackground", 1, 10000);
		if (intro_ani_set->at(Intro_Animation_Car)->GetFrame() == 23)
			Sound::GetInstance()->Play("CarSplash", 0, 1);
		break;
	}
	default:
		break;
	}
	

	if (setAnimation == Intro_Done) {
		Unload();
		CGame::GetInstance()->SwitchScene(1);
	}
	
#pragma region camera
	if (this->moutainY > 40)setEndding = 1;
	if (this->time == 60)setEndding = 2;

	switch (setEndding)
	{
	case 0:
		this->moutainY += 0.1;
		break;
	case 1:
		if (this->posX < 230)this->posX += 1;
		else time++;
		break;
	case 2:
		if(this->textY< 272+326)
			this->textY += 0.5;
		break;
	default:
		break;
	}
	
	
	switch (id)
	{
	case ID_INTRO:
		CGame::GetInstance()->SetCamPos(0,25);
		break;
	case ID_INTROENDING:
		if (setEndding == 0 && this->moutainY < 30)
		{
			CGame::GetInstance()->SetCamPos(this->posX, -10 + 1 * dt);
		}
		else
		{
			CGame::GetInstance()->SetCamPos(this->posX,0);
		}
		break;
	default:
		break;
	}
#pragma endregion

	float x1, y1;
	CGame::GetInstance()->GetCamPosition(x1, y1);
	//DebugOut(L"Camera X: %f \n",x1);
	//DebugOut(L"Camera Y: %f \n", y1);
}


void IntroScene::Render()
{
	int alpha = 255;
	if (this->id == ID_INTRO) {

		switch (setAnimation)
		{
		case Intro_Animation_Logo: 
			intro_ani_set->at(Intro_Animation_Logo)->IntroRender(1, 0, 0,alpha);
			if (intro_ani_set->at(Intro_Animation_Logo)->GetFrame() == intro_ani_set->at(Intro_Animation_Logo)->GetLastFrameIndex()) { 
				setAnimation = Intro_Animation_Frog; 
			} 
			break;
		case Intro_Animation_Frog: 
			intro_ani_set->at(Intro_Animation_Frog)->IntroRender(1, 0, 0, alpha);
			if (intro_ani_set->at(Intro_Animation_Frog)->GetFrame() == intro_ani_set->at(Intro_Animation_Frog)->GetLastFrameIndex()) {
				setAnimation = Intro_Animation_Car; 
			} 
			break;
		case Intro_Animation_Car:
			intro_ani_set->at(Intro_Animation_Car)->IntroRender(1, 0, 0, alpha);
			if (intro_ani_set->at(Intro_Animation_Car)->GetFrame() == intro_ani_set->at(Intro_Animation_Car)->GetLastFrameIndex()) { 
				setAnimation = Intro_Done;
			} 
			break;
		default:
			break;
		}
	}
	else if (this->id == ID_INTROENDING) {
		
		//intro_ani_set->at(Endding_Background1)->IntroRender(1, 0, 0);
		switch (setEndding)
		{
		case 0:
			intro_ani_set->at(Endding_Cloud)->IntroRender(1, 0, 0, alpha);
			intro_ani_set->at(Endding_Mountain)->IntroRender(1, 123, 120 + this->moutainY, alpha);
			intro_ani_set->at(Endding_Forest)->IntroRender(1, 0,116, alpha);
			break;
		case 1:
			intro_ani_set->at(Endding_Background1)->IntroRender(1, 0, 0, alpha);
			intro_ani_set->at(Endding_Frog)->IntroRender(1, 430, 107, alpha);
			intro_ani_set->at(Endding_Hair)->IntroRender(1, 410, 88, alpha);
			break;
		case 2:
			intro_ani_set->at(Endding_Background2)->IntroRender(1, 250, 0, alpha);
			if(this->textY<272+326)
				intro_ani_set->at(Endding_Text1)->IntroRender(1, 360, 272 - this->textY, alpha);
			else
				intro_ani_set->at(Endding_Text2)->IntroRender(1, 360, 0, alpha);
			break;
		default:
			break;
		}	
	}
}

void IntroScene::Unload()
{

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
