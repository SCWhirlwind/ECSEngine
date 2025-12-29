#pragma once
#include "AssetManager.h"

class AudioManager
{
public:

	// Singleton accessor
	static AudioManager& Instance() {
		static AudioManager instance;
		return instance;
	}

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	void PlayMusic(const std::string& filename, int loop = -1);
	void PauseMusic();
	void ResumeMusic();

	void PlaySFX(const std::string& filename, int loops = 0, int channel = 0);

private:

	AudioManager();
	~AudioManager();

	AssetManager& m_AssetManager = AssetManager::Instance();
};