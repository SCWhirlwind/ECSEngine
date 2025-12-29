#include "AudioManager.h"

AudioManager::AudioManager()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        std::cout << "Mixer Initialization Error: " << Mix_GetError() << std::endl;
    }
}

AudioManager::~AudioManager()
{
    Mix_Quit();
}

void AudioManager::PlayMusic(const std::string& filename, int loop)
{
    Mix_PlayMusic(m_AssetManager.GetMusic(filename), loop);
}

void AudioManager::PauseMusic()
{
    if (Mix_PlayingMusic() != 0)
    {
        Mix_PauseMusic();
    }
}

void AudioManager::ResumeMusic()
{
    if (Mix_PausedMusic() != 0)
    {
        Mix_ResumeMusic();
    }
}

void AudioManager::PlaySFX(const std::string& filename, int loops, int channel)
{
    Mix_PlayChannel(channel, m_AssetManager.GetSFx(filename), loops);
}