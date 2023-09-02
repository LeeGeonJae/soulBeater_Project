#include "SoulBeaterProcessor.h"

#include "PlayerComponent.h"
#include "HPComponent.h"
#include "TimerComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "Loader.h"
#include "IncludeComponent.h"
#include "IncludeManager.h"
#include "PlayerFSM.h"
#include "MonsterFSM.h"
#include "BealzebubFSM.h"
#include "ConstantTable.h"
#include "YsBossFSM.h"
#include "EveBossFSM.h"
#include "LilithBossFSM.h"
#include "eSoulBeaterID.h"
#include "SoulBeaterScene.h"
#include "SceneLoader.h"
#include "ObjectHelper.h"
#include "UIManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include "ConstantTable.h"
#include "ObjectHelper.h"

#include <string>

namespace soulBeater
{
	unsigned int SoulBeaterProcessor::mTempId = 98765000;

	unsigned int SoulBeaterProcessor::GetTempId()
	{
		return mTempId++;
	}

	SoulBeaterProcessor::SoulBeaterProcessor(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mBeatManager(new d2dFramework::BeatManager)
		, mEffectManager(new d2dFramework::EffectManager)
		, mParticleManager(new d2dFramework::ParticleManager)
		, mAsyncInputManager(new d2dFramework::AsyncInputManager)
		, mPlayerFSM(new PlayerFSM())
		, mMonsterFSM(new MonsterFSM)
		, mBealzebubFSM(new BealzebubFSM)
		, mYsBossFSM(new YsBossFSM)
		, mEveBossFSM(new EveBossFSM)
		, mLilithFSM(new LilithBossFSM)
		, mUIManager(new UIManager)
	{
	}

	SoulBeaterProcessor::~SoulBeaterProcessor()
	{
		delete mBeatManager; mBeatManager = nullptr;
		delete mEffectManager; mEffectManager = nullptr;
		delete mParticleManager; mParticleManager = nullptr;
		delete mAsyncInputManager; mAsyncInputManager = nullptr;
		delete mUIManager; mUIManager = nullptr;

		delete mPlayerFSM; mPlayerFSM = nullptr;
		delete mMonsterFSM; mMonsterFSM = nullptr;
		delete mBealzebubFSM; mBealzebubFSM = nullptr;
		delete mYsBossFSM; mYsBossFSM = nullptr;
		delete mEveBossFSM; mEveBossFSM = nullptr;
		delete mLilithFSM; mLilithFSM = nullptr;
	}

	void SoulBeaterProcessor::Init(HWND hwnd)
	{
		using namespace d2dFramework;

		static unsigned int tempId = 20000;

		GameProcessor::Init(hwnd);
		//RenderManager::GetInstance()->SetGridObjectTransform(D2D1::Matrix3x2F::Identity());

		Loader::LoadD2DBitmap();
		Loader::LoadSoundAsset();
		SceneLoader::LoadAllBitmaps();

		initScene();

		// manager Init
		{
			mBeatManager->Init(DEFAULT_BPM, getTimeManager()->GetFrequency());
			mEffectManager->Init(RenderManager::GetInstance()->GetD2DDeviceContext());
			mParticleManager;
			mUIManager->Init(getSceneManager());

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

		// Load Asset
		{
			Loader::LoadAnimationAsset(mBeatManager);
		}
		// Setter
		{
			ObjectHelper::SetBeatManager(mBeatManager);
		}

		// FSM Init
		{
			mMonsterFSM->Init(mBeatManager);
			mPlayerFSM->Init(mBeatManager, getTimeManager(), static_cast<unsigned int>(eSoulBeaterID::PlayerObject), static_cast<unsigned int>(eSoulBeaterID::PlayerNodeObject));
			mBealzebubFSM->Init(mBeatManager, mMonsterFSM, static_cast<unsigned int>(eSoulBeaterID::BelzebubObject));
			mYsBossFSM->Init(mBeatManager, mMonsterFSM, static_cast<unsigned int>(eSoulBeaterID::YsObject), static_cast<unsigned int>(eSoulBeaterID::EveObject), static_cast<unsigned int>(eSoulBeaterID::PlayerObject));
			mEveBossFSM->Init(mBeatManager, mMonsterFSM, static_cast<unsigned int>(eSoulBeaterID::YsObject), static_cast<unsigned int>(eSoulBeaterID::EveObject), static_cast<unsigned int>(eSoulBeaterID::PlayerObject));
			mLilithFSM->Init(mBeatManager, mMonsterFSM, static_cast<unsigned int>(eSoulBeaterID::LilithObject));
		}

		// indepentent Object
		{
			ObjectHelper::CreateStageNameObject();
		}

		// particle Init
		initParticle();

		// collision check
		{
			getCollisionManager()->SetIsCollision(eObjectType::Player, eObjectType::Wall);
			getCollisionManager()->SetIsCollision(eObjectType::Player, eObjectType::Enemy);
			getCollisionManager()->SetIsCollision(eObjectType::Player, eObjectType::Projectile);
		}

		getSceneManager()->GetCurrentScene()->Enter();
	}

	void SoulBeaterProcessor::Update()
	{
		using namespace d2dFramework;

		RenderManager::GetInstance()->BitmapBeginDraw();
		RenderManager::GetInstance()->Clear(D2D1::Matrix3x2F::Identity(), { 0,0,0,1 });
		{
			const float DELTA_TIME = getTimeManager()->GetDeltaTime();

			ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(static_cast<unsigned int>(eSoulBeaterID::TitleParticle));
			PE->Render(RenderManager::GetInstance());

			std::queue<KeyInformation> input = mAsyncInputManager->Flush();

			if (!mUIManager->GetIsActive())
			{
				updateFSM(DELTA_TIME, input);
				PE->SetIsEmit(true);
				PE->Update(DELTA_TIME);
			}

			SetIsUpdateable(!mUIManager->GetIsActive());
			mUIManager->Update(DELTA_TIME);
			handleSceneChange();

			GameProcessor::Update();

			updateEffect();
		}
		RenderManager::GetInstance()->BitmapEndDraw();

		RenderManager::GetInstance()->BeginDraw();
		RenderManager::GetInstance()->CopyBitmapRenderToHwndRender();
		RenderManager::GetInstance()->EndDraw();
	}

	void SoulBeaterProcessor::handleSceneChange()
	{
		using namespace d2dFramework;

		SoulBeaterScene* scene = static_cast<SoulBeaterScene*>(getSceneManager()->GetCurrentScene());
		unsigned int sceneId = scene->GetId();

		bool bIsSceneChange = false;
		bool bIsPlayerDie = false;

		const bool CREATE_QUEUE_EMPTY = ObjectManager::GetInstance()->GetCreateObjectQueue().empty();

		std::vector<eSoulBeaterID> particleIDs;
		particleIDs.push_back(eSoulBeaterID::TitleParticle);
		particleIDs.push_back(eSoulBeaterID::BeatParticle1);
		particleIDs.push_back(eSoulBeaterID::BeatParticle2);
		particleIDs.push_back(eSoulBeaterID::BeatParticle3);
		particleIDs.push_back(eSoulBeaterID::BeatParticle4);

		auto setParticleColor = [&, this](const D2D1_COLOR_F& color) -> void
		{
			for (eSoulBeaterID particleID : particleIDs)
			{
				unsigned int id = static_cast<unsigned int>(particleID);
				ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(id);

				if (PE != nullptr)
				{
					ParticleEmitterInformation PEI = PE->GetParticleSysteInformation();
					PEI.StartColor = color;
					PE->SetParticleSystemInformation(PEI);
				}
			}
		};
		auto setParticleIsEmiiter = [&, this](bool bIsEmitter) -> void
		{
			for (eSoulBeaterID particleID : particleIDs)
			{
				unsigned int id = static_cast<unsigned int>(particleID);
				ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(id);

				if (PE != nullptr)
				{
					PE->SetIsEmit(bIsEmitter);
				}
			}
		};

		setParticleColor({ 0,0,1,1 });

		switch (static_cast<eSoulBeaterID>(sceneId))
		{
		case soulBeater::eSoulBeaterID::StartScene:
			setParticleIsEmiiter(false);
			break;
		case soulBeater::eSoulBeaterID::GameOverScene:
			setParticleIsEmiiter(false);
			break;
		case soulBeater::eSoulBeaterID::CutScene1:
			bIsSceneChange = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::CutScene1Object)) == nullptr;
			setParticleColor({ 1, 1, 0, 1 });
			setParticleIsEmiiter(false);
			break;
		case soulBeater::eSoulBeaterID::CutScene2:
			bIsSceneChange = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::CutScene2Object)) == nullptr;
			setParticleColor({ 1, 1, 0, 1 });
			setParticleIsEmiiter(false);
			break;
		case soulBeater::eSoulBeaterID::CutScene3:
			bIsSceneChange = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::CutScene3Object)) == nullptr;
			setParticleColor({ 1, 1, 0, 1 });
			setParticleIsEmiiter(false);
			break;
		case soulBeater::eSoulBeaterID::EndingScene:
			bIsSceneChange = ObjectManager::GetInstance()->GetCreateObjectQueue().empty() && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::CutSceneEndingObject)) == nullptr;
			setParticleColor({ 1, 1, 1, 1 });
			break;
		case soulBeater::eSoulBeaterID::Stage1_1:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage1_2:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage1_3:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage2_1:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage2_2:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage2_3:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage3_1:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage3_2:
			// intentional fall through
		case soulBeater::eSoulBeaterID::Stage3_3:
		{
			GameObject* timerObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::TimerObject), true);
			assert(timerObject != nullptr);
			TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();

			bIsSceneChange = ObjectManager::GetInstance()->GetCreateObjectQueue().empty() && mMonsterFSM->GetMonsterMaps().size() == 0;
			bIsSceneChange |= timerComponent != nullptr && timerComponent->GetTickCount() <= 0.f;

			bIsPlayerDie = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject)) == nullptr;

			break;
		}
		case soulBeater::eSoulBeaterID::BossScene1:
		{
			GameObject* timerObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::TimerObject), true);
			assert(timerObject != nullptr);
			TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();

			bIsSceneChange = ObjectManager::GetInstance()->GetCreateObjectQueue().empty() && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::BelzebubObject)) == nullptr;

			bIsPlayerDie = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject)) == nullptr;
			bIsPlayerDie |= timerComponent != nullptr && timerComponent->GetTickCount() <= 0.f;

			setParticleColor({ 1, 0, 0, 1 });

			break;
		}
		case soulBeater::eSoulBeaterID::BossScene2:
		{
			GameObject* timerObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::TimerObject), true);
			assert(timerObject != nullptr);
			TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();

			bIsSceneChange = ObjectManager::GetInstance()->GetCreateObjectQueue().empty() && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::EveObject)) == nullptr;

			bIsPlayerDie = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject)) == nullptr;
			bIsPlayerDie |= timerComponent != nullptr && timerComponent->GetTickCount() <= 0.f;

			setParticleColor({ 1, 0, 0, 1 });

			break;
		}
		case soulBeater::eSoulBeaterID::BossScene3:
		{
			GameObject* timerObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::TimerObject), true);
			assert(timerObject != nullptr);
			TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();

			bIsSceneChange = ObjectManager::GetInstance()->GetCreateObjectQueue().empty() && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::LilithObject)) == nullptr;

			bIsPlayerDie = CREATE_QUEUE_EMPTY && ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject)) == nullptr;
				bIsPlayerDie |= timerComponent != nullptr && timerComponent->GetTickCount() <= 0.f;

			setParticleColor({ 1, 0, 0, 1 });

			break;
		}
		default:
			assert(false);
			break;
		}

		if (bIsSceneChange || InputManager::GetInstance()->GetKeyState('N') == eKeyState::Push)
		{
			unsigned int nextSceneId = scene->GetNextSceneID();
			mBeatManager->SetBPM(DEFAULT_BPM);
			EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", std::to_string(nextSceneId));
		}
		if (bIsPlayerDie)
		{
			EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", std::to_string(static_cast<unsigned int>(eSoulBeaterID::GameOverScene)));
			// 게임 오버씬으로 전환
		} 
	}

	void SoulBeaterProcessor::Release()
	{
		using namespace d2dFramework;

		// event release
		EventManager::GetInstance()->UnregisterEventHandler(KNIGHT_EFFECT_EVENT, static_cast<unsigned int>(eSoulBeaterID::Processor));

		// manager release
		mBeatManager;
		mEffectManager->Release();
		mParticleManager->Release();
		mUIManager->Release();
		mAsyncInputManager->SetIsEnd(true);
		mInputFuture.get();

		// fsm release
		mPlayerFSM->Release();
		mMonsterFSM->Release();
		mBealzebubFSM->Release();
		mEveBossFSM->Release();
		mYsBossFSM->Release();
		mLilithFSM->Release();

		GameProcessor::Release();
	}

	void SoulBeaterProcessor::updateFSM(float deltaTime, std::queue<d2dFramework::KeyInformation>& input)
	{
		using namespace d2dFramework;

		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject));
		if (player != nullptr)
		{
			Transform* playerTransform = player->GetComponent<Transform>();
			RenderManager::GetInstance()->SetGridObjectTransform(playerTransform->GetInverseTransform() * getCameraManager()->GetCombineMatrix());
		}

		mPlayerFSM->Update(deltaTime, input);
		mMonsterFSM->Update(deltaTime);
		mBealzebubFSM->Update(deltaTime);
		mEveBossFSM->Update(deltaTime);
		mYsBossFSM->Update(deltaTime);
		mLilithFSM->Update(deltaTime);
	}

	void SoulBeaterProcessor::updateEffect()
	{
		using namespace d2dFramework;

		const float DELTA_TIME = getTimeManager()->GetDeltaTime();

		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject));

		if (player != nullptr)
		{
			PlayerComponent* playerComponent = player->GetComponent<PlayerComponent>();
			assert(playerComponent != nullptr);

			playerComponent->AddHitEffectElapsed(DELTA_TIME);
			if (playerComponent->GetHitEffectElased() > 0.f)
			{
				ID2D1Bitmap* bitmap = RenderManager::GetInstance()->GetBitmapOrNull(HIT_EFFECT_SPRITE_KEY);
				assert(bitmap != nullptr);
				RenderManager::GetInstance()->DrawBitMap({ 0,0,1920, 1080 }, { 0,0,1920,1080 }, bitmap);
			}
		}

		double accumulateTime = static_cast<double>(getTimeManager()->GetCurTime().QuadPart - mBeatManager->GetLastBPMChaneTime().QuadPart) / getTimeManager()->GetFrequency().QuadPart;
		const float interval = static_cast<float>(mBeatManager->GetIntervalTime()) * 0.25f;

		unsigned int beforeEffect = (static_cast<unsigned int>(accumulateTime / interval) - 1) % 4;
		unsigned int currentEffect = static_cast<unsigned int>(accumulateTime / interval) % 4;

		ParticleEmitter* beforePE = mParticleManager->FindParticleEmitterOrNull(static_cast<unsigned int>(eSoulBeaterID::BeatParticle1) + beforeEffect);
		ParticleEmitter* currentPE = mParticleManager->FindParticleEmitterOrNull(static_cast<unsigned int>(eSoulBeaterID::BeatParticle1) + currentEffect);
		beforePE->SetIsEmit(false);
		currentPE->SetIsEmit(true);

		for (int i = 0; i < 4; ++i)
		{
			ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(static_cast<unsigned int>(eSoulBeaterID::BeatParticle1) + i);

			if (!mUIManager->GetIsActive())
			{
				PE->Update(DELTA_TIME);
			}

			if (ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject)) != nullptr)
			{
				PE->Render(RenderManager::GetInstance());
			}
		}

		ParticleEmitter* PE = mParticleManager->FindParticleEmitterOrNull(static_cast<unsigned int>(eSoulBeaterID::KnightDeathParticle));
		assert(PE != nullptr);
		if (mKnightDeathElapsed >= 0.f)
		{
			PE->SetIsEmit(true);
			mKnightDeathElapsed -= DELTA_TIME;
		}
		else
		{
			PE->SetIsEmit(false);
		}

		PE->Update(DELTA_TIME);
		PE->Render(RenderManager::GetInstance());
	}

	void SoulBeaterProcessor::initScene()
	{
		SoulBeaterScene* start = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::StartScene));
		start->SetSceneType(eSceneType::Start);
		start->SetUIType(eUIManagerType::StartScene);
		start->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage1_1));
		getSceneManager()->SetCurrentScene(start->GetId());
		SoulBeaterScene* ending = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::EndingScene));
		ending->SetSceneType(eSceneType::Ending);
		ending->SetUIType(eUIManagerType::None);
		ending->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::StartScene));

		SoulBeaterScene* GameOver = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::GameOverScene));
		GameOver->SetSceneType(eSceneType::GameOver);
		GameOver->SetUIType(eUIManagerType::GameOver);
		GameOver->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::StartScene));

		SoulBeaterScene* stage1_1 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage1_1));
		stage1_1->SetSceneType(eSceneType::Stage1_1);
		stage1_1->SetUIType(eUIManagerType::InGame);
		stage1_1->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage1_2));

		SoulBeaterScene* stage1_2 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage1_2));
		stage1_2->SetSceneType(eSceneType::Stage1_2);
		stage1_2->SetUIType(eUIManagerType::InGame);
		stage1_2->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage1_3));

		SoulBeaterScene* stage1_3 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage1_3));
		stage1_3->SetSceneType(eSceneType::Stage1_3);
		stage1_3->SetUIType(eUIManagerType::InGame);
		stage1_3->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::CutScene1));

		SoulBeaterScene* stage2_1 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage2_1));
		stage2_1->SetSceneType(eSceneType::Stage2_1);
		stage2_1->SetUIType(eUIManagerType::InGame);
		stage2_1->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage2_2));

		SoulBeaterScene* stage2_2 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage2_2));
		stage2_2->SetSceneType(eSceneType::Stage2_2);
		stage2_2->SetUIType(eUIManagerType::InGame);
		stage2_2->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage2_3));

		SoulBeaterScene* stage2_3 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage2_3));
		stage2_3->SetSceneType(eSceneType::Stage2_3);
		stage2_3->SetUIType(eUIManagerType::InGame);
		stage2_3->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::CutScene2));

		SoulBeaterScene* stage3_1 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage3_1));
		stage3_1->SetSceneType(eSceneType::Stage3_1);
		stage3_1->SetUIType(eUIManagerType::InGame);
		stage3_1->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage3_2));

		SoulBeaterScene* stage3_2 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage3_2));
		stage3_2->SetSceneType(eSceneType::Stage3_2);
		stage3_2->SetUIType(eUIManagerType::InGame);
		stage3_2->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage3_3));

		SoulBeaterScene* stage3_3 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::Stage3_3));
		stage3_3->SetSceneType(eSceneType::Stage3_3);
		stage3_3->SetUIType(eUIManagerType::InGame);
		stage3_3->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::CutScene3));

		SoulBeaterScene* cutScene1 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::CutScene1));
		cutScene1->SetSceneType(eSceneType::CutScene1);
		cutScene1->SetUIType(eUIManagerType::None);
		cutScene1->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::BossScene1));

		SoulBeaterScene* cutScene2 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::CutScene2));
		cutScene2->SetSceneType(eSceneType::CutScene2);
		cutScene2->SetUIType(eUIManagerType::None);
		cutScene2->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::BossScene2));

		SoulBeaterScene* cutScene3 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::CutScene3));
		cutScene3->SetSceneType(eSceneType::CutScene3);
		cutScene3->SetUIType(eUIManagerType::None);
		cutScene3->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::BossScene3));

		SoulBeaterScene* boss1 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::BossScene1));
		boss1->SetSceneType(eSceneType::Boss1);
		boss1->SetUIType(eUIManagerType::InGame);
		boss1->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage2_1));

		SoulBeaterScene* boss2 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::BossScene2));
		boss2->SetSceneType(eSceneType::Boss2);
		boss2->SetUIType(eUIManagerType::InGame);
		boss2->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::Stage3_1));

		SoulBeaterScene* boss3 = getSceneManager()->CreateScene<SoulBeaterScene>(static_cast<unsigned int>(eSoulBeaterID::BossScene3));
		boss3->SetSceneType(eSceneType::Boss3);
		boss3->SetUIType(eUIManagerType::InGame);
		boss3->SetNextSceneID(static_cast<unsigned int>(eSoulBeaterID::EndingScene));
	}

	void SoulBeaterProcessor::initParticle()
	{
		mKnightDeathElapsed = 0.f;

		auto knightDeath = [this](const std::string& data) ->void
		{
			mKnightDeathElapsed = 0.2f;
		};
		EventManager::GetInstance()->RegisterEventHandler(KNIGHT_EFFECT_EVENT, static_cast<unsigned int>(eSoulBeaterID::Processor), knightDeath);

		ParticleEmitter* particleEmitter;
		ParticleEmitterInformation info;

		{
			ZeroMemory(&info, sizeof(info));
			info.MaxParticleCount = 1000;
			info.OffsetHalfRange = { 1000, 100 };
			info.StartColor = { 0, 0, 1, 0.5 };
			info.StartColorHalfRange = { 0.5f, 0.5f, 0.5f, 0.f };
			info.EndColor = { 0, 0, 0, 0.1f };
			info.EndColorHalfRange = { 0.2f, 0.2f, 0.2f, 0.f };
			info.AngularVelocity;
			info.RotationInRadianHalfRange = 180;
			info.AngularVelocityHalfRange = 180;
			info.Position = { 900, 0 };
			info.Velocity = { 0, 200 };
			info.VelocityHalfRange = { 300, 500 };
			info.Acceleration = { 0, 500 };
			info.AccelerationHalfRange = { 100, 300 };
			info.TimeToLive = 3.f;
			info.TimeToLiveHalfRange = 0.9f;
			info.StartScale = { 3, 3 };
			info.StartScaleHalfRange = { 2, 2 };
			info.EndScale = { 5, 5 };
			info.EndScaleHalfRange = { 0.5f, 0.5f };
			info.Size = { 1.f, 1.f };
			info.ParticleType = eParticleType::Circle;
			info.Bitmap = RenderManager::GetInstance()->GetBitmapOrNull(PLAYER_NODE_SPRITE_KEY);
			info.UVRect = STAGE_NAME[0];
			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::TitleParticle));
			particleEmitter->Init(info);
		}

		// beatParticle
		{
			ZeroMemory(&info, sizeof(info));
			info.MaxParticleCount = 50;
			info.OffsetHalfRange = { 0, 0 };
			info.StartColor = { 0, 1, 1, 1.f };
			info.StartColorHalfRange = { 0.5f, 0.5f, 0.5f, 0.2f };
			info.EndColor = { 0, 0, 0, 0.5f };
			info.EndColorHalfRange = { 0.2f, 0.2f, 0.2f, 0.2f };
			info.AngularVelocity;
			info.RotationInRadianHalfRange = 180;
			info.AngularVelocityHalfRange = 45;
			info.Velocity = { 0, -200 };
			info.VelocityHalfRange = { 100, 50 };
			info.Acceleration = { 0, -500 };
			info.AccelerationHalfRange = { 50, 50 };
			info.TimeToLive = 0.125f;
			info.TimeToLiveHalfRange = 0.0f;
			info.StartScale = { 5, 5 };
			info.StartScaleHalfRange = { 0.1f ,0.1f };
			info.EndScale = { 1, 1 };
			info.EndScaleHalfRange = { 0.1f,0.1f };
			info.Size = { 10.f, 10.f };
			info.ParticleType = eParticleType::Circle;
			info.Bitmap = RenderManager::GetInstance()->GetBitmapOrNull(L"InGameUI");
			info.UVRect = { 42, 156, 184, 311 };// STAGE_NAME[0];

			info.Position = { static_cast<float>(GetWidth()) * 0.44f, static_cast<float>(GetHeight()) * 0.92f };
			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::BeatParticle1));
			particleEmitter->Init(info);

			info.Position = { static_cast<float>(GetWidth()) * 0.48f, static_cast<float>(GetHeight()) * 0.92f };
			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::BeatParticle2));
			particleEmitter->Init(info);

			info.Position = { static_cast<float>(GetWidth()) * 0.52f, static_cast<float>(GetHeight()) * 0.92f };
			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::BeatParticle3));
			particleEmitter->Init(info);

			info.Position = { static_cast<float>(GetWidth()) * 0.56f, static_cast<float>(GetHeight()) * 0.92f };
			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::BeatParticle4));
			particleEmitter->Init(info);
		}

		{
			ZeroMemory(&info, sizeof(info));
			info.MaxParticleCount = 50;
			info.OffsetHalfRange = { 0, 0 };
			info.StartColor = { 1, 1, 1, 0.2f };
			info.StartColorHalfRange = { 0.5f, 0.5f, 0.5f, 0.2f };
			info.EndColor = { 0, 0, 0, 0.7f };
			info.EndColorHalfRange = { 0.2f, 0.2f, 0.2f, 0.2f };
			info.AngularVelocity;
			info.RotationInRadianHalfRange = 180;
			info.AngularVelocityHalfRange = 45;
			info.Velocity = { 0, -200 };
			info.VelocityHalfRange = { 200, 50 };
			info.Acceleration = { 0, 100 };
			info.AccelerationHalfRange = { 200, 50 };
			info.TimeToLive = 0.5f;
			info.TimeToLiveHalfRange = 0.1f;
			info.StartScale = { 4, 4 };
			info.StartScaleHalfRange = { 0.1f ,0.1f };
			info.EndScale = { 10, 10 };
			info.EndScaleHalfRange = { 0.1f,0.1f };
			info.Size = { 10.f, 10.f };
			info.ParticleType = eParticleType::Bitmap;
			info.Bitmap = RenderManager::GetInstance()->GetBitmapOrNull(L"Particle4");
			info.UVRect = { 0, 0, 120, 120 };// STAGE_NAME[0];
			info.Position = { GetWidth() * 0.5f, GetHeight() * 0.5f };

			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::KnightDeathParticle));
			particleEmitter->Init(info);

			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::GhostDeathParticle));
			particleEmitter->Init(info);

			particleEmitter = mParticleManager->CreateParticleEmitter(static_cast<unsigned int>(eSoulBeaterID::SuccubusDeathParticle));
			particleEmitter->Init(info);

		}
	}
}