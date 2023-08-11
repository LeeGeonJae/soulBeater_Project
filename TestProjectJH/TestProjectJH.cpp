#include "TestProjectJH.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "RenderManger.h"
#include "InputManager.h"
#include "BeatManager.h"
#include "AsyncInputManager.h"
#include "TimeManger.h"
#include "MathHelper.h"
#include "SoundManager.h"
#include "SoundAsset.h"
#include "EffectManager.h"

#include <iostream>
#include <string>
#include <future>

namespace testProjectJH
{
	TestProjectJH::TestProjectJH(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mAsyncInputManager(new d2dFramework::AsyncInputManager())
		, mBeatManager(new d2dFramework::BeatManager(DEFUALT_BPM))
		, mEffectManager(new d2dFramework::EffectManager())
	{
	}

	void TestProjectJH::Init()
	{
		using namespace d2dFramework;

		static unsigned int tempId = 20000;

		GameProcessor::Init();
		//getSceneManager()->RegisterScene("hoho", new Scene("hoho"));
		// register Register Scene;

		GameObject* gm = ObjectManager::GetInstance()->CreateObject(tempId++);
		gm->CreateComponent<Transform>(tempId++);
		SpriteRenderer* renderer = gm->CreateComponent<SpriteRenderer>(tempId++);

		getRenderManager()->CreateD2DBitmapFromFile(L"./image/monster2.png");

		renderer->SetSize({ 700, 700 });
		renderer->SetSpriteType(eSpriteType::Sprite);
		renderer->SetUVRectangle({ 0, 0, 500, 500 });

		mEffectManager->SetDC(getRenderManager()->GetD2DDeviceContext());
		mEffectManager->Init();

		mSoundId = tempId++;
		mEffectId = tempId++;
		getSoundManager()->CreateSoundAsset(mSoundId, "./sound/test_sound.ogg", true);
		getSoundManager()->CreateSoundAsset(mEffectId, "./sound/ARTHURHIT.wav", false);
		//getSoundManager()->Play(mSoundId);

		initAsyncInputManager();
		mBeatManager->Init();

		mBitmap = mEffectManager->CreateSpotLight(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"));
		renderer->SetBitmap(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"));
		mBitmap = mEffectManager->CreateGaussianBlur(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"));

		// LoadJsonData(Scene1);

		// GetGameObjectMangaer()->GetObject(10000)->CreateCompoent<PlayerComponet>();
		// GetGameObjectMangaer()->GetObject(20001)->CreateCompoent<EnemyComponet>();
		// GetGameObjectMangaer()->GetObject(20002)->CreateCompoent<EnemyComponet>();
		// GetGameObjectMangaer()->GetObject(20003)->CreateCompoent<EnemyComponet>();
	}

	void TestProjectJH::Update()
	{
		using namespace d2dFramework;
		getRenderManager()->BeginDraw();
		auto pos = InputManager::GetInstance()->GetMousePos();

		getRenderManager()->SetStrokeWidth(10);
		getRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		getRenderManager()->DrawPoint({ static_cast<float>(pos.x), static_cast<float>(pos.y) });

		GameProcessor::Update();

		updateAsyncInput();
		updateSound();


		getRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		mEffectManager->Render();

		getRenderManager()->EndDraw();

		static float elapsed = 0.0f;

		elapsed += getTimeManager()->GetDeltaTime();

		if (elapsed > 2.f)
		{
			elapsed = 0.f;
		}

		mEffectManager->CreateVignetteEffect(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), { 1.f, 0.f,0.f }, elapsed);
		mEffectManager->CreateGaussianBlur(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), elapsed * 10);
		mEffectManager->CreateBlendGaussianBlur(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), elapsed, elapsed * 10);
		mEffectManager->CreateSpotLight(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), { 250,250, 100 }, { 250, 250, 0 }, elapsed * 10, 90);
		mEffectManager->CreateEdgeDetection(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), elapsed, elapsed * 3);
		mEffectManager->CreateDirectionBlur(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), elapsed, elapsed * 10);
		mEffectManager->CreateMorphology(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"), elapsed * 10, elapsed * 10);
		mEffectManager->CreateInvert(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"));
		mEffectManager->CreateGrayScale(getRenderManager()->GetBitmapOrNull(L"./image/monster2.png"));
	}

	void TestProjectJH::Release()
	{
		using namespace d2dFramework;

		mAsyncInputManager->SetIsEnd(true);
		mInputFuture.get();
		mEffectManager->Release();

		GameProcessor::Release();
	}

	void TestProjectJH::initAsyncInputManager()
	{
		mAsyncInputManager->AddHandleKey('W');
		mAsyncInputManager->AddHandleKey('A');
		mAsyncInputManager->AddHandleKey('S');
		mAsyncInputManager->AddHandleKey('D');

		mInputFuture = std::async(std::launch::async, [this]()
			{
				while (!mAsyncInputManager->GetIsEnd())
				{
					mAsyncInputManager->Update();
				}

				mAsyncInputManager->SetIsEnd(false);
			});
	}

	void TestProjectJH::updateAsyncInput()
	{
		using namespace d2dFramework;

		std::queue<KeyInformation> inputQueue(mAsyncInputManager->Flush());
		static double remainderTime = 0.0;
		static double time = 0.0;

		while (!inputQueue.empty())
		{
			KeyInformation& curInput = inputQueue.front();
			inputQueue.pop();

			time = static_cast<double>(curInput.Time.QuadPart - mBeatManager->GetStandardStartTime().QuadPart) / getTimeManager()->GetFrequency().QuadPart;

			remainderTime = MathHelper::GetDoubleRemainder(time, mBeatManager->GetBeatIntervalTime());


			// 비트에 맞는 입력 추적
			// mPlayerInputTime.push(MathHelper::GetDoubleRemainder(time, mBeatManager->GetBeatIntervalTime()));
		}

		getRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		getRenderManager()->WriteText(std::to_wstring(remainderTime).c_str(), { 100, 100, 400, 200 });
		getRenderManager()->WriteText(std::to_wstring(time).c_str(), { 200, 100, 400, 300 });
		getRenderManager()->WriteText(std::to_wstring(mBeatManager->GetBeatIntervalTime()).c_str(), { 300, 100, 500, 300 });
	}

	void TestProjectJH::updateSound()
	{
		using namespace d2dFramework;

		if (InputManager::GetInstance()->GetKeyState('E') == eKeyState::Push)
		{
			getSoundManager()->SetFrequencyAll(getSoundManager()->GetFrequencey() + 0.1f);
		}
		else if (InputManager::GetInstance()->GetKeyState('Q') == eKeyState::Push)
		{
			getSoundManager()->SetFrequencyAll(getSoundManager()->GetFrequencey() - 0.1f);
		}

		if (InputManager::GetInstance()->GetKeyState('Z') == eKeyState::Push)
		{
			getSoundManager()->SetPitchAll(getSoundManager()->GetPitch() + 0.1f);
		}
		else if (InputManager::GetInstance()->GetKeyState('X') == eKeyState::Push)
		{
			getSoundManager()->SetPitchAll(getSoundManager()->GetPitch() - 0.1f);
		}

		if (InputManager::GetInstance()->GetKeyState('P') == eKeyState::Push)
		{
			getSoundManager()->Play(mSoundId);
		}
		else if (InputManager::GetInstance()->GetKeyState('O') == eKeyState::Push)
		{
			getSoundManager()->Stop(mSoundId);
		}

		if (InputManager::GetInstance()->GetKeyState('I') == eKeyState::Push)
		{
			getSoundManager()->Play(mEffectId);
		}
		else if (InputManager::GetInstance()->GetKeyState('U') == eKeyState::Push)
		{
			getSoundManager()->Stop(mEffectId);
		}

		getRenderManager()->WriteText(std::to_wstring(getSoundManager()->GetFrequencey()).c_str(), { 100, 200, 300, 300 });

		getRenderManager()->WriteText(std::to_wstring(getSoundManager()->GetPitch()).c_str(), { 200, 200, 400, 300 });
	}
}