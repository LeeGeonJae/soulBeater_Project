#pragma once

#include "Component.h"
#include "IUpdateable.h"

namespace d2dFramework
{
	class BeatManager;
}

namespace soulBeater
{
	class EveDaggerComponent : public d2dFramework::IUpdateable, public d2dFramework::Component
	{
	public:
		EveDaggerComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~EveDaggerComponent() = default;

		virtual void Init() override;
		virtual void Update(float deltaTime);
		virtual void Release() override;

		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline d2dFramework::GameObject* GetGameObject() const;
		inline unsigned int GetId() const;

	public:
		inline void SetBeatManager(d2dFramework::BeatManager* beatManager);

	private:
		d2dFramework::BeatManager* mBeatManager;

		bool mbIsLeft;
		unsigned int mCurrentBeat;
		unsigned int mNextBeat;
	};

	void EveDaggerComponent::SetBeatManager(d2dFramework::BeatManager* beatManager)
	{
		mBeatManager = beatManager;
	}

	d2dFramework::GameObject* EveDaggerComponent::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int EveDaggerComponent::GetId() const
	{
		return BaseEntity::GetId();
	}
}


