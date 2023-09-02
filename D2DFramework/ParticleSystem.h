#pragma once

#include "Vector2.h"

#include <vector>

namespace d2dFramework
{
	class RenderManager;
	class Particle;

	class ParticleSystem
	{
	public:
		ParticleSystem(Vector2 position, size_t particleSize);
		~ParticleSystem();

		void Update(float deltaTime);
		void Render(RenderManager* renderManager);

		void AddForce(Vector2 force);
		// void ApplyRepeller()

	private:
		Vector2 mPosition;
		size_t mParticleSize;
		std::vector<Particle*> mParticles;
	};
}
