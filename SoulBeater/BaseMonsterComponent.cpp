#include "BaseMonsterComponent.h"

#include "ObjectManager.h"

#include "EventManager.h"
#include "GameObject.h"
#include "CircleCollider.h"
#include "SpriteRenderer.h"

#include <string>

namespace soulBeater
{
	BaseMonsterComponent::BaseMonsterComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component(id, owner)
		, mDectectionArea(0u)
		, mAttackArea(0u)
		, mMoveableBeatCount(1u)
		, mHp(1u)
		, mBeatElapsed(0.f)
	{
	}

	void BaseMonsterComponent::Init()
	{
		using namespace d2dFramework;

		CircleCollider* collider = GetGameObject()->GetComponent<CircleCollider>();

		auto CollisionEnderRegister = [this](unsigned int playerid)
		{
			GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(playerid);

			CircleCollider* playercollider = player->GetComponent<CircleCollider>();

			if (player->GetObjectType() == eObjectType::Player)
			{
				assert(playercollider != nullptr);

				--mHp;
			}
		};

		collider->RegisterOnEnterCollisionCallBack(GetGameObject()->GetId(), CollisionEnderRegister);

	}

	void BaseMonsterComponent::Release()
	{
		using namespace d2dFramework;

		CircleCollider* collider = GetGameObject()->GetComponent<CircleCollider>();
		collider->UnregisterOnEnterCollisionCallBack(GetGameObject()->GetId());

		EventManager::GetInstance()->RegisterBroadcastEvent("UnregisterMonster", std::to_string(GetGameObject()->GetId()));
	}
}  