#include "DaggerComponent.h"

#include "BeatManager.h"
#include "ObjectManager.h"

#include "GameObject.h"
#include "CircleCollider.h"
#include "GridMoveComponent.h"
#include "GridComponent.h"
#include "AABBCollider.h"
#include "SpriteRenderer.h"

namespace TestGeon
{
	DaggerComponent::DaggerComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component::Component(id, owner)
		, mBeatManager(nullptr)
		, mbIsLeft(false)
		, mCurrentBeat(0)
		, mNextBeat(0)
	{
	}

	void DaggerComponent::Init()
	{
		using namespace d2dFramework;

		IUpdateable::Init();

		CircleCollider* collider = Component::GetGameObject()->GetComponent<CircleCollider>();

		auto collisionEnter = [this](unsigned int otherid)
		{
			GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(otherid);

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

			GameObject* Wall = ObjectManager::GetInstance()->FindObjectOrNull(otherid);

			if (Wall->GetObjectType() == eObjectType::Wall)
			{
				AABBCollider* collider = Wall->GetComponent<AABBCollider>();

				assert(collider != nullptr);
				assert(Component::GetGameObject()->GetComponent<GridComponent>() != nullptr);

				ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
			}
		};

		collider->RegisterOnCollisionCallBack(Component::GetGameObject()->GetId(), collisionEnter);
		collider->SetIsTrigger(true);
	}

	void DaggerComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		GridComponent* grid = Component::GetGameObject()->GetComponent<GridComponent>();

		if (grid->GetPosition().X >= grid->GetMaxX() - 1 || grid->GetPosition().X <= 1)
		{
			ObjectManager::GetInstance()->DeleteObject(Component::GetGameObject()->GetId());
		}

		static unsigned int elased = 0;
		elased += deltaTime;
		
		mNextBeat = elased / mBeatManager->GetIntervalTime();

		if (mCurrentBeat + 1 != mNextBeat)
		{
			mCurrentBeat = mNextBeat;

			GridMoveComponent* gridmove = Component::GetGameObject()->GetComponent<GridMoveComponent>();
			SpriteRenderer* render = Component::GetGameObject()->GetComponent<SpriteRenderer>();

			if (gridmove->GetIsCanMove())
			{
				if (render->GetIsLeft())
				{
					gridmove->Move(-1, 0);
				}
				else
				{
					gridmove->Move(1, 0);
				}
			}
		}
	}
	void DaggerComponent::Release()
	{
		IUpdateable::Release();
	}
}