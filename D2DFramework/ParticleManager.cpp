#include "ParticleManager.h"

#include "ParticleEmitter.h"

namespace d2dFramework
{
	ParticleManager::ParticleManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::ParticleManager))
		, mParticlePool(POOL_RESERVE_SIZE)
	{
		mParticleEmitterMap.reserve(RESERVE_SIZE);
	}
	ParticleManager::~ParticleManager()
	{
		Release();
	}

	void ParticleManager::Release()
	{
		for (auto iter = mParticleEmitterMap.begin(); iter != mParticleEmitterMap.end(); ++iter)
		{
			ParticleEmitter* particleEmitter = iter->second;
			delete particleEmitter;
		}

		mParticleEmitterMap.clear();
	}

	ParticleEmitter* ParticleManager::CreateParticleEmitter(unsigned int id)
	{
		ParticleEmitter* particleEmitter = new ParticleEmitter(id, mParticlePool);
		mParticleEmitterMap.insert({ id, particleEmitter });

		return particleEmitter;
	}

	void ParticleManager::DeleteParticleEmitter(unsigned int id)
	{
		auto iter = mParticleEmitterMap.find(id);

		if (iter == mParticleEmitterMap.end())
		{
			return;
		}

		delete iter->second;
		mParticleEmitterMap.erase(iter);
	}
}