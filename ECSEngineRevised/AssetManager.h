#pragma once
#include "Graphics.h"
#include <SDL_mixer.h>
#include <map>

class AssetManager
{
public:

	// Singleton accessor
	static AssetManager& Instance() {
		static AssetManager instance;
		return instance;
	}

	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	SDL_Texture* GetTexture(const std::string& filename);
	SDL_Texture* GetText(const std::string& text, const std::string& filename, int size, SDL_Color color);

	Mix_Music* GetMusic(const std::string& filename);
	Mix_Chunk* GetSFx(const std::string& filename);

private:

	AssetManager();
	~AssetManager();

	TTF_Font* GetFont(const std::string& filename, int size);

	std::map<std::string, SDL_Texture*> m_Textures;
	std::map<std::string, SDL_Texture*> m_Text;
	std::map<std::string, TTF_Font*> m_Fonts;
	std::map<std::string, Mix_Music*> m_Music;
	std::map<std::string, Mix_Chunk*> m_SFx;
};

