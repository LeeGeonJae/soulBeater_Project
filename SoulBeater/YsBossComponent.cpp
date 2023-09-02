#include "YsBossComponent.h"

#include "ObjectManager.h"
#include "GameObject.h"
#include "CircleCollider.h"

namespace soulBeater
{
	YsBossComponent::YsBossComponent(unsigned int id, d2dFramework::GameObject* owner)
		:Component::Component(id, owner)
		, mMoveableBeatCount(0)
		, mBeatElapsed(0)
	{
	}

	void YsBossComponent::Init()
	{
		using namespace d2dFramework;


	}
}
