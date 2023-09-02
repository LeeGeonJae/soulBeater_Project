#include "SoulBeaterProcessor.h"

#include "SceneLoader.h"

#include "IncludeComponent.h"
#include "IncludeManager.h"

#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "AnimationRenderer.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "CircleCollider.h"
#include "PlayerFSM.h"
#include "MonsterFSM.h"
#include "YsBossFSM.h"
#include "EveBossFSM.h"
#include "LilithBossFSM.h"
#include "BealzebubFSM.h"
#include "BealzebubComponent.h"
#include "BaseMonsterComponent.h"
#include "EveBossComponent.h"
#include "PlayerComponent.h"

#include "KeyInformation.h"
#include "YsBossComponent.h"
#include "NodeComponent.h"
#include "LilithBossComponent.h"

#include <queue>

namespace TestGeon
{
	unsigned int SoulBeaterProcessor::mTempId = 20000;

	SoulBeaterProcessor::SoulBeaterProcessor(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mBeatManager(new d2dFramework::BeatManager)
		, mAsyncInputManager(new d2dFramework::AsyncInputManager)
		, mplayerFSM(new PlayerFSM(80000))
		, mMonsterFSM(new MonsterFSM())
		, mYsBossFSM(new YsBossFSM())
		, mEveBossFSM(new EveBossFSM())
		, mLilithBossFSM(new LilithBossFSM())
		, mBealzebubBossFSM(new BealzebubFSM())
		, mPlayer(nullptr)
		, mYsBoss(nullptr)
		, mEveBoss(nullptr)
		, mLilithBoss(nullptr)
		, mBealzebubBoss(nullptr)
		, mMonstercollider(nullptr)
	{
	}

	void SoulBeaterProcessor::Init(HWND hwnd)
	{
		using namespace d2dFramework;

		GameProcessor::Init(hwnd);

		// register Register Scene;
		SceneLoader::LoadAllBitmaps();
		Scene* scene = getSceneManager()->CreateScene<Scene>((unsigned int)10000);
		SceneLoader::LoadScene(scene);
		getSceneManager()->SetCurrentScene(10000);

		// 매니저 세팅
		initAsyncInputManager();

		//int x = GRID_DISTANCE * MAX_X * 0.5f;
		//int y = GRID_DISTANCE * MAX_Y * 0.5f;

		//RenderManager::GetInstance()->SetGridObjectTransform(D2D1::Matrix3x2F::Translation(-x, -y) * getCameraManager()->GetCombineMatrix());

		// 몬스터 세팅
		initMonsterFSM();

		// Player 애니메이션 세팅
		initPlayerAnimation();
		initYsAnimation();
		initEveAnimation();
		initLilithAnimation();
		initBealzebubAnimation();
		initMonsterAnimation();

		// 오브젝트 등록 ( Player, YsAndEve )
		initPlayerSetting();
		initYsSetting();
		initEveSetting();
		initLilithSetting();
		initBealzebubSetting();

		tileLoadSetting();

		mYsBossFSM->Init(mBeatManager, mMonsterFSM, mYsBoss->GetId(), mEveBoss->GetId(), mPlayer->GetId() );
		mEveBossFSM->Init(mBeatManager, mMonsterFSM, mYsBoss->GetId(), mEveBoss->GetId(), mPlayer->GetId() );
		//mLilithBossFSM->Init(mBeatManager, mLilithBoss->GetId(), mMonsterFSM);
		mBealzebubBossFSM->Init(mBeatManager, mMonsterFSM, mBealzebubBoss->GetId());

		getCollisionManager()->SetIsCollision(eObjectType::Enemy, eObjectType::Player);
		getCollisionManager()->SetIsCollision(eObjectType::Projectile, eObjectType::Player);
	}

	void SoulBeaterProcessor::Update()
	{
		using namespace d2dFramework;

		RenderManager::GetInstance()->BitmapBeginDraw();
		{
			updateMonsterFSM();
			GameProcessor::Update();

			std::queue<KeyInformation> input = mAsyncInputManager->Flush();
			std::queue<KeyInformation> testinput = input;

			mplayerFSM->Update(getTimeManager()->GetDeltaTime(), input);
			mYsBossFSM->Update(getTimeManager()->GetDeltaTime());
			mEveBossFSM->Update(getTimeManager()->GetDeltaTime());
			mBealzebubBossFSM->Update(getTimeManager()->GetDeltaTime());
			mLilithBossFSM->Update(getTimeManager()->GetDeltaTime());
			showAsyncInputState(testinput);

			// draw mouse pos
			auto& pos = InputManager::GetInstance()->GetMousePos();
			RenderManager::GetInstance()->SetStrokeWidth(10);
			RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Identity());
			RenderManager::GetInstance()->DrawPoint({ static_cast<float>(pos.x), static_cast<float>(pos.y) });
			RenderManager::GetInstance()->SetStrokeWidth(2);


			D2D1::Matrix3x2F matrix = getCameraManager()->GetCombineMatrix();
			RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Translation((MAX_X)*GRID_DISTANCE * -0.5f, (MAX_Y - 2) * GRID_DISTANCE * -0.5f) * matrix);
			RenderManager::GetInstance()->DrawGrid(0, 0, MAX_X * GRID_DISTANCE, MAX_Y * GRID_DISTANCE, GRID_DISTANCE);
			RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Identity());

			//updateColliderShow();
		}
		RenderManager::GetInstance()->BitmapEndDraw();

		RenderManager::GetInstance()->BeginDraw();
		RenderManager::GetInstance()->CopyBitmapRenderToHwndRender();
		RenderManager::GetInstance()->EndDraw();
	}

	void SoulBeaterProcessor::Release()
	{
		using namespace d2dFramework;

		mplayerFSM->Release();
		mMonsterFSM->Release();
		mYsBossFSM->Release();
		mEveBossFSM->Release();
		//monstercollider->UnregisterOnEnterCollisionCallBack(monstercollider->GetGameObject()->GetId());
		
		mAsyncInputManager->SetIsEnd(true);
		mInputFuture.get();
		GameProcessor::Release();
	}

	void SoulBeaterProcessor::initMonsterFSM()
	{
		using namespace d2dFramework;

		mMonsterFSM->Init(mBeatManager);
	}

	void SoulBeaterProcessor::initAsyncInputManager()
	{
		mBeatManager->Init(BEAT_BPM, getTimeManager()->GetFrequency());
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

	void SoulBeaterProcessor::initPlayerSetting()
	{
		using namespace d2dFramework;

		// 플레이어
		mPlayer = ObjectManager::GetInstance()->CreateObject(mTempId++);
		CircleCollider* colider = mPlayer->CreateComponent<CircleCollider>(mTempId++);
		Transform* playerTransform = mPlayer->CreateComponent<Transform>(mTempId++);
		AnimationRenderer* playerAnimation = mPlayer->CreateComponent<AnimationRenderer>(mTempId++);
		GridComponent* playerGrid = mPlayer->CreateComponent<GridComponent>(mTempId++);
		GridMoveComponent* playerGridmove = mPlayer->CreateComponent<GridMoveComponent>(mTempId++);
		PlayerComponent* playercomponent = mPlayer->CreateComponent<PlayerComponent>(mTempId++);

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
		playerGridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime() / 2);

		colider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		colider->SetIsTrigger(true);


		// 플레이어 유령 노드
		GameObject* playerNode = ObjectManager::GetInstance()->CreateObject(mTempId++);
		Transform* playerNodeTransform = playerNode->CreateComponent<Transform>(mTempId++);
		GridComponent* playerNodeGrid = playerNode->CreateComponent<GridComponent>(mTempId++);
		NodeComponent* playerNodeRenderer = playerNode->CreateComponent<NodeComponent>(mTempId++);

		playerNode->SetObjectType(eObjectType::Node);

		playerNodeRenderer->SetSize({ GRID_DISTANCE, GRID_DISTANCE });
		playerNodeRenderer->SetSpriteType(eSpriteType::Sprite);
		playerNodeRenderer->SetBaseColor({ 0,1,0,0.5 });
		playerNodeRenderer->SetTransformLayer(eTransformLayer::Grid);
		playerNodeRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"PlayerNode"));
		playerNodeRenderer->SetUVRectangle({ 0,0,240,240 });

		playerNodeGrid->SetCellDistance(GRID_DISTANCE);
		playerNodeGrid->SetMaxX(MAX_X);
		playerNodeGrid->SetMaxY(MAX_Y);
		playerNodeGrid->SetPosition({ playerGrid->GetPosition().X, playerGrid->GetPosition().Y });

		mplayerFSM->Init(mPlayer->GetId(), playerNode->GetId(), mBeatManager, getTimeManager());

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

		RenderManager::GetInstance()->SetGridObjectTransform(mPlayer->GetComponent<Transform>()->GetInverseTransform() * getCameraManager()->GetCombineMatrix());
	}

	void SoulBeaterProcessor::initYsSetting()
	{
		using namespace d2dFramework;

		// 플레이어
		mYsBoss = ObjectManager::GetInstance()->CreateObject(mTempId++);
		CircleCollider* colider = mYsBoss->CreateComponent<CircleCollider>(mTempId++);
		Transform* bossTransform = mYsBoss->CreateComponent<Transform>(mTempId++);
		AnimationRenderer* bossAnimation = mYsBoss->CreateComponent<AnimationRenderer>(mTempId++);
		GridComponent* bossGrid = mYsBoss->CreateComponent<GridComponent>(mTempId++);
		GridMoveComponent* bossGridmove = mYsBoss->CreateComponent<GridMoveComponent>(mTempId++);
		YsBossComponent* bossComponent = mYsBoss->CreateComponent<YsBossComponent>(mTempId++);

		mYsBoss->SetObjectType(eObjectType::Boss);

		bossAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"YsIdle"));
		bossAnimation->SetSize({ GRID_DISTANCE + 20, GRID_DISTANCE + 20 });
		bossAnimation->SetIsActive(true);
		bossAnimation->SetIsLoop(true);
		bossAnimation->SetTransformLayer(eTransformLayer::Grid);

		bossGrid->SetCellDistance(GRID_DISTANCE);
		bossGrid->SetMaxX(MAX_X);
		bossGrid->SetMaxY(MAX_Y);
		bossGrid->SetPosition({ MAX_X- 1, 1 });
		bossGridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		colider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		colider->SetIsTrigger(true);

		AnimationAsset* anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"YsIdle");
		bossComponent->SetYsBossAnimationAsset(L"YsIdle", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"YsMove");
		bossComponent->SetYsBossAnimationAsset(L"YsMove", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"YsAttack");
		bossComponent->SetYsBossAnimationAsset(L"YsAttack", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"YsBeAttacked");
		bossComponent->SetYsBossAnimationAsset(L"YsBeAttacked", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"YsDeath");
		bossComponent->SetYsBossAnimationAsset(L"YsDeath", anime);

	}

	void SoulBeaterProcessor::initPlayerAnimation()
	{
		using namespace d2dFramework;

		RenderManager::GetInstance()->CreateD2DBitmapFromFile(L"PlayerNode", L"./image/Player/PlayerNode.png");

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
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 50.0;
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

			for (int i = 0; i < 3; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j ;
					float animationheight = 2850.f / 3 * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5 + animationwidth, 950.f + animationheight };
					animationCount++;
					deathAnmation.push_back(animationinfo);

					if (animationCount >= 11)
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

		// 경고 애니메이션
		{
			std::vector<std::vector<FrameInformation>> warningAnimationAsset;
			std::vector<FrameInformation> warningAnmation;
			for (int i = 0; i < 4; i++)
			{
				FrameInformation animationinfo;
				float animationwidth = 7680.f / 4 * i;
				animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 4;
				animationinfo.UVRectangle = { 0.f + animationwidth, 0.f, 7680.f / 4 + animationwidth, 1080.f };

				warningAnmation.push_back(animationinfo);
			}
			warningAnimationAsset.push_back(warningAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"Warning", L"./image/Object/Warning.png", warningAnimationAsset);
		}
	}

	void SoulBeaterProcessor::initYsAnimation()
	{
		using namespace d2dFramework;

		// 이스 공격 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 3; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 12.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 12)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"YsAttack", L"./image/Ys/YsAttack.png", attackAnimationAsset);
		}

		// 이스 이동 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			FrameInformation animationinfo;

			float animationwidth = 900.f;
			float animationheight = 1069.f;
			animationinfo.AnimationTime = mBeatManager->GetIntervalTime();
			animationinfo.UVRectangle = { 0.f, 0.f, animationwidth, animationheight };
			attackAnmation.push_back(animationinfo);

			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"YsMove", L"./image/Ys/YsMove.png", attackAnimationAsset);
		}

		// 이스 죽음 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 5; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 25)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"YsDeath", L"./image/Ys/YsDeath.png", attackAnimationAsset);
		}

		// 이스 피격 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 2; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 6.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 6)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"YsBeAttacked", L"./image/Ys/YsBeAttacked.png", attackAnimationAsset);
		}

		// 이스 idle 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 1; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 4; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 4.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 4)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"YsIdle", L"./image/Ys/YsIdle.png", attackAnimationAsset);
		}
	}

	void SoulBeaterProcessor::initEveSetting()
	{
		using namespace d2dFramework;

		mEveBoss = ObjectManager::GetInstance()->CreateObject(mTempId++);
		CircleCollider* colider = mEveBoss->CreateComponent<CircleCollider>(mTempId++);
		Transform* bossTransform = mEveBoss->CreateComponent<Transform>(mTempId++);
		AnimationRenderer* bossAnimation = mEveBoss->CreateComponent<AnimationRenderer>(mTempId++);
		GridComponent* bossGrid = mEveBoss->CreateComponent<GridComponent>(mTempId++);
		GridMoveComponent* bossGridmove = mEveBoss->CreateComponent<GridMoveComponent>(mTempId++);
		EveBossComponent* bossComponent = mEveBoss->CreateComponent<EveBossComponent>(mTempId++);

		mEveBoss->SetObjectType(eObjectType::Boss);

		bossAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveIdle"));
		bossAnimation->SetSize({ GRID_DISTANCE, GRID_DISTANCE });
		bossAnimation->SetIsActive(true);
		bossAnimation->SetIsLoop(true);
		bossAnimation->SetTransformLayer(eTransformLayer::Grid);

		bossGrid->SetCellDistance(GRID_DISTANCE);
		bossGrid->SetMaxX(MAX_X);
		bossGrid->SetMaxY(MAX_Y);
		bossGrid->SetPosition({ 1, 1 });
		bossGridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		colider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		colider->SetIsTrigger(true);

		AnimationAsset* anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveIdle");
		bossComponent->SetYsBossAnimationAsset(L"EveIdle", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveMove");
		bossComponent->SetYsBossAnimationAsset(L"EveMove", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveAttack");
		bossComponent->SetYsBossAnimationAsset(L"EveAttack", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveBeAttacked");
		bossComponent->SetYsBossAnimationAsset(L"EveBeAttacked", anime);
		anime = RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveDeath");
		bossComponent->SetYsBossAnimationAsset(L"EveDeath", anime);
	}

	void SoulBeaterProcessor::initEveAnimation()
	{
		using namespace d2dFramework;

		// 이브 공격 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 2; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 10.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 10)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"EveAttack", L"./image/Eve/EveAttack.png", attackAnimationAsset);
		}

		// 이스 이동 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			FrameInformation animationinfo;

			float animationwidth = 900.f;
			float animationheight = 1069.f;
			animationinfo.AnimationTime = mBeatManager->GetIntervalTime();
			animationinfo.UVRectangle = { 0.f, 0.f, animationwidth, animationheight };
			attackAnmation.push_back(animationinfo);

			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"EveMove", L"./image/Eve/EveMove.png", attackAnimationAsset);
		}

		// 이스 죽음 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 5; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 23)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"EveDeath", L"./image/Eve/EveDeath.png", attackAnimationAsset);
		}

		// 이스 피격 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 2; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 9.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 9)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"EveBeAttacked", L"./image/Eve/EveBeAttacked.png", attackAnimationAsset);
		}

		// 이스 idle 애니메이션
		{
			std::vector<std::vector<FrameInformation>> attackAnimationAsset;
			std::vector<FrameInformation> attackAnmation;
			int animationCount = 0;

			for (int i = 0; i < 1; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 4; j++)
				{
					float animationwidth = 900.f * j;
					float animationheight = 1069.f * i;
					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 4.0;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 900.f + animationwidth, 1069.f + animationheight };
					animationCount++;
					attackAnmation.push_back(animationinfo);

					if (animationCount >= 4)
						break;
				}
			}
			attackAnimationAsset.push_back(attackAnmation);
			RenderManager::GetInstance()->CreateAnimationAsset(L"EveIdle", L"./image/Eve/EveIdle.png", attackAnimationAsset);
		}
	}

	void SoulBeaterProcessor::initMonsterAnimation()
	{
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

	void SoulBeaterProcessor::initLilithAnimation()
	{
		// 릴리스 idle 애니메이션
		{
			std::vector<std::vector<FrameInformation>> lilithAnimationAsset;
			std::vector<FrameInformation> LilithAnmation;
			int animationChange = 0;
			int animationMoveCount = 0;
			int animationIdleCount = 0;
			int animationAttackCount = 0;
			int animationBeAttackedCount = 0;
			int animationDeathCount = 0;

			for (int i = 0; i < 8; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 8552.f / 8 * i;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5.f + animationwidth, 8552.f / 8.f + animationheight };

					switch (animationChange)
					{
					case 0:
					{
						animationMoveCount++;

						animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 1.0;
						LilithAnmation.push_back(animationinfo);

						if (animationMoveCount >= 1)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 1:
					{
						animationIdleCount++;

						animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 4.0;
						LilithAnmation.push_back(animationinfo);

						if (animationIdleCount >= 4)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 2:
					{
						animationAttackCount++;

						animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
						LilithAnmation.push_back(animationinfo);

						if (animationAttackCount >= 5)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 3:
					{
						animationBeAttackedCount++;

						animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
						LilithAnmation.push_back(animationinfo);

						if (animationBeAttackedCount >= 6)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 4:
					{
						animationDeathCount++;

						animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 12.0;
						LilithAnmation.push_back(animationinfo);

						if (animationDeathCount >= 12)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					}
				}
			}
			RenderManager::GetInstance()->CreateAnimationAsset(L"Lilith", L"./image/Lilith/Lilith.png", lilithAnimationAsset);
		}
	}

	void SoulBeaterProcessor::initLilithSetting()
	{
		using namespace d2dFramework;

		// 릴리스
		mLilithBoss = ObjectManager::GetInstance()->CreateObject(mTempId++);
		CircleCollider* colider = mLilithBoss->CreateComponent<CircleCollider>(mTempId++);
		Transform* bossTransform = mLilithBoss->CreateComponent<Transform>(mTempId++);
		AnimationRenderer* bossAnimation = mLilithBoss->CreateComponent<AnimationRenderer>(mTempId++);
		GridComponent* bossGrid = mLilithBoss->CreateComponent<GridComponent>(mTempId++);
		GridMoveComponent* bossGridmove = mLilithBoss->CreateComponent<GridMoveComponent>(mTempId++);
		LilithBossComponent* bossComponent = mLilithBoss->CreateComponent<LilithBossComponent>(mTempId++);

		mLilithBoss->SetObjectType(eObjectType::Boss);

		bossAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"Lilith"));
		bossAnimation->SetSize({ GRID_DISTANCE, GRID_DISTANCE });
		bossAnimation->SetIsActive(true);
		bossAnimation->SetIsLoop(true);
		bossAnimation->SetTransformLayer(eTransformLayer::Grid);

		bossGrid->SetCellDistance(GRID_DISTANCE);
		bossGrid->SetMaxX(MAX_X);
		bossGrid->SetMaxY(MAX_Y);
		bossGrid->SetPosition({ MAX_X - 1, 1 });
		bossGridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		colider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		colider->SetIsTrigger(true);
	}

	void SoulBeaterProcessor::initBealzebubAnimation()
	{
		std::vector<std::vector<FrameInformation>> BealzebubAnimationAsset;
		std::vector<FrameInformation> BealzebubAnmation;
		int animationChange = 0;
		int animationMoveCount = 0;
		int animationAttackCount = 0;
		int animationBeAttackedCount = 0;
		int animationDeathCount = 0;

		for (int i = 0; i < 8; i++)
		{
			FrameInformation animationinfo;

			for (int j = 0; j < 5; j++)
			{
				float animationwidth = 4720.f / 5 * j;
				float animationheight = 8552.f / 8 * i;
				animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5.f + animationwidth, 8552.f / 8.f + animationheight };

				switch (animationChange)
				{
				case 0:
				{
					animationMoveCount++;

					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 8.0;
					BealzebubAnmation.push_back(animationinfo);

					if (animationMoveCount >= 8)
					{
						BealzebubAnimationAsset.push_back(BealzebubAnmation);
						BealzebubAnmation.clear();

						animationChange++;
						j = 5;
					}
				}
				break;
				case 1:
				{
					animationAttackCount++;

					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
					BealzebubAnmation.push_back(animationinfo);

					if (animationAttackCount >= 5)
					{
						BealzebubAnimationAsset.push_back(BealzebubAnmation);
						BealzebubAnmation.clear();

						animationChange++;
						j = 5;
					}
				}
				break;
				case 2:
				{
					animationBeAttackedCount++;

					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 5.0;
					BealzebubAnmation.push_back(animationinfo);

					if (animationBeAttackedCount >= 5)
					{
						BealzebubAnimationAsset.push_back(BealzebubAnmation);
						BealzebubAnmation.clear();

						animationChange++;
						j = 5;
					}
				}
				break;
				case 3:
				{
					animationDeathCount++;

					animationinfo.AnimationTime = mBeatManager->GetIntervalTime() / 17.0;
					BealzebubAnmation.push_back(animationinfo);

					if (animationDeathCount >= 17)
					{
						BealzebubAnimationAsset.push_back(BealzebubAnmation);
						BealzebubAnmation.clear();

						animationChange++;
						j = 5;
					}
				}
				break;
				}
			}
		}
		RenderManager::GetInstance()->CreateAnimationAsset(L"Bealzebub", L"./image/Bealzebub/bealzebub.png", BealzebubAnimationAsset);
	}

	void SoulBeaterProcessor::initBealzebubSetting()
	{
		mBealzebubBoss = ObjectManager::GetInstance()->CreateObject(mTempId++);
		GridComponent* grid = mBealzebubBoss->CreateComponent<GridComponent>(mTempId++);
		GridMoveComponent* gridmove = mBealzebubBoss->CreateComponent<GridMoveComponent>(mTempId++);
		Transform* transform = mBealzebubBoss->CreateComponent<Transform>(mTempId++);
		CircleCollider* collider = mBealzebubBoss->CreateComponent<CircleCollider>(mTempId++);
		AnimationRenderer* renderer = mBealzebubBoss->CreateComponent<AnimationRenderer>(mTempId++);
		BealzebubComponent* bealzebubComponent = mBealzebubBoss->CreateComponent<BealzebubComponent>(mTempId++);

		mBealzebubBoss->SetObjectType(eObjectType::Boss);

		renderer->SetSize({ GRID_DISTANCE,GRID_DISTANCE });
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"Bealzebub"));
		renderer->SetTransformLayer(eTransformLayer::Grid);
		renderer->SetAnimationIndex(0);

		grid->SetCellDistance(GRID_DISTANCE);
		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		grid->SetPosition({ MAX_X - 1, 1 });
		gridmove->SetCurrentSpeed(0.3f);

		collider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		collider->SetIsTrigger(true);
	}

	void SoulBeaterProcessor::updateMonsterFSM()
	{
		using namespace d2dFramework;

		mMonsterFSM->Update(getTimeManager()->GetDeltaTime());

		D2D1::Matrix3x2F matrix = getCameraManager()->GetCombineMatrix();
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Translation((MAX_X) * GRID_DISTANCE * -0.5f, (MAX_Y - 2) * GRID_DISTANCE * -0.5f) * matrix);
		RenderManager::GetInstance()->DrawGrid(0, 0, MAX_X * GRID_DISTANCE, MAX_Y * GRID_DISTANCE, GRID_DISTANCE);
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Identity());

		mMonsterFSM->DebugRender(RenderManager::GetInstance(), getCameraManager()->GetCombineMatrix());
	}

	void SoulBeaterProcessor::updateColliderShow()
	{
		using namespace d2dFramework;

		D2D1::Matrix3x2F matrix = getCameraManager()->GetCombineMatrix();
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Translation((MAX_X) * GRID_DISTANCE * -0.5f, (MAX_Y - 2) * GRID_DISTANCE * -0.5f) * matrix);
		CircleCollider* circlecollider = mPlayer->GetComponent<CircleCollider>();
		RenderManager::GetInstance()->DrawCircle({ mPlayer->GetComponent<Transform>()->GetTranslate().GetX()
			, mPlayer->GetComponent<Transform>()->GetTranslate().GetY()
			, mPlayer->GetComponent<GridComponent>()->GetCellDistance() / 2.f - 10.f
			, mPlayer->GetComponent<GridComponent>()->GetCellDistance() / 2.f - 10.f });
		RenderManager::GetInstance()->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void SoulBeaterProcessor::showAsyncInputState(std::queue<d2dFramework::KeyInformation>& testQueue)
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

	void SoulBeaterProcessor::tileLoadSetting()
	{
		auto NoneTileIDs = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Background);

		std::queue<GameObject*> objectQueue = ObjectManager::GetInstance()->GetCreateObjectQueue();

		while (!objectQueue.empty())
		{
			GameObject* tile = objectQueue.front();
			objectQueue.pop();

			if (tile->GetObjectType() == eObjectType::Background)
			{
				AABBCollider* collider = tile->GetComponent<AABBCollider>();

				if (collider != nullptr)
				{
					tile->SetObjectType(eObjectType::Wall);
				}
				else
				{
					tile->SetObjectType(eObjectType::Tile);
				}

				GridComponent* grid = tile->GetComponent<GridComponent>();
				grid->SetPosition(grid->GetPosition());
				tile->GetComponent<SpriteRenderer>()->SetTransformLayer(eTransformLayer::Grid);
			}
		}
	}

	d2dFramework::GameObject* SoulBeaterProcessor::CreateMonster()
	{
		using namespace d2dFramework;

		GameObject* monster = nullptr;
		Transform* transform = nullptr;
		SpriteRenderer* renderer = nullptr;
		GridComponent* grid = nullptr;
		GridMoveComponent* gridmove = nullptr;
		CircleCollider* collider = nullptr;
		TestGeon::BaseMonsterComponent* baseMonsterComponent = nullptr;

		monster = ObjectManager::GetInstance()->CreateObject(mTempId++);
		monster->SetObjectType(eObjectType::Enemy);
		transform = monster->CreateComponent<Transform>(mTempId++);
		renderer = monster->CreateComponent<SpriteRenderer>(mTempId++);
		grid = monster->CreateComponent<GridComponent>(mTempId++);
		gridmove = monster->CreateComponent<GridMoveComponent>(mTempId++);
		collider = monster->CreateComponent<CircleCollider>(mTempId++);
		baseMonsterComponent = monster->CreateComponent<TestGeon::BaseMonsterComponent>(mTempId++);

		renderer->SetSize({ 50,	50 });
		renderer->SetSpriteType(eSpriteType::Circle);
		renderer->SetBaseColor({ 1,1,1,0.5 });
		renderer->SetTransformLayer(eTransformLayer::Grid);

		transform->SetTranslate({ 0, 0 });
		grid->SetCellDistance(GRID_DISTANCE);
		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		gridmove->SetCurrentSpeed(0.3f);

		collider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		collider->SetIsTrigger(true);

		baseMonsterComponent->SetDetectionArea(5);
		baseMonsterComponent->SetAttackArea(0);
		baseMonsterComponent->SetMoveableBeatCount(3);
		EventManager::GetInstance()->RegisterBroadcastEvent("RegisterMonster", std::to_string(monster->GetId()));

		grid->SetPosition({ 0,0 });

		return monster;
	}
}