#include "SoulBeaterScene.h"

#include "SceneLoader.h"
#include "ObjectManager.h"
#include "eSoulBeaterID.h"
#include "IncludeComponent.h"
#include "TimerComponent.h"
#include "HPComponent.h"
#include "ConstantTable.h"
#include "ObjectHelper.h"
#include "BossState.h"
#include "GridPosition.h"

namespace soulBeater
{
	SoulBeaterScene::SoulBeaterScene(unsigned int id)
		: Scene(id)
		, mUIType(eUIManagerType::None)
		, mSceceType(eSceneType::Size)
	{
	}

	void SoulBeaterScene::Enter()
	{
		using namespace d2dFramework;

		//소리 재생.
		enterSoundPlay();

		Scene::Enter();
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
				grid->SetCellDistance(GRID_DISTANCE);
				grid->SetMaxX(MAX_X);
				grid->SetMaxY(MAX_Y);
				grid->SetPosition(grid->GetPosition());

				SpriteRenderer* renderer = tile->GetComponent<SpriteRenderer>();
				renderer->SetTransformLayer(eTransformLayer::Grid);
				renderer->SetSize({ static_cast<float>(GRID_DISTANCE), static_cast<float>(GRID_DISTANCE) });
			}
		}

		// 씬 종속적이지 않은 오브젝트들 설정
		enterIndependentObject();

		// 씬 진입 시 생성되어야 할 오브젝트 정의
		enterCreateObject();

		// 해당 함수는 타일맵만을 로딩함
	}

	void SoulBeaterScene::Exit()
	{
		Scene::Exit();
	}

	void SoulBeaterScene::SerializeIn(nlohmann::ordered_json& object)
	{
		//mSceneType = object["InitButtonManagerType"];
		Scene::SerializeIn(object);
	}

	void SoulBeaterScene::SerializeOut(nlohmann::ordered_json& object)
	{
		//object["InitButtonManagerType"] = mSceneType;
		Scene::SerializeOut(object);
	}

	void SoulBeaterScene::enterIndependentObject()
	{
		using namespace d2dFramework;

		GameObject* stageNameObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::StageNameObject), true);
		GameObject* timerObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::TimerObject), true);
		GameObject* hpObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::HPObject), true);

		switch (mSceceType)
		{
		case eSceneType::Start:
			// intential fall through
		case eSceneType::Ending:
			// intential fall through
		case eSceneType::CutScene1:
			// intential fall through
		case eSceneType::CutScene2:
			// intential fall through
		case eSceneType::CutScene3:
			if (stageNameObject != nullptr)
			{
				SpriteRenderer* renderer = stageNameObject->GetComponent<SpriteRenderer>();
				assert(renderer != nullptr);

				renderer->SetIsActive(false);
			}
			if (timerObject != nullptr)
			{
				TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();
				assert(timerComponent);

				timerComponent->SetIsActive(false);
			}
			if (hpObject != nullptr)
			{
				HPComponent* hpComponent = hpObject->GetComponent<HPComponent>();
				assert(hpComponent);

				hpComponent->SetIsActive(false);
			}
			break;
		case eSceneType::Stage1_1:
			if (hpObject != nullptr)
			{
				HPComponent* hpComponent = hpObject->GetComponent<HPComponent>();
				assert(hpComponent);
				hpComponent->SetHP(3);
			}
			// intential fall through
		case eSceneType::Stage1_2:
			// intential fall through
		case eSceneType::Stage1_3:
			// intential fall through
		case eSceneType::Stage2_1:
			// intential fall through
		case eSceneType::Stage2_2:
			// intential fall through
		case eSceneType::Stage2_3:
			// intential fall through
		case eSceneType::Stage3_1:
			// intential fall through
		case eSceneType::Stage3_2:
			// intential fall through
		case eSceneType::Stage3_3:
			if (stageNameObject != nullptr)
			{
				SpriteRenderer* renderer = stageNameObject->GetComponent<SpriteRenderer>();
				assert(renderer != nullptr);

				renderer->SetIsActive(true);
				renderer->SetUVRectangle(STAGE_NAME[static_cast<int>(mSceceType) - static_cast<int>(eSceneType::Stage1_1)]);
			}
			if (timerObject != nullptr)
			{
				TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();
				assert(timerComponent);

				timerComponent->SetIsActive(true);
				timerComponent->SetCurrentTime(static_cast<float>(STAGE_TIME));
			}
			if (hpObject != nullptr)
			{
				HPComponent* hpComponent = hpObject->GetComponent<HPComponent>();
				assert(hpComponent);

				hpComponent->SetIsActive(true);
			}
			break;
		case eSceneType::Boss1:
			// intential fall through
		case eSceneType::Boss2:
			// intential fall through
		case eSceneType::Boss3:
			if (stageNameObject != nullptr)
			{
				SpriteRenderer* renderer = stageNameObject->GetComponent<SpriteRenderer>();
				assert(renderer != nullptr);

				renderer->SetIsActive(true);
				renderer->SetUVRectangle(STAGE_NAME[static_cast<int>(mSceceType) - static_cast<int>(eSceneType::Stage1_1)]);
			}
			if (timerObject != nullptr)
			{
				TimerComponent* timerComponent = timerObject->GetComponent<TimerComponent>();
				assert(timerComponent);

				timerComponent->SetIsActive(true);
				timerComponent->SetCurrentTime(static_cast<float>(BOSS_STAGE_TIME));
			}
			if (hpObject != nullptr)
			{
				HPComponent* hpComponent = hpObject->GetComponent<HPComponent>();
				assert(hpComponent);

				hpComponent->SetIsActive(true);
			}
			break;
		case eSceneType::Size:
			assert(false);
			break;
		default:
			break;
		}
	}

	void SoulBeaterScene::enterCreateObject()
	{
		switch (mSceceType)
		{
		case eSceneType::Start:
			break;
		case eSceneType::Ending:
			ObjectHelper::CreateCutScene(eCutSceneObjectType::EndingCreding);
			break;
		case eSceneType::CutScene1:
			ObjectHelper::CreateCutScene(eCutSceneObjectType::CutScene1);
			break;
		case eSceneType::CutScene2:
			ObjectHelper::CreateCutScene(eCutSceneObjectType::CutScene2);
			break;
		case eSceneType::CutScene3:
			ObjectHelper::CreateCutScene(eCutSceneObjectType::CutScene3);
			break;
		case eSceneType::Stage1_1:
			ObjectHelper::CreatePlayer({ 3, 3 });
			ObjectHelper::CreateMonster({ 10, 8 }, eMonsterType::Escape, 3, 1);
			ObjectHelper::CreateMonster({ 4, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 3 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 8 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 8, 7 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 5 }, eMonsterType::Attack, 3, 2);
			ObjectHelper::CreateMonster({ 12, 10 }, eMonsterType::Attack, 2, 1);
			break;
		case eSceneType::Stage1_2:
			ObjectHelper::CreatePlayer({ 5, 6 });
			ObjectHelper::CreateMonster({ 6, 4 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 10, 8 }, eMonsterType::Escape, 2, 2);
			ObjectHelper::CreateMonster({ 10, 4 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 7, 10 }, eMonsterType::Escape, 1, 2);
			ObjectHelper::CreateMonster({ 2, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 2, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 6, 8 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 10, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 14, 6 }, eMonsterType::Attack, 3, 2);
			break;
		case eSceneType::Stage1_3:
			ObjectHelper::CreatePlayer({ 7, 5 });
			ObjectHelper::CreateMonster({ 3, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 3 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 6, 7 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 12, 7 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 10 }, eMonsterType::Attack, 4, 2);
			ObjectHelper::CreateMonster({ 5, 4 }, eMonsterType::Attack, 4, 2);
			ObjectHelper::CreateMonster({ 8, 9 }, eMonsterType::Attack, 4, 2);
			ObjectHelper::CreateMonster({ 11, 4 }, eMonsterType::Attack, 5, 1);
			break;
		case eSceneType::Stage2_1:
			ObjectHelper::CreatePlayer({ 7, 6 });
			ObjectHelper::CreateMonster({ 7, 2 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 10, 5 }, eMonsterType::Escape, 3, 3);
			ObjectHelper::CreateMonster({ 3, 7 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 9 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 11, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 12, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 3, 4 }, eMonsterType::Attack, 3, 3);
			ObjectHelper::CreateMonster({ 13, 7 }, eMonsterType::Attack, 2, 2);
			break;
		case eSceneType::Stage2_2:
			ObjectHelper::CreatePlayer({ 12, 3 });
			ObjectHelper::CreateMonster({ 3, 7 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 4, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 6, 6 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 4, 9 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 8, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 12, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 12, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 8, 2 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 12, 9 }, eMonsterType::Attack, 3, 2);
			break;
		case eSceneType::Stage2_3:
			ObjectHelper::CreatePlayer();
			ObjectHelper::CreateMonster({ 4, 6 }, eMonsterType::Escape, 2, 2);
			ObjectHelper::CreateMonster({ 6, 4 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 9, 2 }, eMonsterType::Escape, 2, 2);
			ObjectHelper::CreateMonster({ 3, 8 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 7 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 11, 9 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 11, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 6, 6 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 12, 7 }, eMonsterType::Attack);
			break;
		case eSceneType::Stage3_1:
			ObjectHelper::CreatePlayer({ 13 , 2 });
			ObjectHelper::CreateMonster({ 10, 6 }, eMonsterType::Escape, 2, 2);
			ObjectHelper::CreateMonster({ 4, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 10, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 8 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 4, 10 }, eMonsterType::Attack, 3, 3);
			ObjectHelper::CreateMonster({ 7, 10 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 12, 8 }, eMonsterType::Attack, 4, 2);
			break;
		case eSceneType::Stage3_2:
			ObjectHelper::CreatePlayer({ 2, 11 });
			ObjectHelper::CreateMonster({ 2, 8 }, eMonsterType::Escape, 1, 1);
			ObjectHelper::CreateMonster({ 6, 9 }, eMonsterType::Escape, 3, 3);
			ObjectHelper::CreateMonster({ 9, 11 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 10, 6 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 2, 9 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 2, 9 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 6, 11 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 2 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 9, 8 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 2, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 3, 2 }, eMonsterType::Attack, 2, 2);
			ObjectHelper::CreateMonster({ 10, 2 }, eMonsterType::Attack, 3, 1);
			ObjectHelper::CreateMonster({ 13, 5 }, eMonsterType::Attack, 4, 3);
			break;
		case eSceneType::Stage3_3:
			ObjectHelper::CreatePlayer({ 7 , 4 });
			ObjectHelper::CreateMonster({ 6, 7 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 9, 7 }, eMonsterType::Escape, 2, 3);
			ObjectHelper::CreateMonster({ 5, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 8, 8 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 10, 10 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 10, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 3, 10 }, eMonsterType::Attack, 2, 2);
			ObjectHelper::CreateMonster({ 3, 6 }, eMonsterType::Attack, 4, 3);
			ObjectHelper::CreateMonster({ 7, 10 }, eMonsterType::Attack, 3, 3);
			ObjectHelper::CreateMonster({ 12, 10 }, eMonsterType::Attack, 2, 2);
			ObjectHelper::CreateMonster({ 12, 6 }, eMonsterType::Attack, 4, 3);
			break;
		case eSceneType::Boss1:
			ObjectHelper::CreatePlayer({ 3, 4 });
			ObjectHelper::CreateBealzebub({12, 5});
			break;
		case eSceneType::Boss2:
			ObjectHelper::CreatePlayer();
			ObjectHelper::CreateEve({  12, 3 });
			ObjectHelper::CreateYs({11, 5 });
			break;
		case eSceneType::Boss3:
			ObjectHelper::CreatePlayer();
			ObjectHelper::CreateLilith({ 12, 5 });
			break;
		case eSceneType::Size:
			assert(false);
			break;
		default:
			break;
		}
	}

	void SoulBeaterScene::enterSoundPlay()
	{
		using namespace d2dFramework;

		switch (mSceceType)
		{
		case eSceneType::Start:
			SoundManager::GetInstance()->Play(BGM_TITLE_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage1_1:
			SoundManager::GetInstance()->Play(BGM_STAGE1_1_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage1_2:
			SoundManager::GetInstance()->Play(BGM_STAGE1_2_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage1_3:
			SoundManager::GetInstance()->Play(BGM_STAGE1_3_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage2_1:
			SoundManager::GetInstance()->Play(BGM_STAGE2_1_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage2_2:
			SoundManager::GetInstance()->Play(BGM_STAGE2_2_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage2_3:
			SoundManager::GetInstance()->Play(BGM_STAGE2_3_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage3_1:
			SoundManager::GetInstance()->Play(BGM_STAGE3_1_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage3_2:
			SoundManager::GetInstance()->Play(BGM_STAGE3_2_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Stage3_3:
			SoundManager::GetInstance()->Play(BGM_STAGE3_3_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Ending:
			break;
		case eSceneType::Size:
			assert(false);
			break;
		case eSceneType::CutScene1: break;
		case eSceneType::CutScene2: break;
		case eSceneType::CutScene3: break;
		case eSceneType::Boss1: 
			SoundManager::GetInstance()->Play(BGM_STAGE1_BOSS_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Boss2: 
			SoundManager::GetInstance()->Play(BGM_STAGE2_BOSS_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::Boss3: 
			SoundManager::GetInstance()->Play(BGM_STAGE3_BOSS_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		case eSceneType::GameOver: 
			SoundManager::GetInstance()->Play(BGM_GAMEOVER_KEY, eSoundType::BGM, ePlayType::Overwrite, 0);
			break;
		default:
			break;
		}
	}
}
