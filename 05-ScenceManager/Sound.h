#pragma once
#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <dsound.h>


#define SOUND_RESOURCE_INTRO "Sources/Sound/SoundIntro.xml"
#define SOUND_RESOURCE_UNDERWORLD "Sources/Sound/SoundUnderworld.xml"
#define SOUND_RESOURCE_OVERWORLD "Sources/Sound/SoundOverworld.xml"
#define SOUND_RESOURCE_BOSS "Sources/Sound/SoundBoss.xml"
#define SOUND_RESOURCE_ENDING "Sources/Sound/SoundEnding.xml"

class Sound
{
private:
	Sound(HWND hWnd);
	static Sound* Instance;
	IDirectSoundBuffer* primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*> soundBufferMap;
	IDirectSound8* pDevice;
	bool isMute;

	std::map<std::string, IDirectSoundBuffer8*> soundTemp; // sound lưu thông tin để resume

public:
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	static Sound* GetInstance();
	float volume;
	void static Create(HWND hWnd);
	void Play(std::string name, bool infiniteLoop, int times = 100000);
	void Stop(std::string name = "");
	void LoadSound(std::string fileName, std::string name);
	void UnLoadSound(std::string name);
	void LoadSoundResource(const char*);
	~Sound();
};



