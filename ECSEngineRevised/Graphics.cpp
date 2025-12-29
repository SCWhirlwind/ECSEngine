#include "Graphics.h"


Graphics::Graphics() : m_Window(nullptr), m_BackBuffer(nullptr), m_Renderer(nullptr)
{
	InitSDL();
}

Graphics::~Graphics()
{
	ShutdownSDL();
}

bool Graphics::InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL Initialization Error!" << std::endl;
		return false;
	}

	m_Window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	if (m_Window == nullptr)
	{
		std::cout << "SDL Initialization Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	if (m_Renderer == nullptr)
	{
		std::cout << "Renderer Creation Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderDrawColor(m_Renderer, 0x00, 0x00, 0x00, 0xFF);

	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "IMG Initialization Error: " << IMG_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "TTF Initialization Error: " << TTF_GetError() << std::endl;
		return false;
	}

	m_BackBuffer = SDL_GetWindowSurface(m_Window);

	return true;
}

void Graphics::ShutdownSDL()
{
	if (m_Renderer)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	if (m_Window)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* Graphics::LoadTexture(const std::string& path)
{
	SDL_Texture* tex = nullptr;

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr)
	{
		std::cout << "Image Load Error: Path (" << path.c_str() << ") - Error (" << IMG_GetError() << ")" << std::endl;
		return tex;
	}

	tex = SDL_CreateTextureFromSurface(m_Renderer, surface);

	if (tex == nullptr)
	{
		std::cout << "Create Texture Error: " << SDL_GetError() << std::endl;
		return tex;
	}

	SDL_FreeSurface(surface);
	return tex;
}

SDL_Texture* Graphics::CreateTextTexture(const std::string& text, TTF_Font* font,  SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (surface == nullptr)
	{
		std::cout << "Text Render Error:" << TTF_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(m_Renderer, surface);

	if (tex == nullptr)
	{
		std::cout << "Text Texture Creation Error: " << SDL_GetError();
		return nullptr;
	}

	SDL_FreeSurface(surface);
	return tex;
}

void Graphics::ClearBackBuffer()
{
	SDL_RenderClear(m_Renderer);
}

void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* clip, SDL_Rect* rend, float angle, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(m_Renderer, tex, clip, rend, angle, nullptr, flip);
}

void Graphics::DrawLine(float startX, float startY, float endX, float endY)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(m_Renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(m_Renderer, static_cast<int>(startX), static_cast<int>(startY), static_cast<int>(endX), static_cast<int>(endY));
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
}

void Graphics::DrawRect(SDL_Rect& rect)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(m_Renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(m_Renderer, &rect);
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
}

void Graphics::Render()
{
	SDL_RenderPresent(m_Renderer);
}
