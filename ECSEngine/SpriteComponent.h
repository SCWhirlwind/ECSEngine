#pragma once
#include "Component.h"
#include "AssetManager.h"

class SpriteComponent : public Component
{
	AssetManager* mAsset = AssetManager::Instance();

public:

	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect, destRect;

	int height;
	int width;
	int scale;

	SpriteComponent()
	{
	}

	SpriteComponent(std::string id, int w, int h, int s)
	{
		texture = mAsset->GetTexture(id);
		srcRect.x = srcRect.y = 0;
		srcRect.w = width = w;
		srcRect.h = height = h;
		scale = s;
	}

	SpriteComponent(std::string text, std::string filename, int size, int w, int h, int s, SDL_Color color)
	{
		texture = mAsset->GetText(text, filename, size, color);
		srcRect.x = srcRect.y = 0;
		srcRect.w = width = w;
		srcRect.h = height = h;
		scale = s;

		SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	}

	~SpriteComponent()
	{
		texture = nullptr;
		mAsset = nullptr;
	}
};