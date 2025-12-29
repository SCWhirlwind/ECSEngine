#include "InputManager.h"

InputManager::InputManager()
{
    m_KeyboardState = SDL_GetKeyboardState(&m_KeyLength);
    m_PrevKeyboardState = new Uint8[m_KeyLength];
    memcpy(m_PrevKeyboardState, m_KeyboardState, m_KeyLength);
}

InputManager::~InputManager()
{
    delete[] m_PrevKeyboardState;
    m_PrevKeyboardState = nullptr;
}

bool InputManager::KeyPressed(SDL_Scancode scanCode)
{
    return (m_PrevKeyboardState[scanCode]) == 0 && (m_KeyboardState[scanCode] != 0);
}

bool InputManager::KeyReleased(SDL_Scancode scanCode)
{
    return (m_PrevKeyboardState[scanCode] != 0) && (m_KeyboardState[scanCode] == 0);
}

bool InputManager::MouseButtonDown(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button)
    {
        case MOUSE_BUTTON::left:    mask = SDL_BUTTON_LMASK;  break;
        case MOUSE_BUTTON::right:   mask = SDL_BUTTON_RMASK;  break;
        case MOUSE_BUTTON::middle:  mask = SDL_BUTTON_MMASK;  break;
        case MOUSE_BUTTON::back:    mask = SDL_BUTTON_X1MASK; break;
        case MOUSE_BUTTON::forward: mask = SDL_BUTTON_X2MASK; break;
    }

    return ((m_MouseState & mask) != 0);
}

bool InputManager::MouseButtonPressed(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button)
    {
        case MOUSE_BUTTON::left:    mask = SDL_BUTTON_LMASK;  break;
        case MOUSE_BUTTON::right:   mask = SDL_BUTTON_RMASK;  break;
        case MOUSE_BUTTON::middle:  mask = SDL_BUTTON_MMASK;  break;
        case MOUSE_BUTTON::back:    mask = SDL_BUTTON_X1MASK; break;
        case MOUSE_BUTTON::forward: mask = SDL_BUTTON_X2MASK; break;
    }

    return ((m_PrevMouseState & mask) == 0) && ((m_MouseState & mask) != 0);
}

bool InputManager::MouseButtonReleased(MOUSE_BUTTON button)
{
    Uint32 mask = 0;

    switch (button)
    {
        case MOUSE_BUTTON::left:    mask = SDL_BUTTON_LMASK;  break;
        case MOUSE_BUTTON::right:   mask = SDL_BUTTON_RMASK;  break;
        case MOUSE_BUTTON::middle:  mask = SDL_BUTTON_MMASK;  break;
        case MOUSE_BUTTON::back:    mask = SDL_BUTTON_X1MASK; break;
        case MOUSE_BUTTON::forward: mask = SDL_BUTTON_X2MASK; break;
    }

    return ((m_PrevMouseState & mask) != 0) && ((m_MouseState & mask) == 0);
}

Vec2 InputManager::MousePos()
{
    return Vec2(static_cast<float>(m_MouseXPos), static_cast<float>(m_MouseYPos));
}

void InputManager::Update()
{
    m_MouseState = SDL_GetMouseState(&m_MouseXPos, &m_MouseYPos);
}

void InputManager::UpdatePrevInput()
{
    memcpy(m_PrevKeyboardState, m_KeyboardState, m_KeyLength);
    m_PrevMouseState = m_MouseState;
}