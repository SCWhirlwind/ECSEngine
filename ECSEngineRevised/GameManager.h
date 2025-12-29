#pragma once

#include "Timer.h"
#include "Graphics.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "Scene.h"

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameManager
{
public:
	
	static GameManager& Instance() {
		static GameManager instance;
		return instance;
	}

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	void Run();
	void Quit();

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void ToggleTypeInput(bool toggle);
	bool getTypeInput() const { return isTypeInput; }
	bool getRenderText() const { return renderText; }
	std::string getInputText() { return inputText; }
	void setInputText(std::string& text) { inputText = text; }

	std::shared_ptr<Scene> currentScene() const;

private:

	GameManager();
	~GameManager();

	void EarlyUpdate();
	void Update();
	void LateUpdate();

	void Render();

	void Input();

	const int FRAME_RATE = 120;

	bool mQuit = false;

	std::string inputText;
	bool isTypeInput = false;
	bool renderText = false;

	std::unique_ptr<Graphics> m_Graphics;
	std::unique_ptr<AssetManager> m_AssetManager;
	std::unique_ptr<InputManager> m_InputManager;
	std::unique_ptr<AudioManager> m_AudioManager;
	std::unique_ptr<Timer> m_Timer;

	SDL_Event m_Events;

	SceneMap m_sceneMap;

	std::string m_currentScene;
};

