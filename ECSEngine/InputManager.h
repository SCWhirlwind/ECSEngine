#pragma once
#include <SDL.h>
#include<memory>

#include "Vec2.h"

class InputManager
{
public:

	enum class MOUSE_BUTTON { left = 0, right, middle, back, forward };

	static InputManager* Instance();
	static void Release();

	bool KeyPressed(SDL_Scancode scanCode);
	bool KeyReleased(SDL_Scancode scanCode);

	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleasd(MOUSE_BUTTON button);

	Vec2 MousePos();


	void Update();
	void UpdatePrevInput();

private:

	static InputManager* sInstance;

	Uint8* mPrevKeyboardState;
	const Uint8* mKeyboardState;
	int mKeyLength;

	Uint32 mPrevMouseState;
	Uint32 mMouseState;

	int mMouseXPos;
	int mMouseYPos;

	InputManager();
	~InputManager();
};

