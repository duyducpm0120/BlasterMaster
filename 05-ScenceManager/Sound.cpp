#include "Sound.h"
#include "Game.h"
#include <dsound.h>
#include <d3dx9.h>
#include <d3d9.h>
#include "tinyxml.h"
#include "Utils.h"

Sound* Sound::Instance = nullptr;

Sound* Sound::GetInstance()
{
	if (Instance == nullptr)
		Instance = new Sound(CGame::GetInstance()->GetHWND());

	return Instance;
}
Sound::Sound(HWND hWnd)
{
	primaryBuffer = 0;
	HRESULT result;

	DSBUFFERDESC bufferDesc;
	result = DirectSoundCreate8(NULL, &pDevice, NULL);

	if (FAILED(result))
	{
		DebugOut(L"[Sound]Can not create sound device!!");
	}

	result = pDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = pDevice->CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL);

	if (FAILED(result))
	{
		DebugOut(L"[Sound]Can not create primaryBuffer");
	}
	volume = 100.0f;
	isMute = false;

}

void Sound::Create(HWND hWnd)
{
	if (Instance == nullptr)
	{
		Instance = new Sound(hWnd);
	}
}

void Sound::LoadSound(std::string fileName, std::string name)
{
	if (soundBufferMap.find(name) != soundBufferMap.end())
		return;
	FILE* filePtr;
	WaveHeaderStruct waveHeaderStruct;
	IDirectSoundBuffer* tempBuffer;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
	unsigned char* wavData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	int error = fopen_s(&filePtr, fileName.c_str(), "rb");
	if (error != 0)
	{
		DebugOut(L"[Sound]Can not load: %s\n", fileName);
		return;
	}

	fread(&waveHeaderStruct, sizeof(WaveHeaderStruct), 1, filePtr);
	//fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);

	if ((waveHeaderStruct.format[0] != 'W') || (waveHeaderStruct.format[1] != 'A') ||
		(waveHeaderStruct.format[2] != 'V') || (waveHeaderStruct.format[3] != 'E'))
	{
		DebugOut(L"[Sound]file format does not support: %s", fileName);
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = waveHeaderStruct.sampleRate;
	waveFormat.wBitsPerSample = waveHeaderStruct.bitsPerSample;
	waveFormat.nChannels = waveHeaderStruct.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
	bufferDesc.dwBufferBytes = waveHeaderStruct.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer8* secondaryBuffer = 0;
	IDirectSoundBuffer8** pSecondaryBuffer = &secondaryBuffer;
	//IDirectCSoundBuffer8

	//long result = pDevice->CreateCSoundBuffer(&bufferDesc, &secondaryBuffer, NULL);

	pDevice->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);

	long result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*pSecondaryBuffer));

	tempBuffer->Release();
	tempBuffer = 0;

	if (FAILED(result))
	{
		DebugOut(L"[Sound]Can not create secondaryBuffer ");
		return;
	}

	//fseek(filePtr, sizeof(WaveHeaderStruct), SEEK_SET); // move the filePointer cursor to data section

	fseek(filePtr, sizeof(WaveHeaderStruct), SEEK_SET);


	wavData = new unsigned char[waveHeaderStruct.dataSize];



	fread(wavData, waveHeaderStruct.dataSize, 1, filePtr);

	error = fclose(filePtr);
	if (error != 0)
	{
		DebugOut(L"[Sound]Can not close file ");
	}

	result = (*pSecondaryBuffer)->Lock(0, waveHeaderStruct.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);

	if (FAILED(result))
	{
		return;
	}

	memcpy(bufferPtr, wavData, waveHeaderStruct.dataSize);

	(*pSecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);

	if (wavData != NULL)
		delete wavData;
	wavData = 0;
	long tempVolume = (volume) / 100 * (-DSBVOLUME_MIN) + DSBVOLUME_MIN;
	(*pSecondaryBuffer)->SetVolume(tempVolume);

	soundBufferMap[name] = secondaryBuffer;
}

void Sound::UnLoadSound(std::string name)
{
	try {
		std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
		it = soundBufferMap.find(name);
		it->second->Release();
		//SAFE_DELETE(it->second);
		soundBufferMap.erase(name);
	}
	catch
		(exception e) {
	}
}

void Sound::LoadSoundResource(const char* filePath)
{
	TiXmlDocument doc(filePath);
	if (!doc.LoadFile())
	{
		DebugOut(L"[Sound] failure %s", doc.ErrorDesc());
	}

	TiXmlElement* root = doc.RootElement();

	TiXmlElement* sound = nullptr;
	string name;
	string path;

	soundBufferMap.clear();
	DebugOut(L"sound size: %d", soundBufferMap.size());

	for (sound = root->FirstChildElement(); sound != NULL; sound = sound->NextSiblingElement())
	{
		name = sound->Attribute("name"); path = sound->Attribute("path");
		LoadSound(path, name);
	}
}

void Sound::Play(std::string name, bool infiniteLoop, int times)
{
	if (isMute)
	{
		return;
	}
	std::map<std::string, IDirectSoundBuffer8*> ::iterator it;
	it = soundBufferMap.find(name);
	if (it == soundBufferMap.end())
		return;
	if (infiniteLoop)
	{
		it->second->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		//it->second->Stop();
		it->second->SetCurrentPosition(0);
		it->second->Play(0, 0, times - 1);
	}
	//DebugOut(L"so luong am thanh: %d\n", soundBufferMap.size());
}

void Sound::Stop(std::string name)
{
	if (name == "")
	{
		for (std::map< std::string, IDirectSoundBuffer8*> ::iterator it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
		{
			it->second->Stop();
			//soundTemp.insert(make_pair(it->first, it->second));
			it->second->SetCurrentPosition(0);
		}
	}
	else
	{
		std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
		it = soundBufferMap.find(name);
		if (it == soundBufferMap.end())
			return;
		else it->second->Stop();
	}
}

Sound::~Sound()
{
	for (auto it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
	{
		if (it->second)
		{
			it->second->Release();
			it->second = NULL;
		}

	}
	if (primaryBuffer)
		primaryBuffer->Release();
	if (pDevice)
		pDevice->Release();
	pDevice = NULL;

	primaryBuffer = NULL;
}