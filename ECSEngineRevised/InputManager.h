#pragma once
#include <SDL.h>
#include<memory>

#include "Vec2.h"

class InputManager
{
public:

	enum class MOUSE_BUTTON { left = 0, right, middle, back, forward };

	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	bool KeyPressed(SDL_Scancode scanCode);
	bool KeyReleased(SDL_Scancode scanCode);

	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleased(MOUSE_BUTTON button);

	Vec2 MousePos();

	void Update();
	void UpdatePrevInput();

private:

	InputManager();
	~InputManager();

	Uint8* m_PrevKeyboardState;
	const Uint8* m_KeyboardState;
	int m_KeyLength;

	Uint32 m_PrevMouseState;
	Uint32 m_MouseState;

	int m_MouseXPos;
	int m_MouseYPos;
};

