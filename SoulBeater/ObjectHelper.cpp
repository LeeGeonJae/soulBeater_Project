#include "ObjectHelper.h"

#include "PlayerComponent.h"
#include "NodeComponent.h"
#include "BaseMonsterComponent.h"
#include "LilithBossComponent.h"
#include "YsBossComponent.h"
#include "EveBossComponent.h"
#include "BealzebubComponent.h"
#include "SoulBeaterProcessor.h"
#include "eSoulBeaterID.h"
#include "ConstantTable.h"
#include "CutSceneComponent.h"
#include "HPComponent.h"

#include "BeatManager.h"
#include "GameObject.h"
#include "IncludeComponent.h"
#include "IncludeManager.h"
#include "GridPosition.h"

#include <cassert>

namespace soulBeater
{
	d2dFramework::BeatManager* ObjectHelper::mBeatManager = nullptr;

	void ObjectHelper::SetBeatManager(d2dFramework::BeatManager* beatManager)
	{
		mBeatManager = beatManager;
	}

	d2dFramework::GameObject* ObjectHelper::CreatePlayer(const d2dFramework::GridPosition& position)
	{
		using namespace d2dFramework;

		assert(mBeatManager != nullptr);

		// 플레이어
		GameObject* player = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::PlayerObject));
		CircleCollider* colider = player->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());
		Transform* playerTransform = player->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		AnimationRenderer* playerAnimation = player->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
		GridComponent* playerGrid = player->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		GridMoveComponent* playerGridmove = player->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::GetTempId());
		PlayerComponent* playercomponent = player->CreateComponent<soulBeater::PlayerComponent>(SoulBeaterProcessor::GetTempId());

		GameObject* hpObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::HPObject), true);
		if (hpObject != nullptr)
		{
			HPComponent* hpComponent = hpObject->GetComponent<HPComponent>();
			playercomponent->SetPlayerHp(hpComponent->GetHP());
		}

		player->SetObjectType(eObjectType::Player);

		AnimationAsset* asset = RenderManager::GetInstance()->GetAnimationAssetOrNull(PLAYER_ANI_KEY);
		playerAnimation->SetAnimationAsset(asset);
		playerAnimation->SetSize({ static_cast<float>(GRID_DISTANCE * 1.2f),	static_cast<float>(GRID_DISTANCE * 1.2f) });
		playerAnimation->SetOffSet({ 0, static_cast<float>(GRID_DISTANCE * -0.3f) });
		playerAnimation->SetIsActive(true);
		playerAnimation->SetIsLoop(true);
		playerAnimation->SetTransformLayer(eTransformLayer::Grid);

		playerGrid->SetCellDistance(GRID_DISTANCE);
		playerGrid->SetMaxX(MAX_X);
		playerGrid->SetMaxY(MAX_Y);
		playerGrid->SetPosition(position);
		playerGridmove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime() / 2));

		colider->SetRadius(static_cast<float>(GRID_DISTANCE / 2.f - 30.f));
		colider->SetIsTrigger(true);

		// 플레이어 유령 노드
		GameObject* playerNode = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::PlayerNodeObject));
		Transform* playerNodeTransform = playerNode->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		GridComponent* playerNodeGrid = playerNode->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		NodeComponent* playerNodeRenderer = playerNode->CreateComponent<NodeComponent>(SoulBeaterProcessor::GetTempId());

		playerNode->SetObjectType(eObjectType::Node);

		playerNodeRenderer->SetSize({ static_cast<float>(GRID_DISTANCE), static_cast<float>(GRID_DISTANCE) });
		playerNodeRenderer->SetSpriteType(eSpriteType::Sprite);
		playerNodeRenderer->SetBaseColor({ 0,1,0,0.5 });
		playerNodeRenderer->SetTransformLayer(eTransformLayer::Grid);
		playerNodeRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(PLAYER_NODE_SPRITE_KEY));
		playerNodeRenderer->SetUVRectangle({ 0,0,240,240 });

		playerNodeGrid->SetCellDistance(GRID_DISTANCE);
		playerNodeGrid->SetMaxX(MAX_X);
		playerNodeGrid->SetMaxY(MAX_Y);
		playerNodeGrid->SetPosition({ playerGrid->GetPosition().X, playerGrid->GetPosition().Y });

		return player;
	}

	d2dFramework::GameObject* ObjectHelper::CreateBealzebub(const d2dFramework::GridPosition& position)
	{
		GameObject* BealzebubBoss = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::BelzebubObject));
		GridComponent* grid = BealzebubBoss->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		GridMoveComponent* gridmove = BealzebubBoss->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::GetTempId());
		Transform* transform = BealzebubBoss->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		CircleCollider* collider = BealzebubBoss->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());
		AnimationRenderer* renderer = BealzebubBoss->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
		BealzebubComponent* bealzebubComponent = BealzebubBoss->CreateComponent<BealzebubComponent>(SoulBeaterProcessor::GetTempId());

		BealzebubBoss->SetObjectType(eObjectType::Boss);

		renderer->SetSize({ static_cast<float>(GRID_DISTANCE * 1.2f),	static_cast<float>(GRID_DISTANCE * 1.2f) });
		renderer->SetOffSet({ 0, static_cast<float>(GRID_DISTANCE * -0.3f) });
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(BEALZEBUB_ANI_KEY));
		renderer->SetTransformLayer(eTransformLayer::Grid);
		renderer->SetAnimationIndex(0);

		grid->SetCellDistance(GRID_DISTANCE);
		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		grid->SetPosition(position);
		gridmove->SetCurrentSpeed(0.3f);

		collider->SetRadius(GRID_DISTANCE / 2.f - 20.f);
		collider->SetIsTrigger(true);

		return BealzebubBoss;
	}

	d2dFramework::GameObject* ObjectHelper::CreateEve(const d2dFramework::GridPosition& position)
	{
		using namespace d2dFramework;

		CircleCollider* colider = nullptr;
		Transform* bossTransform = nullptr;
		AnimationRenderer* bossAnimation = nullptr;
		GridComponent* bossGrid = nullptr;
		GridMoveComponent* bossGridmove = nullptr;
		EveBossComponent* bossComponent = nullptr;

		// 이브
		GameObject* EveBoss = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::EveObject));
		colider = EveBoss->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());
		bossTransform = EveBoss->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		bossAnimation = EveBoss->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
		bossGrid = EveBoss->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		bossGridmove = EveBoss->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::GetTempId());
		bossComponent = EveBoss->CreateComponent<EveBossComponent>(SoulBeaterProcessor::GetTempId());

		EveBoss->SetObjectType(eObjectType::Boss);

		bossAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(EVE_ANI_KEY));
		bossAnimation->SetSize({ static_cast<float>(GRID_DISTANCE) * 1.2f,	static_cast<float>(GRID_DISTANCE) * 1.2f });
		bossAnimation->SetOffSet({ 0, static_cast<float>(GRID_DISTANCE) * -0.3f });
		bossAnimation->SetIsActive(true);
		bossAnimation->SetIsLoop(true);
		bossAnimation->SetTransformLayer(eTransformLayer::Grid);

		bossGrid->SetCellDistance(GRID_DISTANCE);
		bossGrid->SetMaxX(MAX_X);
		bossGrid->SetMaxY(MAX_Y);
		bossGrid->SetPosition(position);
		bossGridmove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()));

		colider->SetRadius(static_cast<float>(GRID_DISTANCE) / 2.f - 20.f);
		colider->SetIsTrigger(true);

		return EveBoss;
	}

	d2dFramework::GameObject* ObjectHelper::CreateYs(const d2dFramework::GridPosition& position)
	{
		using namespace d2dFramework;

		CircleCollider* colider = nullptr;
		Transform* bossTransform = nullptr;
		AnimationRenderer* bossAnimation = nullptr;
		GridComponent* bossGrid = nullptr;
		GridMoveComponent* bossGridmove = nullptr;

		// 이스
		GameObject* YsBoss = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::YsObject));
		colider = YsBoss->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());
		bossTransform = YsBoss->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		bossAnimation = YsBoss->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
		bossGrid = YsBoss->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		bossGridmove = YsBoss->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::GetTempId());
		YsBossComponent* bossComponent = YsBoss->CreateComponent<YsBossComponent>(SoulBeaterProcessor::GetTempId());

		YsBoss->SetObjectType(eObjectType::Enemy);

		bossAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(YS_ANI_KEY));
		bossAnimation->SetSize({ static_cast<float>(GRID_DISTANCE) * 1.2f,	static_cast<float>(GRID_DISTANCE) * 1.2f });
		bossAnimation->SetOffSet({ 0, static_cast<float>(GRID_DISTANCE) * -0.3f });
		bossAnimation->SetIsActive(true);
		bossAnimation->SetIsLoop(true);
		bossAnimation->SetTransformLayer(eTransformLayer::Grid);

		bossGrid->SetCellDistance(GRID_DISTANCE);
		bossGrid->SetMaxX(MAX_X);
		bossGrid->SetMaxY(MAX_Y);
		bossGrid->SetPosition(position);
		bossGridmove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()));

		colider->SetRadius(static_cast<float>(GRID_DISTANCE) / 2.f - 20.f);
		colider->SetIsTrigger(true);

		return YsBoss;
	}
	d2dFramework::GameObject* ObjectHelper::CreateLilith(const d2dFramework::GridPosition& position)
	{
		using namespace d2dFramework;

		// 릴리스
		GameObject* Lilith = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::LilithObject));
		CircleCollider* colider = Lilith->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());
		Transform* bossTransform = Lilith->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		AnimationRenderer* bossAnimation = Lilith->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
		GridComponent* bossGrid = Lilith->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		GridMoveComponent* bossGridmove = Lilith->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::GetTempId());
		LilithBossComponent* bossComponent = Lilith->CreateComponent<LilithBossComponent>(SoulBeaterProcessor::GetTempId());

		Lilith->SetObjectType(eObjectType::Boss);

		bossAnimation->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(LILITH_ANI_KEY));
		bossAnimation->SetSize({ static_cast<float>(GRID_DISTANCE) * 1.2f,	static_cast<float>(GRID_DISTANCE) * 1.2f });
		bossAnimation->SetOffSet({ 0, static_cast<float>(GRID_DISTANCE) * -0.3f });
		bossAnimation->SetIsActive(true);
		bossAnimation->SetIsLoop(true);
		bossAnimation->SetTransformLayer(eTransformLayer::Grid);

		bossGrid->SetCellDistance(GRID_DISTANCE);
		bossGrid->SetMaxX(MAX_X);
		bossGrid->SetMaxY(MAX_Y);
		bossGrid->SetPosition(position);
		bossGridmove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()));

		colider->SetRadius(static_cast<float>(GRID_DISTANCE) / 2.f - 20.f);
		colider->SetIsTrigger(true);

		return Lilith;
	}

	d2dFramework::GameObject* ObjectHelper::CreateMonster(const d2dFramework::GridPosition& position, eMonsterType monsterType, unsigned int detectArea, unsigned int moveableBeatCount)
	{
		using namespace d2dFramework;

		GameObject* monster = nullptr;
		Transform* transform = nullptr;
		AnimationRenderer* renderer = nullptr;
		GridComponent* grid = nullptr;
		GridMoveComponent* gridmove = nullptr;
		BaseMonsterComponent* baseMonsterComponent = nullptr;
		CircleCollider* collider = nullptr;

		monster = ObjectManager::GetInstance()->CreateObject(SoulBeaterProcessor::GetTempId());
		monster->SetObjectType(eObjectType::Enemy);
		transform = monster->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		renderer = monster->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
		renderer->SetTransformLayer(eTransformLayer::Grid);
		grid = monster->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
		gridmove = monster->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::GetTempId());
		baseMonsterComponent = monster->CreateComponent<BaseMonsterComponent>(SoulBeaterProcessor::GetTempId());
		collider = monster->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());

		renderer->SetSize({ static_cast<float>(GRID_DISTANCE) * 1.2f,	static_cast<float>(GRID_DISTANCE) * 1.2f });
		renderer->SetOffSet({ 0, static_cast<float>(GRID_DISTANCE) * -0.3f });
		renderer->SetIsLoop(true);

		grid->SetCellDistance(GRID_DISTANCE);
		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		gridmove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()) / 2.f);

		baseMonsterComponent->SetDetectionArea(detectArea);
		baseMonsterComponent->SetAttackArea(0);
		baseMonsterComponent->SetMoveableBeatCount(moveableBeatCount);
		baseMonsterComponent->SetMonsterType(monsterType);

		grid->SetPosition(position);
		collider->SetRadius(static_cast<float>(GRID_DISTANCE) / 3.f);

		switch (monsterType)
		{
		case eMonsterType::Hold:
			renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(SUCCUBUS_ANI_KEY));
			break;
		case eMonsterType::Attack:
			renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(GHOST_ANI_KEY));
			break;
		case eMonsterType::Escape:
			renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(KNIGHT_ANI_KEY));
			break;
		default:
			assert(false);
			break;
		}

		EventManager::GetInstance()->RegisterBroadcastEvent("RegisterMonster", std::to_string(monster->GetId()));

		return monster;
	}

	d2dFramework::GameObject* ObjectHelper::CreateStageNameObject()
	{
		using namespace d2dFramework;

		GameObject* stageName = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::StageNameObject), true);
		Transform* transform = stageName->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		SpriteRenderer* spriteRenderer = stageName->CreateComponent<SpriteRenderer>(SoulBeaterProcessor::GetTempId());

		stageName->SetObjectType(eObjectType::UI);

		transform->SetTranslate(STAGE_NAME_OBJECT_TRANSLATE);
		transform->SetScale({ 1, -1 });

		spriteRenderer->SetTransformLayer(eTransformLayer::UI);
		spriteRenderer->SetSpriteType(eSpriteType::Sprite);
		spriteRenderer->SetSize({ 189, 104 });
		ID2D1Bitmap* bitmap = RenderManager::GetInstance()->GetBitmapOrNull(STAGE_SPRITE_KEY);
		assert(bitmap != nullptr);
		spriteRenderer->SetBitmap(bitmap);

		return stageName;
	}

	d2dFramework::GameObject* ObjectHelper::CreateCutScene(eCutSceneObjectType cutSceneObjectType)
	{
		using namespace d2dFramework;

		GameObject* cutSceneObject = nullptr;
		ID2D1Bitmap* bitmap = nullptr;

		switch (cutSceneObjectType)
		{
		case soulBeater::eCutSceneObjectType::CutScene1:
			cutSceneObject = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::CutScene1Object));
			bitmap = RenderManager::GetInstance()->GetBitmapOrNull(CUTSCENE_BEALZEBUB_SPRITE_KEY);
			break;
		case soulBeater::eCutSceneObjectType::CutScene2:
			cutSceneObject = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::CutScene2Object));
			bitmap = RenderManager::GetInstance()->GetBitmapOrNull(CUTSCENE_EVE_ESS_SPRITE_KEY);
			break;
		case soulBeater::eCutSceneObjectType::CutScene3:
			cutSceneObject = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::CutScene3Object));
			bitmap = RenderManager::GetInstance()->GetBitmapOrNull(CUTSCENE_LILITH_SPRITE_KEY);
			break;
		case soulBeater::eCutSceneObjectType::EndingCreding:
			cutSceneObject = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::CutSceneEndingObject));
			bitmap = RenderManager::GetInstance()->GetBitmapOrNull(ENDING_SPRITE_KEY);
			break;
		case soulBeater::eCutSceneObjectType::GameOver:
			cutSceneObject = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::CutSceneGameOverObject));
			bitmap = RenderManager::GetInstance()->GetBitmapOrNull(GAMEOVER_SPRITE_KEY);
			break;
		default:
			assert(false);
			break;
		}

		cutSceneObject->SetObjectType(eObjectType::None);

		Transform* transform = cutSceneObject->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
		transform->SetScale({ 1, -1 });
		transform->SetTranslate({ 0, 0 });

		CutSceneComponent* renderer = cutSceneObject->CreateComponent<CutSceneComponent>(SoulBeaterProcessor::GetTempId());

		renderer->SetTransformLayer(eTransformLayer::None);
		renderer->SetSpriteType(eSpriteType::Sprite);
		renderer->SetOffSet({ 0, 0 });
		renderer->SetSize({ 1920, 1080 });

		assert(bitmap != nullptr);
		renderer->SetBitmap(bitmap);
		renderer->SetIsActive(true);
		renderer->SetElasped(1.f);
		renderer->SetUVRectangle({ 0,0,1920, 1080 });

		return cutSceneObject;
	}
}