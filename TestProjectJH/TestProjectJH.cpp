#include "TestProjectJH.h"

#include "Scene.h"
#include "GameObject.h"
#include "MathHelper.h"
#include "SoundAsset.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Repeller.h"
#include "MonsterFSM.h"
#include "BaseMonsterComponent.h"
#include "FlyComponent.h"
#include "IncludeManager.h"
#include "IncludeComponent.h"
#include "MonsterFSM.h"
#include "PlayerFSM.h"
#include "BealzebubFSM.h"

#include <iostream>
#include <string>
#include <future>
#include <vector>

namespace testProjectJH
{
	unsigned int TestProjectJH::mTempId = 20000u;

	unsigned int TestProjectJH::GetTempId()
	{
		return mTempId++;
	}

	TestProjectJH::TestProjectJH(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mAsyncInputManager(new d2dFramework::AsyncInputManager())
		, mBeatManager(new d2dFramework::BeatManager())
		, mEffectManager(new d2dFramework::EffectManager())
		, mParticleManager(new d2dFramework::ParticleManager())
		, mMonsterFSM(new MonsterFSM())
		, mplayerFSM(new d2dFramework::PlayerFSM(92))
		, mBealzebubFSM(new BealzebubFSM())
	{
	}

	TestProjectJH::~TestProjectJH()
	{
		delete mAsyncInputManager;
		delete mBeatManager;
		delete mEffectManager;
		delete mParticleManager;
		delete mplayerFSM;
	}

	void TestProjectJH::Init(HWND hwnd)
	{
		using namespace d2dFramework;

		GameProcessor::Init(hwnd);
		mBeatManager->Init(DEFUALT_BPM, getTimeManager()->GetFrequency());

		initAsyncInputManager();
		//initSound();
		//initEffect(); //  이펙트 관련 렌더링 레이어 최적화 필요
		initParticle();

		int x = GRID_DISTANCE * MAX_X * 0.5f;
		int y = GRID_DISTANCE * MAX_Y * 0.5f;

		RenderManager::GetInstance()->SetGridObjectTransform(D2D1::Matrix3x2F::Translation(-x, -y) * getCameraManager()->GetCombineMatrix());

		getCollisionManager()->SetIsCollision(eObjectType::Player, eObjectType::Enemy);
		getCollisionManager()->SetIsCollision(eObjectType::Player, eObjectType::Projectile);

		initPlayerFSM();
		initBealzebubFSM();
		initMonsterFSM();
	}

	void TestProjectJH::Update()
	{
		using namespace d2dFramework;

		static float elapsed = 0.0f;
		ID2D1Bitmap* bitmap = nullptr;

		RenderManager::GetInstance()->BitmapBeginDraw();
		RenderManager::GetInstance()->Clear(D2D1::Matrix3x2F::Identity(), { 1,1,1,1 });
		{
			updateMonsterFSM();
			GameProcessor::Update();

			std::queue<KeyInformation> input = mAsyncInputManager->Flush();
			std::queue<KeyInformation> testinput = input;

			mplayerFSM->Update(getTimeManager()->GetDeltaTime(), input);
			showAsyncInputState(testinput);
			mBealzebubFSM->Update(getTimeManager()->GetDeltaTime());
			updateParticle();
			//updateAsyncInput();
			//updateSound();
		}
		RenderManager::GetInstance()->BitmapEndDraw();

		RenderManager::GetInstance()->BeginDraw();
		//getRen5derManager()->CopyBitmapRenderToHwndRender();
		// updateEffect();

		RenderManager::GetInstance()->CopyBitmapRenderToHwndRender();
		RenderManager::GetInstance()->EndDraw();
	}

	void TestProjectJH::Release()
	{
		using namespace d2dFramework;

		mAsyncInputManager->SetIsEnd(true);
		mInputFuture.get();
		//mEffectManager->Release();

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

	void TestProjectJH::initSound()
	{
		mSoundId = mTempId++;
		mEffectId = mTempId++;
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(mSoundId, "./sound/test_sound.ogg", true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(mEffectId, "./sound/ARTHURHIT.wav", false);
	}

	void TestProjectJH::initEffect()
	{
		using namespace d2dFramework;

		Scene* scene = getSceneManager()->GetCurrentScene();

		GameObject* gm = ObjectManager::GetInstance()->CreateObject(mTempId++);
		gm->CreateComponent<Transform>(mTempId++);
		SpriteRenderer* renderer = gm->CreateComponent<SpriteRenderer>(mTempId++);

		RenderManager::GetInstance()->CreateD2DBitmapFromFile(L"./image/monster2.png");

		ID2D1Bitmap* bitmap = RenderManager::GetInstance()->GetBitmapOrNull(L"./image/monster2.png");
		renderer->SetSize({ 1100, 700 });
		renderer->SetSpriteType(eSpriteType::Sprite);
		renderer->SetUVRectangle({ 0, 0, 500, 500 });
		renderer->SetBitmap(bitmap);

		mEffectManager->Init(RenderManager::GetInstance()->GetD2DDeviceContext());
		RenderManager::GetInstance()->CreateRadialGradientBrush();
	}

	void TestProjectJH::initParticle()
	{
		using namespace d2dFramework;

		mParticleID[0] = mTempId++;
		mParticleID[1] = mTempId++;
		mParticleID[2] = mTempId++;

		ParticleEmitter* particleEmitter;
		particleEmitter = mParticleManager->CreateParticleEmitter(mParticleID[0]);
		ParticleEmitterInformation info;
		ZeroMemory(&info, sizeof(info));
		info.MaxParticleCount = 10;
		info.OffsetHalfRange = { 1000, 100 };
		info.StartColor = { 1, 0, 0, 1 };
		info.StartColorHalfRange = { 0.5f, 0.5f, 0.5f, 0.2f };
		info.EndColor = { 0, 0, 0, 0.3 };
		info.EndColorHalfRange = { 0.2f, 0.2f, 0.2f, 0.5f };
		info.AngularVelocity;
		info.RotationInRadianHalfRange = 180;
		info.AngularVelocityHalfRange = 180;
		info.Position = { 900, 800 };
		info.Velocity = { 0, 300 };
		info.VelocityHalfRange = { 300, 500 };
		info.Acceleration = { 0, 1000 };
		info.AccelerationHalfRange = { 100, 300 };
		info.TimeToLive = 1.f;
		info.TimeToLiveHalfRange = 0.9f;
		info.StartScale = { 3, 3 };
		info.StartScaleHalfRange = { 2, 2 };
		info.EndScale = { 1, 1 };
		info.EndScaleHalfRange = { 0.5f, 0.5f };
		info.Size = { 10, 10 };
		particleEmitter->Init(info);

		particleEmitter = mParticleManager->CreateParticleEmitter(mParticleID[1]);
		info.MaxParticleCount = 1000;
		info.Velocity = { 400, 300 };
		info.Acceleration = { 700, 500 };
		particleEmitter->Init(info);

		particleEmitter = mParticleManager->CreateParticleEmitter(mParticleID[2]);
		info.MaxParticleCount = 1000;
		info.Velocity = { 400, 300 };
		info.Acceleration = { 700, 500 };
		particleEmitter->Init(info);


		std::vector<std::vector<FrameInformation>> ghostInfo =
		{
		};
		std::vector<std::vector<FrameInformation>> knightInfo =
		{
		};
		std::vector<std::vector<FrameInformation>> succubusInfo =
		{
		};

	}

	void TestProjectJH::initMonsterFSM()
	{
		using namespace d2dFramework;

		mMonsterFSM->Init(mBeatManager);

		//GameObject* monster = nullptr;
		//GameObject* monster = nullptr;
		//GridComponent* grid = nullptr;
		//GridComponent* grid = nullptr;
		//GameObject* player = nullptr;
		//GameObject* player = nullptr;
		//
		//
		//monster = createMonster();
		//monster = createMonster();
		//grid = monster->GetComponent<GridComponent>();
		//grid = monster->GetComponent<GridComponent>();
		//grid->SetPosition({ 4,4 });
		//grid->SetPosition({ 4,4 });
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);
		//
		//
		//monster = createMonster();
		//monster = createMonster();
		//grid = monster->GetComponent<GridComponent>();
		//grid = monster->GetComponent<GridComponent>();
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Escape);
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Escape);
		//grid->SetPosition({ 5,2 });
		//grid->SetPosition({ 5,2 });
		//
		//
		//monster = createMonster();
		//monster = createMonster();
		//grid = monster->GetComponent<GridComponent>();
		//grid = monster->GetComponent<GridComponent>();
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Escape);
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Escape);
		//grid->SetPosition({ 4,3 });
		//grid->SetPosition({ 4,3 });
		//
		//
		//monster = createMonster();
		//monster = createMonster();
		//grid = monster->GetComponent<GridComponent>();
		//grid = monster->GetComponent<GridComponent>();
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Attack);
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Attack);
		//grid->SetPosition({ 0,1 });
		//grid->SetPosition({ 0,1 });
		//
		//
		//monster = createMonster();
		//monster = createMonster();
		//grid = monster->GetComponent<GridComponent>();
		//grid = monster->GetComponent<GridComponent>();
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Attack);
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Attack);
		//grid->SetPosition({ 1,1 });
		//grid->SetPosition({ 1,1 });
		//
		//
		//monster = createMonster();
		//monster = createMonster();
		//grid = monster->GetComponent<GridComponent>();
		//grid = monster->GetComponent<GridComponent>();
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Attack);
		//monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Attack);
		//grid->SetPosition({ 2,1 });
		//grid->SetPosition({ 2,1 });

	}

	void TestProjectJH::initPlayerFSM()
	{
		using namespace d2dFramework;

		// 애니메이션 로딩
		using namespace d2dFramework;

		// 공격 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 2; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 1900.f / 2 * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 7.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5 + animationwidth, 950.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 7)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"PlayerAttack", L"./image/Player/PlayerAttack.png", attackAnimationAsset);
		}

		// 피격 애니메이션
		{
			std::vector<std::vector<FrameInformation>> beAttackedAnimationAsset;
			std::vector<FrameInformation> beAttackedAnmation;
			int animationCount = 0;

			for (int i = 0; i < 2; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 1900.f / 2 * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 7.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5 + animationwidth, 950.f + animationheight };
					animationCount++;
					beAttackedAnmation.push_back(animationinfo);

					if (animationCount >= 9)
						break;
				}
			}

			beAttackedAnimationAsset.push_back(beAttackedAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"PlayerBeAttacked", L"./image/Player/PlayerBeAttacked.png", beAttackedAnimationAsset);
		}
		// 죽음 애니메이션
		{
			std::vector<std::vector<FrameInformation>> deathAnimationAsset;
			std::vector<FrameInformation> deathAnmation;
			int animationCount = 0;

			for (int i = 0; i < 2; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 1900.f / 2 * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5 + animationwidth, 950.f + animationheight };
					animationCount++;
					deathAnmation.push_back(animationinfo);

					if (animationCount >= 10)
						break;
				}
			}
			deathAnimationAsset.push_back(deathAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"PlayerDeath", L"./image/Player/PlayerDeath.png", deathAnimationAsset);
		}

		// Idle 애니메이션
		{
			std::vector<std::vector<FrameInformation>> idleAnimationAsset;
			std::vector<FrameInformation> idleAnmation;
			for (int i = 0; i < 4; i++)
			{
				FrameInformation animationinfo;
				float animationwidth = 944.f * i;
				animationinfo.AnimationTime = 0.2f;
				animationinfo.UVRectangle = { 0.f + animationwidth, 0.f, 944.f + animationwidth, 950.f };

				idleAnmation.push_back(animationinfo);
			}
			idleAnimationAsset.push_back(idleAnmation);

			RenderManager::GetInstance()->CreateAnimationAsset(L"PlayerIdle", L"./image/Player/PlayerIdle.png", idleAnimationAsset);
		}

		// Move 애니메이션
		{
			std::vector<std::vector<FrameInformation>> moveAnimationAsset;
			std::vector<FrameInformation> moveAnmation;
			for (int i = 0; i < 1; i++)
			{
				FrameInformation animationinfo;
				float animationwidth = 1661.f * i;
				animationinfo.AnimationTime = 0.2f;
				animationinfo.UVRectangle = { 0.f + animationwidth, 0.f, 944.f + animationwidth, 950.f };

				moveAnmation.push_back(animationinfo);
			}
			moveAnimationAsset.push_back(moveAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"PlayerMove", L"./image/Player/PlayerIdle.png", moveAnimationAsset);
		}

		// 플레이어
		mPlayer = ObjectManager::GetInstance()->CreateObject(mTempId++);
		CircleCollider* colider = mPlayer->CreateComponent<CircleCollider>(mTempId++);
		Transform* playerTransform = mPlayer->CreateComponent<Transform>(mTempId++);
		AnimationRenderer* playerAnimation = mPlayer->CreateComponent<AnimationRenderer>(mTempId++);
		GridComponent* playerGrid = mPlayer->CreateComponent<GridComponent>(mTempId++);
		GridMoveComponent* playerGridmove = mPlayer->CreateComponent<GridMoveComponent>(mTempId++);

		mPlayer->SetObjectType(eObjectType::Player);

		playerAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"PlayerIdle"));
		playerAnimation->SetSize({ GRID_DISTANCE, GRID_DISTANCE });
		playerAnimation->SetIsActive(true);
		playerAnimation->SetIsLoop(true);
		playerAnimation->SetTransformLayer(eTransformLayer::Grid);

		playerGrid->SetCellDistance(GRID_DISTANCE);
		playerGrid->SetMaxX(MAX_X);
		playerGrid->SetMaxY(MAX_Y);
		playerGrid->SetPosition({ 3,3 });
		playerGridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		colider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		colider->SetIsTrigger(true);

		// 플레이어 유령 노드
		GameObject* playerNode = ObjectManager::GetInstance()->CreateObject(mTempId++);
		Transform* playerNodeTransform = playerNode->CreateComponent<Transform>(mTempId++);
		SpriteRenderer* playerNodeRenderer = playerNode->CreateComponent<SpriteRenderer>(mTempId++);

		playerNodeRenderer->SetSize({ 30, 30 });
		playerNodeRenderer->SetSpriteType(eSpriteType::Circle);
		playerNodeRenderer->SetBaseColor({ 1,1,1,1 });

		mplayerFSM->Init(mPlayer, playerNode, mBeatManager, getTimeManager());

		AnimationAsset* anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"PlayerIdle");
		mplayerFSM->SetPlayerAnimationAsset(L"PlayerIdle", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"PlayerMove");
		mplayerFSM->SetPlayerAnimationAsset(L"PlayerMove", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"PlayerAttack");
		mplayerFSM->SetPlayerAnimationAsset(L"PlayerAttack", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"PlayerBeAttacked");
		mplayerFSM->SetPlayerAnimationAsset(L"PlayerBeAttacked", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"PlayerDeath");
		mplayerFSM->SetPlayerAnimationAsset(L"PlayerDeath", anime);
	}

	void TestProjectJH::initBealzebubFSM()
	{
		using namespace d2dFramework;

		mBealzebubFSM->Init(mBeatManager, mMonsterFSM, mBealzebubId);

		// 플레이어
		GameObject* boss = ObjectManager::GetInstance()->CreateObject(mBealzebubId++);
		CircleCollider* colider = boss->CreateComponent<CircleCollider>(mBealzebubId++);
		Transform* transform = boss->CreateComponent<Transform>(mBealzebubId++);
		AnimationRenderer* animation = boss->CreateComponent<AnimationRenderer>(mBealzebubId++);
		GridComponent* grid = boss->CreateComponent<GridComponent>(mBealzebubId++);
		GridMoveComponent* gridmove = boss->CreateComponent<GridMoveComponent>(mBealzebubId++);

		boss->SetObjectType(eObjectType::Enemy);

		animation->SetSize({ GRID_DISTANCE, GRID_DISTANCE });
		animation->SetIsActive(true);
		animation->SetIsLoop(true);
		animation->SetTransformLayer(eTransformLayer::Grid);

		grid->SetCellDistance(GRID_DISTANCE);
		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		grid->SetPosition({ 7,7 });
		gridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		colider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		colider->SetIsTrigger(true);

		std::vector<std::vector<FrameInformation>> belzebubFrames =
		{
			{ // idle
				{ 0,0,944,1069,0.2f},
				{ 944,0,1888,1069,0.2f },
				{ 1888,0,2832,1069,0.2f },
				{ 2832,0,3776,1069,0.2f },
				{ 3776,0,4720,1069,0.2f },
			},
			{ // move
				{ 0,1069,944,2138,0.2f },
				{ 944,1069,1888,2138,0.2f },
				{ 1888,1069,2832,2138,0.2f },
			},
			{ // attack
{ 0,2138,944,3207,0.2f },
{ 944,2138,1888,3207,0.2f },
{ 1888,2138,2832,3207,0.2f },
{ 2832,2138,3776,3207,0.2f },
{ 3776,2138,4720,3207,0.2f },
			},
			{ // hit
{ 0,3207,944,4276,0.2f },
{ 944,3207,1888,4276,0.2f },
{ 1888,3207,2832,4276,0.2f },
{ 2832,3207,3776,4276,0.2f },
{ 3776,3207,4720,4276,0.2f },
			},
			{ // death
{ 0,4276,944,5345,0.2f },
{ 944,4276,1888,5345,0.2f },
{ 1888,4276,2832,5345,0.2f },
{ 2832,4276,3776,5345,0.2f },
{ 3776,4276,4720,5345,0.2f },
{ 0,5345,944,6414,0.2f },
{ 944,5345,1888,6414,0.2f },
{ 1888,5345,2832,6414,0.2f },
{ 2832,5345,3776,6414,0.2f },
{ 3776,5345,4720,6414,0.2f },
{ 0,6414,944,7483,0.2f },
{ 944,6414,1888,7483,0.2f },
{ 1888,6414,2832,7483,0.2f },
{ 2832,6414,3776,7483,0.2f },
{ 3776,6414,4720,7483,0.2f },
{ 0,7483,944,8552,0.2f },
{ 944,7483,1888,8552,0.2f },
{ 1888,7483,2832,8552,0.2f },
			}
		};
		RenderManager::GetInstance()->CreateAnimationAsset(L"belzebub", L"./image/Bealzebub/bealzebub.png", belzebubFrames);
		animation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"belzebub"));

		std::vector<std::vector<FrameInformation>> flyMove =
		{
			{
				{ 0, 0, 314, 475, 0.1f},
				{ 314, 0, 628, 475, 0.1f},
				{ 628, 0, 942, 475, 0.1f},
				{ 942, 0, 1256,	475, 0.1f},
				{ 1256, 0,1570, 475, 0.1f},
				{ 1570,	0,	1884, 475, 0.1f},
				{ 0	,475, 314, 950, 0.1f},
				{ 314, 475,	628, 950, 0.1f},
				{ 628, 475,	942, 950, 0.1f},
				{ 942, 475,	1256, 950, 0.1f},
				{ 1256, 475, 1570, 950, 0.1f},
				{ 1570, 475, 1884, 950, 0.1f},
			}
		};
		RenderManager::GetInstance()->CreateAnimationAsset(L"flyMove", L"./image/Bealzebub/fly_move.png", flyMove);
	}

	void TestProjectJH::updateAsyncInput()
	{
		using namespace d2dFramework;

		std::queue<KeyInformation> inputQueue = mAsyncInputManager->Flush();
		static double remainderTime = 0.0;
		static double time = 0.0;
		static double defferTime = 0.0;
		while (!inputQueue.empty())
		{
			KeyInformation& curInput = inputQueue.front();
			inputQueue.pop();

			// 요게 
			time = static_cast<double>(curInput.Time.QuadPart - mBeatManager->GetLastBPMChaneTime().QuadPart) / getTimeManager()->GetFrequency().QuadPart;

			defferTime = mBeatManager->GetIntervalDefferenceTime(curInput.Time);

			remainderTime = MathHelper::GetDoubleRemainder(time, mBeatManager->GetIntervalTime());

			// 비트에 맞는 입력 추적
			// mPlayerInputTime.push(MathHelper::GetDoubleRemainder(time, mBeatManager->GetIntervalTime()));
		}

		RenderManager::GetInstance()->SetColor({ 0, 0, 0, 1 });
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Identity());
		RenderManager::GetInstance()->WriteText(std::to_wstring(remainderTime).c_str(), { 100, 100, 400, 200 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(time).c_str(), { 200, 100, 400, 300 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(defferTime).c_str(), { 200, 400, 400, 500 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(mBeatManager->GetIntervalTime()).c_str(), { 300, 100, 500, 300 });
	}

	void TestProjectJH::updateSound()
	{
		using namespace d2dFramework;

		if (InputManager::GetInstance()->GetKeyState('E') == eKeyState::Push)
		{
			SoundManager::GetInstance()->SetFrequency(SoundManager::GetInstance()->GetFrequencey() + 0.1f);
		}
		else if (InputManager::GetInstance()->GetKeyState('Q') == eKeyState::Push)
		{
			SoundManager::GetInstance()->SetFrequency(SoundManager::GetInstance()->GetFrequencey() - 0.1f);
		}

		if (InputManager::GetInstance()->GetKeyState('Z') == eKeyState::Push)
		{
			SoundManager::GetInstance()->SetPitch(SoundManager::GetInstance()->GetPitch() + 0.1f);
		}
		else if (InputManager::GetInstance()->GetKeyState('X') == eKeyState::Push)
		{
			SoundManager::GetInstance()->SetPitch(SoundManager::GetInstance()->GetPitch() - 0.1f);
		}

		if (InputManager::GetInstance()->GetKeyState('P') == eKeyState::Push)
		{
			SoundManager::GetInstance()->Play(mSoundId, eSoundType::BGM, ePlayType::Overwrite, 0);
		}
		else if (InputManager::GetInstance()->GetKeyState('O') == eKeyState::Push)
		{
			SoundManager::GetInstance()->Play(mEffectId, eSoundType::Effect, ePlayType::Overwrite, 0);
		}

		if (InputManager::GetInstance()->GetKeyState('I') == eKeyState::Push)
		{
			SoundManager::GetInstance()->Pause(true);
		}
		else if (InputManager::GetInstance()->GetKeyState('U') == eKeyState::Push)
		{
			SoundManager::GetInstance()->Pause(false);
		}
		RenderManager::GetInstance()->SetColor({ 0, 0, 0, 1 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(SoundManager::GetInstance()->GetFrequencey()).c_str(), { 100, 200, 300, 300 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(SoundManager::GetInstance()->GetPitch()).c_str(), { 200, 200, 400, 300 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(getTimeManager()->GetFPS()).c_str(), { 300, 200, 400, 300 });
	}

	void TestProjectJH::updateEffect()
	{
		using namespace d2dFramework;

		ID2D1Bitmap* bitmap = nullptr;
		static float elapsed = 0.0f;

		ID2D1Bitmap* backBuffer = nullptr;
		RenderManager::GetInstance()->GetBitmapRenderTarget()->GetBitmap(&backBuffer);

		if (InputManager::GetInstance()->GetKeyState('1') == eKeyState::Hold)
		{
			bitmap = mEffectManager->CreateGrayScale(backBuffer);
		}
		else if (InputManager::GetInstance()->GetKeyState('2') == eKeyState::Hold) // n
		{
			bitmap = mEffectManager->CreateBlendGaussianBlur(backBuffer, elapsed);
		}
		else if (InputManager::GetInstance()->GetKeyState('3') == eKeyState::Hold)
		{
			bitmap = mEffectManager->CreateDirectionBlur(backBuffer, elapsed);
		}
		else if (InputManager::GetInstance()->GetKeyState('4') == eKeyState::Hold)
		{
			bitmap = mEffectManager->CreateEdgeDetection(backBuffer, elapsed);
		}
		else if (InputManager::GetInstance()->GetKeyState('5') == eKeyState::Hold) // n
		{
			bitmap = mEffectManager->CreateGaussianBlur(backBuffer, elapsed);
		}
		else if (InputManager::GetInstance()->GetKeyState('6') == eKeyState::Hold)
		{
			bitmap = mEffectManager->CreateInvert(backBuffer);
		}
		else if (InputManager::GetInstance()->GetKeyState('7') == eKeyState::Hold)
		{
			bitmap = mEffectManager->CreateMorphology(backBuffer, elapsed);
		}
		else if (InputManager::GetInstance()->GetKeyState('8') == eKeyState::Hold) // n
		{
			bitmap = mEffectManager->CreateSpotLight(backBuffer, { 980 ,500 ,1000 }, { 980 ,500, 0 }, elapsed, 45, 0.5f, { 1,1,1 });
		}
		else if (InputManager::GetInstance()->GetKeyState('9') == eKeyState::Hold)
		{
			bitmap = mEffectManager->CreateVignetteEffect(backBuffer, { 1,0,0 }, elapsed, elapsed);
		}

		if (InputManager::GetInstance()->GetKeyState('N') == eKeyState::Hold)
		{
			elapsed += 1.f;
		}
		else if (InputManager::GetInstance()->GetKeyState('M') == eKeyState::Hold)
		{
			elapsed -= 1.f;
		}

		if (elapsed < 0.f)
		{
			elapsed = 0.f;
		}

		if (bitmap != nullptr)
		{
			float width = static_cast<float>(GetWidth());
			float height = static_cast<float>(GetHeight());

			RenderManager::GetInstance()->BitmapBeginDraw();
			RenderManager::GetInstance()->DrawBitMap({ 0,0, 1902,1022 }, { 0,0, 1902,1022 }, bitmap);
			RenderManager::GetInstance()->BitmapEndDraw();

			bitmap->Release();
		}
	}

	void TestProjectJH::updateParticle()
	{
		using namespace d2dFramework;
		float deltatime = getTimeManager()->GetDeltaTime();

		float speed = 1000;
		Vector2 moveDistance;
		if (InputManager::GetInstance()->GetKeyState(VK_LEFT) == eKeyState::Hold)
		{
			moveDistance.SetX(-speed * deltatime);
		}
		else if (InputManager::GetInstance()->GetKeyState(VK_RIGHT) == eKeyState::Hold)
		{
			moveDistance.SetX(speed * deltatime);
		}
		if (InputManager::GetInstance()->GetKeyState(VK_UP) == eKeyState::Hold)
		{
			moveDistance.SetY(-speed * deltatime);
		}
		else if (InputManager::GetInstance()->GetKeyState(VK_DOWN) == eKeyState::Hold)
		{
			moveDistance.SetY(speed * deltatime);
		}

		// mParticleEmitter->AddForce({ 50000 * deltatime, 0 });
		//mParticleEmitter->ApplyGravity(9.8f * 2);
		//Repeller repeller1({ static_cast<float>(GetWidth() / 2), static_cast<float>(GetHeight() / 3 * 2) }, 100000);
		//Repeller repeller2({ static_cast<float>(GetWidth() / 2 + 100), static_cast<float>(GetHeight() / 3 * 2) }, 100000);
		//Repeller repeller3({ static_cast<float>(GetWidth() / 2 - 100), static_cast<float>(GetHeight() / 3 * 2) }, 100000);
		//repeller1.Render(RenderManager::GetInstance());
		//repeller2.Render(RenderManager::GetInstance());
		//repeller3.Render(RenderManager::GetInstance());
		//mParticleEmitter->ApplyRepeller(repeller1);
		//mParticleEmitter->ApplyRepeller(repeller2);
		//mParticleEmitter->ApplyRepeller(repeller3);

		if (InputManager::GetInstance()->GetKeyState(VK_SPACE) == eKeyState::Hold)
		{
			for (int i = 0; i < 3; ++i)
			{
				ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(mParticleID[i]);

				PE->SetIsEmit(true);
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(mParticleID[i]);

				PE->SetIsEmit(false);
			}
		}

		for (int i = 0; i < 3; ++i)
		{
			ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(mParticleID[i]);

			PE->Update(deltatime);
			PE->Render(RenderManager::GetInstance());
			PE->MovePosition(moveDistance);
		}
	}

	void TestProjectJH::updateMonsterFSM()
	{
		using namespace d2dFramework;

		mMonsterFSM->Update(getTimeManager()->GetDeltaTime());

		D2D1::Matrix3x2F matrix = getCameraManager()->GetCombineMatrix();
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Translation(MAX_X * GRID_DISTANCE * -0.5f, MAX_Y * GRID_DISTANCE * -0.5f) * matrix);
		RenderManager::GetInstance()->DrawGrid(0, 0, MAX_X * GRID_DISTANCE, MAX_Y * GRID_DISTANCE, GRID_DISTANCE);
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Identity());

		mMonsterFSM->DebugRender(RenderManager::GetInstance(), getCameraManager()->GetCombineMatrix());
	}

	d2dFramework::GameObject* TestProjectJH::createMonster()
	{
		using namespace d2dFramework;

		GameObject* monster = nullptr;
		Transform* transform = nullptr;
		AnimationRenderer* renderer = nullptr;
		GridComponent* grid = nullptr;
		GridMoveComponent* gridmove = nullptr;
		BaseMonsterComponent* baseMonsterComponent = nullptr;
		CircleCollider* collider = nullptr;

		monster = ObjectManager::GetInstance()->CreateObject(mTempId++);
		monster->SetObjectType(eObjectType::Enemy);
		transform = monster->CreateComponent<Transform>(mTempId++);
		renderer = monster->CreateComponent<AnimationRenderer>(mTempId++);
		renderer->SetTransformLayer(eTransformLayer::Grid);
		grid = monster->CreateComponent<GridComponent>(mTempId++);
		gridmove = monster->CreateComponent<GridMoveComponent>(mTempId++);
		baseMonsterComponent = monster->CreateComponent<testProjectJH::BaseMonsterComponent>(mTempId++);
		collider = monster->CreateComponent<CircleCollider>(mTempId++);

		renderer->SetSize({ 50,	50 });
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"belzebub"));
		renderer->SetIsLoop(true);

		transform->SetTranslate({ 0, 0 });
		grid->SetCellDistance(GRID_DISTANCE);
		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		gridmove->SetCurrentSpeed(0.5f);

		baseMonsterComponent->SetDetectionArea(3);
		baseMonsterComponent->SetAttackArea(0);
		baseMonsterComponent->SetMoveableBeatCount(1);
		EventManager::GetInstance()->RegisterBroadcastEvent("RegisterMonster", std::to_string(monster->GetId()));

		grid->SetPosition({ 0,0 });

		collider->SetRadius(25);

		return monster;
	}

	void TestProjectJH::showAsyncInputState(std::queue<d2dFramework::KeyInformation>& testQueue)
	{
		using namespace d2dFramework;

		static double time = 0.0;
		static double Delaytime = 0.0;
		static double defferTime = 0.0;
		static double chanTime = 0.0;

		while (!testQueue.empty())
		{
			KeyInformation& curInput = testQueue.front();
			testQueue.pop();

			// 요게 
			time = static_cast<double>(curInput.Time.QuadPart - mBeatManager->GetLastBPMChaneTime().QuadPart) / getTimeManager()->GetFrequency().QuadPart;
			Delaytime = static_cast<double>(getTimeManager()->GetCurTime().QuadPart - mBeatManager->GetLastBPMChaneTime().QuadPart) / getTimeManager()->GetFrequency().QuadPart;
			defferTime = mBeatManager->GetIntervalDefferenceTime(curInput.Time);
			chanTime = mBeatManager->GetLastBPMChaneTime().QuadPart;

			// 비트에 맞는 입력 추적
			// mPlayerInputTime.push(MathHelper::GetDoubleRemainder(time, mBeatManager->GetIntervalTime()));
		}

		RenderManager::GetInstance()->WriteText(std::to_wstring(time).c_str(), { 200, 100, 400, 300 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(Delaytime).c_str(), { 200, 200, 400, 300 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(chanTime).c_str(), { 200, 300, 400, 300 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(defferTime).c_str(), { 200, 400, 400, 500 });
		RenderManager::GetInstance()->WriteText(std::to_wstring(mBeatManager->GetIntervalTime()).c_str(), { 300, 100, 500, 300 });
	}

}