#include "EveDaggerComponent.h"

#include "BeatManager.h"
#include "ObjectManager.h"

#include "GameObject.h"
#include "CircleCollider.h"
#include "GridMoveComponent.h"
#include "GridComponent.h"
#include "SpriteRenderer.h"

#include "ConstantTable.h"

namespace soulBeater
{
	EveDaggerComponent::EveDaggerComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component::Component(id, owner)
		, mBeatManager(nullptr)
		, mbIsLeft(false)
		, mCurrentBeat(0)
		, mNextBeat(0)
	{
	}

	void EveDaggerComponent::Init()
	{
		using namespace d2dFramework;

		IUpdateable::Init();

		CircleCollider* collider = Component::GetGameObject()->GetComponent<CircleCollider>();

		auto collisionEnter = [this](unsigned int playerid)
		{
			GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(playerid);

			if (player->GetObjectType() == eObjectType::Player)
			{
				GridMoveComponent* gridmove = player->GetComponent<GridMoveComponent>();
				assert(gridmove != nullptr);
				assert(Component::GetGameObject()->GetComponent<CircleCollider>() != nullptr);
				assert(Component::GetGameObject()->GetComponent<GridComponent>() != nullptr);
				assert(Component::GetGameObject()->GetComponent<SpriteRenderer>() != nullptr);

				if (gridmove->GetIsCanMove())
				{
					ObjectManager::GetInstance()->DeleteObject(Component::GetGameObject()->GetId());
				}
			}
		};

		collider->RegisterOnCollisionCallBack(Component::GetGameObject()->GetId(), collisionEnter);
		collider->SetIsTrigger(true);
	}

	void EveDaggerComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		GridComponent* grid = Component::GetGameObject()->GetComponent<GridComponent>();

		if (grid->GetPosition().X >= grid->GetMaxX() || grid->GetPosition().X <= 1)
		{
			ObjectManager::GetInstance()->DeleteObject(Component::GetGameObject()->GetId());
		}

		static unsigned int elased = 0;
		elased += deltaTime;

		mNextBeat = static_cast<unsigned int>(elased / mBeatManager->GetIntervalTime());

		if (mCurrentBeat + 1 != mNextBeat)
		{
			mCurrentBeat = mNextBeat;

			GridMoveComponent* gridmove = Component::GetGameObject()->GetComponent<GridMoveComponent>();
			SpriteRenderer* render = Component::GetGameObject()->GetComponent<SpriteRenderer>();

			if (gridmove->GetIsCanMove())
			{
				if (render->GetIsLeft())
				{
					gridmove->Move(-static_cast<int>(MAX_X) + 1, 0);
				}
				else
				{
					gridmove->Move(static_cast<int>(MAX_X), 0);
				}
			}
		}
	}
	void EveDaggerComponent::Release()
	{
		IUpdateable::Release();
	}
}