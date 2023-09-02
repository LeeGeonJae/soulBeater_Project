#include "Repeller.h"

#include "Particle.h"
#include "MathHelper.h"
#include "RenderManger.h"

namespace d2dFramework
{
	Repeller::Repeller(const Vector2& position, float power)
		: mPosition(position)
		, mPower(power)
	{
	}

	void Repeller::Render(RenderManager* renderManager)
	{
		renderManager->DrawCircle(mPosition, { 30,30 });
	}

	Vector2 Repeller::CalculateRepelForce(const Particle& particle) const
	{
		Vector2 direction = particle.GetPosition() - mPosition;
		float distance = direction.GetMagnitudeSquared();

		distance = MathHelper::Clamp(distance, 1, 10000);

		if (distance > 100)
		{
			return Vector2(0, 0);
		}
		float force = mPower / distance;

		direction.Normalize();

		return direction * force;
	}
}