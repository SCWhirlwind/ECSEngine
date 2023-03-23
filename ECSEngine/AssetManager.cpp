#include "AssetManager.h"
AssetManager* AssetManager::sInstance = nullptr;

AssetManager* AssetManager::Instance()
{
	if (sInstance == nullptr)
	{
		sInstance = new AssetManager();
	}
	return sInstance;
}

void AssetManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

SDL_Texture* AssetManager::GetTexture(std::string filename)
{
	char* basepath = SDL_GetBasePath();
	std::string fullPath = basepath;
	fullPath.append("Assets\\" + filename);

	if (mTextures[fullPath] == nullptr)
	{
		mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);
	}
	SDL_free(basepath);
	return mTextures[fullPath];
}

SDL_Texture* AssetManager::GetText(std::string text, std::string filename, int size, SDL_Color color)
{
	TTF_Font* font = GetFont(filename, size);

	std::string key = text + filename + (char)size + (char)color.r + (char)color.b + (char)color.g;

	if (mText[key] == nullptr)
	{
		mText[key] = Graphics::Instance()->CreateTextTexture(font, text, color);
	}
	return mText[key];
}

Mix_Music* AssetManager::GetMusic(std::string filename)
{
	char* basepath = SDL_GetBasePath();
	std::string fullPath = basepath;
	fullPath.append("Assets\\" + filename);

	if (mMusic[fullPath] == nullptr)
	{
		mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());
		if (mMusic[fullPath] == nullptr)
		{
			std::cout << "Music Loading Error: File - " << filename.c_str() << " Error - " << Mix_GetError() << std::endl;
		}
	}

	SDL_free(basepath);
	return mMusic[fullPath];
}

Mix_Chunk* AssetManager::GetSFx(std::string filename)
{
	char* basepath = SDL_GetBasePath();
	std::string fullPath = basepath;
	fullPath.append("Assets\\" + filename);

	if (mSFx[fullPath] == nullptr)
	{
		mSFx[fullPath] = Mix_LoadWAV(fullPath.c_str());
		if (mSFx[fullPath] == nullptr)
		{
			std::cout << "SFx Loading Error: File - " << filename.c_str() << " Error - " << Mix_GetError() << std::endl;
		}
	}

	SDL_free(basepath);
	return mSFx[fullPath];
}

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{
	for (auto& tex : mTextures)
	{
		if (tex.second != nullptr)
		{
			SDL_DestroyTexture(tex.second);
		}
	}

	mTextures.clear();

	for (auto& text : mText)
	{
		if (text.second != nullptr)
		{
			SDL_DestroyTexture(text.second);
		}
	}

	mText.clear();

	for (auto& font : mFonts)
	{
		if (font.second != nullptr)
		{
			TTF_CloseFont(font.second);
		}
	}

	mFonts.clear();

	for (auto& music : mMusic)
	{
		if (music.second != nullptr)
		{
			Mix_FreeMusic(music.second);
		}
	}

	mMusic.clear();

	for (auto& sfx : mSFx)
	{
		if (sfx.second != nullptr)
		{
			Mix_FreeChunk(sfx.second);
		}
	}

	mSFx.clear();
}

TTF_Font* AssetManager::GetFont(std::string filename, int size)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets\\" + filename);
	std::string key = fullPath + (char)size;

	if (mFonts[key] == nullptr)
	{
		mFonts[key] = TTF_OpenFont(fullPath.c_str(), size);

		if (mFonts[key] == nullptr)
		{
			std::cout << "Font Loading Error: Font - " << filename.c_str() << "Error - " << TTF_GetError() << std::endl;
		}
	}
	return mFonts[key];
}