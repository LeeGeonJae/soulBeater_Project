#include "BealzebubComponent.h"

namespace TestGeon
{
	BealzebubComponent::BealzebubComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component::Component(id, owner)
		, mMoveableBeatCount(0)
		, mBeatElapsed(0)
	{
	}

	void BealzebubComponent::Init()
	{
	}
}