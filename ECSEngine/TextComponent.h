#pragma once
#include "Component.h"
#include "AssetManager.h"

class TextComponent : public Component
{
	AssetManager* mAsset = AssetManager::Instance();

public:

	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect, destRect;

	std::string text;
	std::string fname;
	int size;
	SDL_Color color;
	int height;
	int width;
	int scale;

	TextComponent()
	{
	}

	TextComponent(std::string tex, std::string filename, int textsize, int w, int h, int s, SDL_Color textcolor)
	{
		text = tex;
		fname = filename;
		size = textsize;
		color = textcolor;
		texture = mAsset->GetText(text, fname, size, color);
		srcRect.x = srcRect.y = 0;
		srcRect.w = width = w;
		srcRect.h = height = h;
		scale = s;

		SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	}

	~TextComponent()
	{
		texture = nullptr;
		mAsset = nullptr;
	}
};