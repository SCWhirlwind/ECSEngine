#include "GameManager.h"
#include <stdlib.h>
#include <time.h>
#include "Scene_Menu.h"

GameManager* GameManager::sInstance = nullptr;

GameManager* GameManager::Instance()
{
	if (sInstance == nullptr)
	{
		sInstance = new GameManager();
	}
	return sInstance;
}

void GameManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

void GameManager::Run()
{
	changeScene("MENU", std::make_shared<Scene_Menu>(this));

	while (!mQuit)
	{
		mTimer->Update();

		if (mTimer->DeltaTime() >= (1.0f / FRAME_RATE))
		{
			EarlyUpdate();
			Update();
			LateUpdate();
			Render();
		}
	}
}

void GameManager::Quit()
{
	mQuit = true;
}

void GameManager::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (scene)
	{
		m_sceneMap[sceneName] = scene;
		m_currentScene = sceneName;
	}
	if (m_sceneMap.find(sceneName) == m_sceneMap.end())
	{
		m_currentScene = "";
	}
}

GameManager::GameManager()
{
	srand((Uint8)time(0));

	mQuit = false;

	mGraphics = Graphics::Instance();

	if (!Graphics::Initialized())
	{
		mQuit = true;
	}

	mAssetManager = AssetManager::Instance();

	mInputManager = InputManager::Instance();

	mAudioManager = AudioManager::Instance();

	mTimer = Timer::Instance();

	//mPhysicsManager = PhysicsManager::Instance();
	//mPhysicsManager->SetLayerCollisionMasks(PhysicsManager::CollisionLayers::Friendly, PhysicsManager::CollisionFlags::Hostile | PhysicsManager::CollisionFlags::HostileProjectile);
	//mPhysicsManager->SetLayerCollisionMasks(PhysicsManager::CollisionLayers::FriendlyProjectile, PhysicsManager::CollisionFlags::Hostile);
	//mPhysicsManager->SetLayerCollisionMasks(PhysicsManager::CollisionLayers::Hostile, PhysicsManager::CollisionFlags::Friendly | PhysicsManager::CollisionFlags::FriendlyProjectile);
	//mPhysicsManager->SetLayerCollisionMasks(PhysicsManager::CollisionLayers::HostileProjectile, PhysicsManager::CollisionFlags::Friendly);
}

GameManager::~GameManager()
{

	//PhysicsManager::Release();
	//mPhysicsManager = nullptr;

	AudioManager::Release();
	mAudioManager = nullptr;

	AssetManager::Release();
	mAssetManager = nullptr;

	Graphics::Release();
	mGraphics = nullptr;

	InputManager::Release();
	mInputManager = nullptr;

	Timer::Release();
	mTimer = nullptr;
}

void GameManager::EarlyUpdate()
{
	mTimer->Reset();
	mInputManager->Update();
	Input();
	
}

void GameManager::Update()
{
	if (currentScene())
	{
		currentScene()->update();
	}
}

void GameManager::LateUpdate()
{
	//mPhysicsManager->Update();
	mInputManager->UpdatePrevInput();
}

void GameManager::Render()
{
	mGraphics->ClearBackBuffer();
	currentScene()->sRender();
	mGraphics->Render();
}

void GameManager::Input()
{
	while (SDL_PollEvent(&mEvents) != 0)
	{
		if (mEvents.type == SDL_QUIT)
		{
			mQuit = true;
		}

		if (mEvents.type == SDL_KEYDOWN || mEvents.type == SDL_KEYUP)
		{
			if (currentScene()->getActionMap().find(mEvents.key.keysym.scancode) == currentScene()->getActionMap().end())
			{
				std::cout << "Non registered key!" << std::endl;
			}
			else
			{
				const std::string actionType = (mEvents.type == SDL_KEYDOWN) ? "START" : "END";

				currentScene()->doAction(Action(currentScene()->getActionMap().at(mEvents.key.keysym.scancode), actionType));
			}
		}
	}
}

std::shared_ptr<Scene> GameManager::currentScene()
{
	return m_sceneMap[m_currentScene];
}
