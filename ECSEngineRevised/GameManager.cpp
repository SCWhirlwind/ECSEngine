#include "GameManager.h"
#include <stdlib.h>
#include <time.h>
#include "Scene_Menu.h"

GameManager::GameManager()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	mQuit = false;

	m_Graphics = std::make_unique<Graphics>();

	if (!m_Graphics->InitSDL())
	{
		mQuit = true;
		std::cerr << "Graphics failed to initialize!" << std::endl;
	}

	m_AssetManager = std::make_unique<AssetManager>();
	m_InputManager = std::make_unique<InputManager>();
	m_AudioManager = std::make_unique<AudioManager>();
	m_Timer = std::make_unique<Timer>();

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

GameManager::~GameManager(){}

void GameManager::Run()
{
	while (!mQuit)
	{
		m_Timer->Update();

		if (m_Timer->DeltaTime() >= (1.0f / FRAME_RATE))
		{
			EarlyUpdate();
			Update();
			LateUpdate();
			Render();
		}
	}
}

void GameManager::Quit() { mQuit = true; }

void GameManager::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (scene) m_sceneMap[sceneName] = scene;
	else if (m_sceneMap.find(sceneName) == m_sceneMap.end())
	{
		std::cerr << "Warning: Scene does not exist: " << sceneName << std::endl;
		return;
	}

	if (endCurrentScene && !m_currentScene.empty()) m_sceneMap.erase(m_currentScene);

	m_currentScene = sceneName;
}

std::shared_ptr<Scene> GameManager::currentScene() const
{
	auto it = m_sceneMap.find(m_currentScene);
	return (it != m_sceneMap.end()) ? it->second : nullptr;
}

void GameManager::EarlyUpdate()
{
	m_Timer->Reset();
	m_InputManager->Update();
	Input();

}

void GameManager::Update()
{
	if (currentScene()) currentScene()->update();
}

void GameManager::LateUpdate()
{
	m_InputManager->UpdatePrevInput();
}

void GameManager::Render()
{
	if (!currentScene()) return;
	m_Graphics->ClearBackBuffer();
	currentScene()->sRender();
	m_Graphics->Render();
}

void GameManager::Input()
{
	while (SDL_PollEvent(&m_Events))
	{
		if (m_Events.type == SDL_QUIT) mQuit = true;

		if (!isTypeInput)
		{
			if (m_Events.type == SDL_KEYDOWN || m_Events.type == SDL_KEYUP)
			{
				if (currentScene()->getActionMap().find(m_Events.key.keysym.scancode) == currentScene()->getActionMap().end())
				{
					std::cout << "Non registered key!" << std::endl;
				}
				else
				{
					const std::string actionType = (m_Events.type == SDL_KEYDOWN) ? "START" : "END";

					currentScene()->doAction(Action(currentScene()->getActionMap().at(m_Events.key.keysym.scancode), actionType));
				}
			}
		}
		else
		{
			renderText = false;

			if (m_Events.type == SDL_KEYDOWN)
			{
				if (m_Events.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
				{
					inputText.pop_back();
					renderText = true;
					std::cout << "Text: " << inputText << std::endl;
				}
			}
			else if (m_Events.type == SDL_TEXTINPUT)
			{
				if (!(SDL_GetModState() & KMOD_CTRL && (m_Events.text.text[0] == 'c' || m_Events.text.text[0] == 'C' || m_Events.text.text[0] == 'v' || m_Events.text.text[0] == 'V')))
				{
					inputText += m_Events.text.text;
					renderText = true;
					std::cout << "Text: " << inputText << std::endl;
				}
			}
		}
	}
}

void GameManager::ToggleTypeInput(bool toggle)
{
	isTypeInput = toggle;
}

bool GameManager::getTypeInput() const
{
	return isTypeInput;
}

bool GameManager::getRenderText() const
{
	return renderText;
}

std::string GameManager::getInputText()
{
	return inputText;
}

void GameManager::setInputText(std::string& text)
{
	inputText = text;
}