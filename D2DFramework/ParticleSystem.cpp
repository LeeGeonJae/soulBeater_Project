#include "ParticleSystem.h"
#include "Particle.h"

namespace d2dFramework
{
	ParticleSystem::ParticleSystem(Vector2 position, size_t particleSize)
		: mPosition(position)
		, mParticleSize(particleSize)
	{
		mParticles.resize(mParticleSize, new Particle({ 0, static_cast<float>(rand() % 10 - 5) }, { 0, static_cast<float>(rand() % 10 - 5) }, { 0, 0 }, { static_cast<float>(rand() % 20 + 10), static_cast<float>(rand() % 20 + 10) }, rand() % 50 + 25, static_cast<float>(rand() % 3 + 1)));
	}
	ParticleSystem::~ParticleSystem()
	{
		for (Particle* particle : mParticles)
		{
			delete particle;
		}

		mParticles.clear();
	}

	void ParticleSystem::Update(float deltaTime)
	{
		for (Particle* particle : mParticles)
		{
			if (particle->GetIsAlive())
			{
				particle->Update(deltaTime);
			}
			else
			{
				delete particle;
				particle = new Particle({ 0, static_cast<float>(rand() % 10 - 5) }, { 0, static_cast<float>(rand() % 10 - 5) }, { 0, 0 }, { static_cast<float>(rand() % 20 + 10), static_cast<float>(rand() % 20 + 10) }, rand() % 50 + 25, static_cast<float>(rand() % 3 + 1));
			}
		}
	}

	void ParticleSystem::Render(RenderManager* renderManager)
	{
		for (auto particle : mParticles)
		{
			particle->Render(renderManager);
		}
	}

	void ParticleSystem::AddForce(Vector2 force)
	{
		for (auto particle : mParticles)
		{
			particle->AddForce(force);
		}
	}
}