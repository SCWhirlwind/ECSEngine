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
	static GameManager* Instance();
	static void Release();

	void Run();
	void Quit();

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

private:

	GameManager();
	~GameManager();

	void EarlyUpdate();
	void Update();
	void LateUpdate();

	void Render();

	void Input();

	const int FRAME_RATE = 120;

	static GameManager* sInstance;
	bool mQuit;

	Graphics* mGraphics;
	AssetManager* mAssetManager;
	InputManager* mInputManager;
	AudioManager* mAudioManager;
	//PhysicsManager* mPhysicsManager;

	Timer* mTimer;

	SDL_Event mEvents;

	SceneMap m_sceneMap;

	std::shared_ptr<Scene> currentScene();

	std::string m_currentScene;
};

