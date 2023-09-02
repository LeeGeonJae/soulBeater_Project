#include "PlayerComponent.h"

#include "ObjectManager.h"
#include "GridComponent.h"
#include "CircleCollider.h"

namespace TestGeon
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

				if (mPlayerCurrentState == ePlayerState::Idle)
				{
					mPlayerNextState = ePlayerState::BeAttacked;
					mHp -= 10;
				}
				if (mPlayerCurrentState == ePlayerState::Dash)
				{
					mPlayerNextState = ePlayerState::Attack;
				}
			}
			if (other->GetObjectType() == eObjectType::Projectile)
			{
				assert(other->GetComponent<Transform>() != nullptr);
				assert(other->GetComponent<GridComponent>() != nullptr);

				if (mPlayerCurrentState == ePlayerState::Idle)
				{
					mPlayerNextState = ePlayerState::BeAttacked;
					mHp -= 10;
				}
			}
		};

		d2dFramework::GameObject* player = GetGameObject();

		assert(player != nullptr);

		CircleCollider* playercollider = player->GetComponent<CircleCollider>();
		playercollider->RegisterOnEnterCollisionCallBack(player->GetId(), OtherColliderMonserCallback);
		playercollider->SetIsTrigger(true);
	}
}