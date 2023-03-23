#pragma once
#include "AssetManager.h"

class AudioManager
{
public:

	static AudioManager* Instance();
	static void Release();

	void PlayMusic(std::string filename, int loop = -1);
	void PauseMusic();
	void ResumeMusic();

	void PlaySFX(std::string filename, int loops = 0, int channel = 0);

private:

	static AudioManager* sInstance;

	AssetManager* mAssetManager;

	AudioManager();
	~AudioManager();

};