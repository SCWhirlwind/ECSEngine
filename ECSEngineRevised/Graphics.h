#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Graphics
{
public:

	// Singleton accessor
	static Graphics& Instance() {
		static Graphics instance;
		return instance;
	}

	// Constants for screen dimensions and window title
	static constexpr int SCREEN_WIDTH = 1280;
	static constexpr int SCREEN_HEIGHT = 1024;
	static constexpr const char* WINDOW_TITLE = "ECS Engine";

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	bool InitSDL();

	SDL_Renderer* GetRenderer() const { return m_Renderer; }
	SDL_Window* GetWindow() const { return m_Window; }

	SDL_Texture* LoadTexture(const std::string& path);
	SDL_Texture* CreateTextTexture(const std::string& text, TTF_Font* font, SDL_Color color);

	void ClearBackBuffer();

	void DrawTexture(SDL_Texture* tex, SDL_Rect* clip = nullptr, SDL_Rect* rend = nullptr, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DrawLine(float startX, float startY, float endX, float endY);

	void DrawRect(SDL_Rect& rect);

	void Render();

private:

	Graphics();
	~Graphics();

	void ShutdownSDL();

	bool m_Initialized;

	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
	SDL_Surface* m_BackBuffer;
};