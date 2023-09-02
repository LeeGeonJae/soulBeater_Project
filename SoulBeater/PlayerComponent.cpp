#include "PlayerComponent.h"

#include "ObjectManager.h"

#include "GridComponent.h"
#include "CircleCollider.h"
#include "EventManager.h"
#include "PlayerFSM.h"
#include "ConstantTable.h"

namespace soulBeater
{
	PlayerComponent::PlayerComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component::Component(id, owner)
		, mMoveableBeatCount(0)
		, mBeatElapsed(0)
		, mHp(50)
	{

	}

	void PlayerComponent::Init()
	{
		auto OtherColliderMonserCallback = [this](unsigned int monsterid)->void
		{
			d2dFramework::GameObject* other = ObjectManager::GetInstance()->FindObjectOrNull(monsterid);

			if (other->GetObjectType() == eObjectType::Enemy)
			{
				assert(other->GetComponent<Transform>() != nullptr);
				assert(other->GetComponent<GridComponent>() != nullptr);
				assert(other->GetComponent<GridMoveComponent>() != nullptr);

				if (mPlayerCurrentState == ePlayerState::Idle || mPlayerCurrentState == ePlayerState::BeAttacked)
				{
					mPlayerNextState = ePlayerState::BeAttacked;
					mHp -= 0;
					mHitEffectElpased = 0.15f;
				}
				if (mPlayerCurrentState == ePlayerState::Dash)
				{
					mPlayerNextState = ePlayerState::Attack;
					mHp += 1;
					EventManager::GetInstance()->ExcuteBroadcastEvent(KNIGHT_EFFECT_EVENT, "");
				}
			}

		};

		auto OtherColliderCallback = [this](unsigned int otherid)->void
		{
			d2dFramework::GameObject* other = ObjectManager::GetInstance()->FindObjectOrNull(otherid);

			if (other->GetObjectType() == eObjectType::Projectile)
			{
				assert(other->GetComponent<Transform>() != nullptr);
				assert(other->GetComponent<GridComponent>() != nullptr);

				if (mPlayerCurrentState == ePlayerState::Idle)
				{
					mPlayerNextState = ePlayerState::BeAttacked;
					mHp -= 1;
					mHitEffectElpased = 0.15f;
				}
			}
		};

		d2dFramework::GameObject* player = GetGameObject();

		assert(player != nullptr);

		CircleCollider* playercollider = player->GetComponent<CircleCollider>();
		playercollider->RegisterOnEnterCollisionCallBack(player->GetId(), OtherColliderMonserCallback);
		playercollider->RegisterOnCollisionCallBack(player->GetId(), OtherColliderCallback);
		playercollider->SetIsTrigger(true);
	}
}