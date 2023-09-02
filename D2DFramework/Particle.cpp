#include "Particle.h"

#include "ParticleEmitter.h"
#include "RenderManger.h"
#include "MathHelper.h"

namespace d2dFramework
{
	void Particle::Init(const ParticleInformation& particleInformation)
	{
		mParticleInformation = particleInformation;
	}

	void Particle::Update(float deltaTime)
	{
		if (mParticleInformation.TimeToLive < 0.f)
		{
			return;
		}

		ParticleInformation& PI = mParticleInformation;
		float normalizedDistance = 1 - PI.TimeToLive;

		// translate
		PI.Velocity += PI.Acceleration * deltaTime;
		PI.Velocity += PI.Impurse;
		PI.Impurse.SetXY(0.f, 0.f);
		PI.Position += PI.Velocity * deltaTime;

		// rotate
		PI.RotationInRadian += PI.AngularVelocity * deltaTime;

		// scale
		PI.CurrentScale.SetXY(
			MathHelper::Interpolation(PI.StartScale.GetX(), PI.EndScale.GetX(), normalizedDistance),
			MathHelper::Interpolation(PI.StartScale.GetY(), PI.EndScale.GetY(), normalizedDistance)
		);

		// color
		PI.CurrentColor =
		{
			MathHelper::Interpolation(PI.StartColor.r, PI.EndColor.r, normalizedDistance),
			MathHelper::Interpolation(PI.StartColor.g, PI.EndColor.g, normalizedDistance),
			MathHelper::Interpolation(PI.StartColor.b, PI.EndColor.b, normalizedDistance),
			MathHelper::Interpolation(PI.StartColor.a, PI.EndColor.a, normalizedDistance)
		};

		// lifeSpan;
		PI.TimeToLive -= deltaTime * PI.LifeRatio;
	}

	void Particle::Render(RenderManager* renderManager)
	{
		ParticleInformation& PI = mParticleInformation;

		if (PI.TimeToLive < 0.f)
		{
			return;
		}

		renderManager->SetTransform(
			D2D1::Matrix3x2F::Scale(PI.CurrentScale.GetX(), PI.CurrentScale.GetY())
			* D2D1::Matrix3x2F::Rotation(PI.RotationInRadian)
			* D2D1::Matrix3x2F::Translation(PI.Position.GetX(), PI.Position.GetY()));
		D2D1_COLOR_F prevColor = renderManager->SetColor(PI.CurrentColor);
		{
			renderManager->DrawRectangle({ 0,0 }, PI.Size);
			renderManager->FillRectangle({ 0, 0 }, PI.Size);
		}
		renderManager->SetColor(prevColor);
		renderManager->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}