#include "GameProcessor.h"

#include "RenderManger.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "SoundManager.h"
#include "IUpdateable.h"
#include "IFixedUpdateable.h"
#include "ICollideable.h"
#include "IRenderable.h"
#include "WinApp.h"
#include "eFrameworkID.h"
#include "Scene.h"

#include <cassert>

namespace d2dFramework
{
	GameProcessor::GameProcessor(UINT width, UINT height, std::wstring name)
		: mWidth(width)
		, mHeight(height)
		, mTitle(name)
		, mTimeManager(new TimeManager)
		, mSceneManager(new SceneManager)
		, mCollisionManager(new CollisionManager)
		, mCameraManager(new CameraManager)
		, mHwnd(nullptr)
		, mbIsUpdate()
	{
		InputManager::mInstance = new InputManager;
		EventManager::mInstance = new EventManager;
		ObjectManager::mInstance = new ObjectManager;
		SoundManager::mInstance = new SoundManager;
		RenderManager::mInstance = new RenderManager;
	}

	GameProcessor::~GameProcessor()
	{
		delete mTimeManager;
		delete mSceneManager;
		delete mCollisionManager;
		delete mCameraManager;
		delete InputManager::mInstance;
		InputManager::mInstance = nullptr;
		delete EventManager::mInstance;
		EventManager::mInstance = nullptr;
		delete ObjectManager::mInstance;
		ObjectManager::mInstance = nullptr;
		delete SoundManager::mInstance;
		SoundManager::mInstance = nullptr;
		delete RenderManager::mInstance;
		RenderManager::mInstance = nullptr;
	}

	void GameProcessor::Init(HWND hwnd)
	{
		HRESULT hr;
		hr = CoInitialize(NULL);
		assert(SUCCEEDED(hr));

		mHwnd = hwnd;

		ICollideable::SetCollisionManager(mCollisionManager);
		IRenderable::SetRenderManager(RenderManager::mInstance);
		IUpdateable::SetObjectManager(ObjectManager::mInstance);
		IFixedUpdateable::SetObjectManager(ObjectManager::mInstance);

		mTimeManager->Init();
		mCollisionManager->Init();
		mSceneManager->Init();
		//mUIManager->SetSceneManager(mSceneManager);
		//mSoundManager->Init();

		// Singleton Manager
		RenderManager::mInstance->init(mHwnd);
		InputManager::mInstance->init(mHwnd);
		EventManager::mInstance;
		ObjectManager::mInstance;
		SoundManager::mInstance->Init();
		mCameraManager->SetScreenSize({ static_cast<float>(mWidth), static_cast<float>(mHeight) });

		mbIsUpdate = true;
	}

	void GameProcessor::Release()
	{
		mTimeManager;
		mSceneManager->Release();
		mCollisionManager->Release();
		mCameraManager->Release();

		// singleton class
		InputManager::mInstance;
		EventManager::mInstance->release();
		ObjectManager::mInstance->Release();
		SoundManager::mInstance->release();
		RenderManager::mInstance->release();

		ICollideable::SetCollisionManager(nullptr);
		IRenderable::SetRenderManager(nullptr);
		IUpdateable::SetObjectManager(nullptr);
		IFixedUpdateable::SetObjectManager(nullptr);

		mHwnd = NULL;

		CoUninitialize();
	}

	void GameProcessor::Update()
	{
		static float s_FixedTime = 0.f;

		mTimeManager->Update();
		InputManager::mInstance->update();

		const float DELTA_TIME = mTimeManager->GetDeltaTime();
		assert(DELTA_TIME >= 0.f);

		if (mbIsUpdate)
		{
			s_FixedTime += DELTA_TIME;
			const float FIXED_DELTA_TIME = 0.02f;
			while (s_FixedTime >= FIXED_DELTA_TIME)
			{
				ObjectManager::mInstance->fixedUpdate(FIXED_DELTA_TIME);
				mCollisionManager->Update();

				s_FixedTime -= FIXED_DELTA_TIME; // FIXED_DELTA_TIME;
			}

			ObjectManager::mInstance->update(DELTA_TIME);
		}

		RenderManager::mInstance->render(mCameraManager);
		ObjectManager::mInstance->handleObjectLifeSpan();
		EventManager::mInstance->handleEvent();
	}
}