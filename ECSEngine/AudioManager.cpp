#include "AudioManager.h"

AudioManager* AudioManager::sInstance = nullptr;

AudioManager* AudioManager::Instance()
{
    if (sInstance == nullptr)
    {
        sInstance = new AudioManager();
    }
    return sInstance;
}

void AudioManager::Release()
{
    delete sInstance;
    sInstance = nullptr;
}

void AudioManager::PlayMusic(std::string filename, int loop)
{
    Mix_PlayMusic(mAssetManager->GetMusic(filename), loop);
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

void AudioManager::PlaySFX(std::string filename, int loops, int channel)
{
    Mix_PlayChannel(channel, mAssetManager->GetSFx(filename), loops);
}

AudioManager::AudioManager()
{
    mAssetManager = AssetManager::Instance();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        std::cout << "Mixer Initialization Error: " << Mix_GetError() << std::endl;
    }
}

AudioManager::~AudioManager()
{
    mAssetManager = nullptr;
    Mix_Quit();
}