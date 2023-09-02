#pragma once

#include "ObjectPool.h"
#include "BaseEntity.h"
#include "eFrameworkID.h"
#include "Particle.h"

#include <unordered_map>

namespace d2dFramework
{
	class ParticleEmitter;

	class ParticleManager final : public BaseEntity
	{
	public:
		ParticleManager();
		~ParticleManager();

		void Release();

		ParticleEmitter* CreateParticleEmitter(unsigned int id);
		inline ParticleEmitter* FindParticleEmitterOrNull(unsigned int id);
		void DeleteParticleEmitter(unsigned int id);

	private:
		enum { RESERVE_SIZE = 1024 };
		enum { POOL_RESERVE_SIZE = 4096 };

		std::unordered_map<unsigned int, ParticleEmitter*> mParticleEmitterMap;
		ObjectPool<Particle> mParticlePool;
	};

	ParticleEmitter* ParticleManager::FindParticleEmitterOrNull(unsigned int id)
	{
		auto iter = mParticleEmitterMap.find(id);

		if (iter == mParticleEmitterMap.end())
		{
			return nullptr;
		}

		return iter->second;
	}
}