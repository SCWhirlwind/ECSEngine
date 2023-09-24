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

	~SpriteComponent()
	{
		texture = nullptr;
		mAsset = nullptr;
	}
};