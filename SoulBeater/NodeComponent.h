#pragma once

#include "Component.h"
#include "IRenderable.h"
#include "SpriteRenderer.h"
#include "Vector2.h"
#include "eTransformLayer.h"

#include <d2d1.h>
#include <d2d1_1.h>

namespace soulBeater
{
	class d2dFramework::GameObject;

	enum class eNodeState
	{
		UnableMove,
		Moveable,
		MoveSuccess,
		MoveFailed,
	};

	class NodeComponent final : public d2dFramework::SpriteRenderer
	{
	public:
		NodeComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~NodeComponent() override = default;

		virtual void Init() override;
		virtual void Release() override;

		virtual void Render(const D2D1::Matrix3x2F& cameraTransform) override;

	public:
		inline eNodeState GetNodeState() const;
		inline void SetNodeState(eNodeState nextNodeState);

	private:
		void nodeRender();

	private:
		eNodeState mNodeState;
	};

	eNodeState NodeComponent::GetNodeState() const
	{
		return mNodeState;
	}

	void NodeComponent::SetNodeState(eNodeState nextNodeState)
	{
		mNodeState = nextNodeState;
	}
}