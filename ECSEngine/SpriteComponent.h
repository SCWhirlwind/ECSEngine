#pragma once
#include "Component.h"
#include "AssetManager.h"
#include "Animation.h"

class SpriteComponent : public Component
{
	AssetManager* mAsset = AssetManager::Instance();

public:

	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect, destRect;			

	std::string textureName;
	int height;
	int width;
	float scale;

	Animation activeAnim;
	bool hasAnim;

	std::map<std::string, Animation> animations;

	SpriteComponent()
	{
	}

	SpriteComponent(std::string id, int w, int h, float s, bool a)
	{
		textureName = id;
		texture = mAsset->GetTexture(id);
		srcRect.x = srcRect.y = 0;
		srcRect.w = width = w;
		srcRect.h = height = h;
		scale = s;
		hasAnim = a;
	}

	void AddAnimation(std::string name, int index, int frames, int speed)
	{
		animations.emplace(name, Animation(index, frames, speed));
	}

	void Play(std::string name)
	{
		activeAnim.frames = animations[name].frames;
		activeAnim.index = animations[name].index;
		activeAnim.speed = animations[name].speed;
	}

	~SpriteComponent()
	{
		texture = nullptr;
		mAsset = nullptr;
		animations.clear();
	}
};