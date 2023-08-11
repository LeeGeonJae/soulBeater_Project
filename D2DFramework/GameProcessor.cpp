#include "GameProcessor.h"

#include "RenderManger.h"
#include "TimeManger.h"
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
		, mTimeManager(new TimeManager())
		, mRenderManager(new RenderManager())
		, mSceneManager(new SceneManager())
		, mCollisionManager(new CollisionManager())
		, mCameraManager(new CameraManager())
		, mDefaultCamera(nullptr)
		, mSoundManager(new SoundManager())
	{
		InputManager::mInstance = new InputManager;
		EventManager::mInstance = new EventManager;
		ObjectManager::mInstance = new ObjectManager;
	}

	GameProcessor::~GameProcessor()
	{
		delete mTimeManager;
		delete mRenderManager;
		delete mSceneManager;
		delete mCollisionManager;
		delete InputManager::mInstance;
		InputManager::mInstance = nullptr;
		delete EventManager::mInstance;
		EventManager::mInstance = nullptr;
		delete ObjectManager::mInstance;
		ObjectManager::mInstance = nullptr;
	}

	void GameProcessor::Init()
	{
		HRESULT hr;
		hr = CoInitialize(NULL);
		assert(SUCCEEDED(hr));

		ICollideable::SetCollisionManager(mCollisionManager);
		IRenderable::SetRenderManager(mRenderManager);
		IUpdateable::SetSceneManager(mSceneManager);
		IFixedUpdateable::SetSceneManager(mSceneManager);
		mRenderManager->SetHwnd(mHwnd);
		InputManager::GetInstance()->SetHwnd(mHwnd);

		mRenderManager->Init();
		mTimeManager->Init();
		mCollisionManager->Init();
		mSceneManager->Init();
		mSoundManager->Init();

		// EventManager::mInstance;
		InputManager::mInstance->Init();

		mCameraManager->SetScreenSize({ static_cast<float>(mWidth), static_cast<float>(mHeight) });
	}

	void GameProcessor::Update()
	{
		static float s_FixedTime = 0.f;

		mTimeManager->Update();
		InputManager::mInstance->Update();

		const float DELTA_TIME = mTimeManager->GetDeltaTime();
		assert(DELTA_TIME >= 0.f);

		s_FixedTime += DELTA_TIME;

		const float FIXED_DELTA_TIME = 0.02f;
		while (s_FixedTime >= FIXED_DELTA_TIME)
		{
			mSceneManager->FixedUpdate(FIXED_DELTA_TIME);
			mCollisionManager->Update();

			s_FixedTime -= FIXED_DELTA_TIME; // FIXED_DELTA_TIME;
		}

		mSceneManager->Update(DELTA_TIME);
		// lateUpdate?
		mRenderManager->Render(mCameraManager);

		ObjectManager::mInstance->handleObjectLifeSpan();
		EventManager::mInstance->handleEvent();
	}

	void GameProcessor::Release()
	{
		mSoundManager->Release();
		mRenderManager->Release();
		mSceneManager->Release();
		mCollisionManager->Release();
		EventManager::mInstance->release();
		ObjectManager::mInstance->release();

		ICollideable::SetCollisionManager(nullptr);
		IRenderable::SetRenderManager(nullptr);
		IUpdateable::SetSceneManager(nullptr);
		IFixedUpdateable::SetSceneManager(nullptr);

		CoUninitialize();
	}
}