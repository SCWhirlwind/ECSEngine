#include "AssetManager.h"

AssetManager::AssetManager() {}

AssetManager::~AssetManager()
{
	for (auto& tex : m_Textures) if (tex.second != nullptr) SDL_DestroyTexture(tex.second);
		
	m_Textures.clear();

	for (auto& text : m_Text) if (text.second != nullptr) SDL_DestroyTexture(text.second);

	m_Text.clear();

	for (auto& font : m_Fonts) if (font.second != nullptr) TTF_CloseFont(font.second);

	m_Fonts.clear();

	for (auto& music : m_Music) if (music.second != nullptr) Mix_FreeMusic(music.second);

	m_Music.clear();

	for (auto& sfx : m_SFx) if (sfx.second != nullptr) Mix_FreeChunk(sfx.second);

	m_SFx.clear();
}

SDL_Texture* AssetManager::GetTexture(const std::string& filename)
{
	char* basepath = SDL_GetBasePath();
	std::string fullPath = basepath;
	fullPath.append("Assets\\" + filename);

	if (m_Textures[fullPath] == nullptr)
	{
		m_Textures[fullPath] = Graphics::Instance().LoadTexture(fullPath);
	}
	SDL_free(basepath);
	return m_Textures[fullPath];
}

SDL_Texture* AssetManager::GetText(const std::string& text, const std::string& filename, int size, SDL_Color color)
{
	TTF_Font* font = GetFont(filename, size);

	std::string key = text + filename + (char)size + (char)color.r + (char)color.b + (char)color.g + (char)color.a;

	if (m_Text[key] == nullptr)
	{
		m_Text[key] = Graphics::Instance().CreateTextTexture(text, font, color);
	}
	return m_Text[key];
}

Mix_Music* AssetManager::GetMusic(const std::string& filename)
{
	char* basepath = SDL_GetBasePath();
	std::string fullPath = basepath;
	fullPath.append("Assets\\" + filename);

	if (m_Music[fullPath] == nullptr)
	{
		m_Music[fullPath] = Mix_LoadMUS(fullPath.c_str());
		if (m_Music[fullPath] == nullptr)
		{
			std::cout << "Music Loading Error: File - " << filename.c_str() << " Error - " << Mix_GetError() << std::endl;
		}
	}

	SDL_free(basepath);
	return m_Music[fullPath];
}

Mix_Chunk* AssetManager::GetSFx(const std::string& filename)
{
	char* basepath = SDL_GetBasePath();
	std::string fullPath = basepath;
	fullPath.append("Assets\\" + filename);

	if (m_SFx[fullPath] == nullptr)
	{
		m_SFx[fullPath] = Mix_LoadWAV(fullPath.c_str());
		if (m_SFx[fullPath] == nullptr)
		{
			std::cout << "SFx Loading Error: File - " << filename.c_str() << " Error - " << Mix_GetError() << std::endl;
		}
	}

	SDL_free(basepath);
	return m_SFx[fullPath];
}

TTF_Font* AssetManager::GetFont(const std::string& filename, int size)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets\\" + filename);
	std::string key = fullPath + (char)size;

	if (m_Fonts[key] == nullptr)
	{
		m_Fonts[key] = TTF_OpenFont(fullPath.c_str(), size);

		if (m_Fonts[key] == nullptr)
		{
			std::cout << "Font Loading Error: Font - " << filename.c_str() << "Error - " << TTF_GetError() << std::endl;
		}
	}
	return m_Fonts[key];
}