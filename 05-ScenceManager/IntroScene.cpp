#pragma once
#include "IntroScene.h"
#include "Utils.h"

#define ID_INTRO			21
#define ID_INTROENDING			22

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define MAX_SCENE_LINE 1024


IntroScene::IntroScene()
{
}

IntroScene::IntroScene(int _idStage) : CScene()
{
	idStage = _idStage;
	key_handler = new IntroScenceKeyHandler(this);
	CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	Load();
	switch (idStage)
	{
	case ID_INTRO: {
		//new IntroScene(ID_INTRO) ->intro
		//new IntroScene(ID_INTROENDIING) ->ending
		intro_ani_set = CAnimationSets::GetInstance()->Get(Intro_Scene);
		//Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_INTRO);
		break;
	}
	case ID_INTROENDING: {
		
		intro_ani_set = CAnimationSets::GetInstance()->Get(Ending_Scene);
		//Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_ENDING);

		break;
	}
	default:
		break;
	}
}
void IntroScene::Load()
{
	switch (idStage)
	{
	case ID_INTRO: {
		texturesFilePath = ToLPCWSTR("Intro_Scene.txt");
		//intro_ani_set = CAnimationSets::GetInstance()->Get(Intro_Scene);
		//Sound::GetInstance()->LoadSound("Resource\\Sound\\Intro.wav","BackgroundMusic");
		break;
	}
	case ID_INTROENDING: {
		texturesFilePath = ToLPCWSTR("Ending.txt"); 
		//intro_ani_set = CAnimationSets::GetInstance()->Get(701);
		break;
	}

	default:
		break;
	}
	LoadBaseTextures();
	CGame ::GetInstance()->SetCamPos(0.0f, 0.0f);	//initial camera
}
void IntroScene::LoadBaseTextures()
{
	DebugOut(L"[INFO] Start loading TEXTURES resources from : %s \n", texturesFilePath);

	ifstream f;
	f.open(texturesFilePath);
	
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

	DebugOut(L"[INFO] Done loading TEXTURES resources %s\n", texturesFilePath);
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
	DebugOut(L"[Info] animation added: ");
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

//void PlayScene::SwitchScene(int scene_id)
//{
//	DebugOut(L"dung cong");
//}

void IntroScene::Update(DWORD dt)
{
	/*switch (setAnimation)
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
	}*/
	

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
	
	
	switch (idStage)
	{
	case ID_INTRO:
		CGame::GetInstance()->SetCamPos(0, 0);
		break;
	case ID_INTROENDING:
		if (setEndding == 0 && this->moutainY < 30)
		{
			CGame::GetInstance()->SetCamPos(this->posX, -15 + 1 * dt);
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


}

void IntroScene::Render()
{

	if (this->idStage == ID_INTRO) {

		switch (setAnimation)
		{
		case Intro_Animation_Logo: 
			intro_ani_set->at(Intro_Animation_Logo)->Render(1, 0, 0);
			if (intro_ani_set->at(Intro_Animation_Logo)->GetFrame() == intro_ani_set->at(Intro_Animation_Logo)->GetLastFrameIndex()) { 
				setAnimation = Intro_Animation_Frog; 
			} 
			break;
		case Intro_Animation_Frog: 
			intro_ani_set->at(Intro_Animation_Frog)->Render(1, 0, 0);
			if (intro_ani_set->at(Intro_Animation_Frog)->GetFrame() == intro_ani_set->at(Intro_Animation_Frog)->GetLastFrameIndex()) {
				setAnimation = Intro_Animation_Car; 
			} 
			break;
		case Intro_Animation_Car:
			intro_ani_set->at(Intro_Animation_Car)->Render(1, 0, 0);
			if (intro_ani_set->at(Intro_Animation_Car)->GetFrame() == intro_ani_set->at(Intro_Animation_Car)->GetLastFrameIndex()) { 
				setAnimation = Intro_Done;
			} 
			break;
		default:
			break;
		}
	}
	else if (this->idStage == ID_INTROENDING) {
		
		//intro_ani_set->at(Endding_Background1)->IntroRender(1, 0, 0);
		switch (setEndding)
		{
		case 0:
			intro_ani_set->at(Endding_Cloud)->Render(1, 0, 0);
			intro_ani_set->at(Endding_Mountain)->Render(1, 123, 120 + this->moutainY);
			intro_ani_set->at(Endding_Forest)->Render(1, 0, 105);
			break;
		case 1:
			intro_ani_set->at(Endding_Background1)->Render(1, 0, 0);
			intro_ani_set->at(Endding_Frog)->Render(1, 384, 107);
			intro_ani_set->at(Endding_Hair)->Render(1, 369, 91);
			break;
		case 2:
			intro_ani_set->at(Endding_Background2)->Render(1, 250, 0);
			if(this->textY<272+326)
				intro_ani_set->at(Endding_Text1)->Render(1, 360, 272 - this->textY);
			else
				intro_ani_set->at(Endding_Text2)->Render(1, 360, 0);
			break;
		default:
			break;
		}
		


		
	}
}
void IntroScene::Unload()
{
	//Sound::GetInstance()->UnLoadSound("Opening");
	//Sound::GetInstance()->UnLoadSound("CarSplash");
	//Sound::GetInstance()->UnLoadSound("CarBackground");

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
