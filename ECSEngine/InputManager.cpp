#include "InputManager.h"

InputManager* InputManager::sInstance = nullptr;

InputManager* InputManager::Instance()
{
    if (sInstance == nullptr)
    {
        sInstance = new InputManager();
    }
    return sInstance;
}

void InputManager::Release()
{
    delete sInstance;
    sInstance = nullptr;
}

bool InputManager::KeyPressed(SDL_Scancode scanCode)
{
    return (mPrevKeyboardState[scanCode]) == 0 && (mKeyboardState[scanCode] != 0);
}

bool InputManager::KeyReleased(SDL_Scancode scanCode)
{
    return (mPrevKeyboardState[scanCode] != 0) && (mKeyboardState[scanCode] == 0);
}

bool InputManager::MouseButtonDown(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button)
    {
    case MOUSE_BUTTON::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MOUSE_BUTTON::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MOUSE_BUTTON::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    case MOUSE_BUTTON::back:
        mask = SDL_BUTTON_X1MASK;
        break;
    case MOUSE_BUTTON::forward:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return ((mMouseState & mask) != 0);
}

bool InputManager::MouseButtonPressed(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button)
    {
    case MOUSE_BUTTON::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MOUSE_BUTTON::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MOUSE_BUTTON::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    case MOUSE_BUTTON::back:
        mask = SDL_BUTTON_X1MASK;
        break;
    case MOUSE_BUTTON::forward:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return ((mPrevMouseState & mask) == 0) && ((mMouseState & mask) != 0);
}

bool InputManager::MouseButtonReleasd(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button)
    {
    case MOUSE_BUTTON::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MOUSE_BUTTON::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MOUSE_BUTTON::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    case MOUSE_BUTTON::back:
        mask = SDL_BUTTON_X1MASK;
        break;
    case MOUSE_BUTTON::forward:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return ((mPrevMouseState & mask) != 0) && ((mMouseState & mask) == 0);
}

Vec2 InputManager::MousePos()
{
    return Vec2((float)mMouseXPos, (float)mMouseYPos);
}

void InputManager::Update()
{
    mMouseState = SDL_GetMouseState(&mMouseXPos, &mMouseYPos);
}

void InputManager::UpdatePrevInput()
{
    memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
    mPrevMouseState = mMouseState;
}

InputManager::InputManager()
{
    mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
    mPrevKeyboardState = new Uint8[mKeyLength];
    memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
}

InputManager::~InputManager()
{
    delete[] mPrevKeyboardState;
    mPrevKeyboardState = nullptr;
}