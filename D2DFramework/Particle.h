#pragma once

#include "Vector2.h"

namespace d2dFramework
{
	class RenderManager;

	struct ParticleInformation
	{
		Vector2 Position;
		Vector2 Size;
		float RotationInRadian;

		Vector2 Velocity;
		Vector2 Acceleration;
		Vector2 Impurse;

		Vector2 StartScale;
		Vector2 CurrentScale;
		Vector2 EndScale;

		float AngularVelocity;

		D2D1_COLOR_F StartColor;
		D2D1_COLOR_F CurrentColor;
		D2D1_COLOR_F EndColor;

		float Mass;
		float InverseMass;

		float TimeToLive;
		float LifeRatio;
	};

	class Particle
	{
		friend class ParticleEmitter;

	public:
		Particle() = default;
		~Particle() = default;

		void Init(const ParticleInformation& particleInformation);
		void Update(float deltaTime);
		void Render(RenderManager* renderManager);

		inline void AddForce(Vector2 force);

		inline const Vector2& GetPosition() const;
		inline bool GetIsAlive() const;
		inline float GetMass() const;

	private:
		ParticleInformation mParticleInformation;
	};

	void Particle::AddForce(Vector2 force)
	{
		force *= mParticleInformation.InverseMass;
		mParticleInformation.Impurse += force;
	}
	bool Particle::GetIsAlive() const
	{
		return mParticleInformation.TimeToLive >= 0.f;
	}

	float Particle::GetMass() const
	{
		return mParticleInformation.Mass;
	}

	const Vector2& Particle::GetPosition() const
	{
		return mParticleInformation.Position;
	}
}