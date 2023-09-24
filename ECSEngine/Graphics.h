#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class Graphics
{
public:

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 1024;

	const char* WINDOW_TITLE = "ECS Engine";

	static Graphics* Instance();
	static void Release();
	static bool Initialized();

	SDL_Texture* LoadTexture(std::string path);
	SDL_Texture* CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color);

	void ClearBackBuffer();

	void DrawTexture(SDL_Texture* tex, SDL_Rect* clip = nullptr, SDL_Rect* rend = nullptr, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DrawLine(float startX, float startY, float endX, float endY);

	void DrawRect(SDL_Rect& rect);

	void Render();

private:

	Graphics();
	~Graphics();

	bool Init();

	static Graphics* sInstance;
	static bool sInitialized;

	SDL_Window* mWindow;
	SDL_Surface* mBackBuffer;

	SDL_Renderer* mRenderer;
};

