#pragma once

#include "Vector2.h"

namespace d2dFramework
{
	class Particle;
	class RenderManager;

	class Repeller
	{
	public:
		Repeller(const Vector2& position, float power);
		~Repeller() = default;

		void Render(RenderManager* renderManager);

		Vector2 CalculateRepelForce(const Particle& particle) const;

	private:
		Vector2 mPosition;
		float mPower;
	};
}