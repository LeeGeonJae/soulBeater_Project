#include "EveBossComponent.h"

#include "ObjectManager.h"
#include "GameObject.h"
#include "CircleCollider.h"

namespace soulBeater
{
	EveBossComponent::EveBossComponent(unsigned int id, d2dFramework::GameObject* owner)
		:Component::Component(id, owner)
		, mMoveableBeatCount(0)
		, mHp(10)
		, mBeatElapsed(0)
	{
	}

	void EveBossComponent::Init()
	{
	}
}
