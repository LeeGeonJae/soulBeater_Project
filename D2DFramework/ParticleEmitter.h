#pragma once

#include "Vector2.h"
#include "ObjectPool.h"
#include "BaseEntity.h"

#include <vector>

namespace d2dFramework
{
	class RenderManager;
	class Particle;
	class Repeller;

	struct ParticleInformation;

	enum class eParticleType
	{
		Circle,
		Rectangle,
		Bitmap
	};

	struct ParticleEmitterInformation
	{
		Vector2 Position;
		unsigned int MaxParticleCount;

		Vector2 Offset;
		Vector2 OffsetHalfRange; // 0.5f

		Vector2 Size;
		Vector2 SizeHalfRange;

		float RotationInRadian;
		float RotationInRadianHalfRange;

		// 프로젝트에 필요한 파티클에 따라 속도 또한 보간하여 처리
		Vector2 Velocity;
		Vector2 VelocityHalfRange;
		Vector2 Acceleration;
		Vector2 AccelerationHalfRange;

		Vector2 StartScale;
		Vector2 StartScaleHalfRange;
		Vector2 EndScale;
		Vector2 EndScaleHalfRange;

		float AngularVelocity;
		float AngularVelocityHalfRange;

		D2D1_COLOR_F StartColor;
		D2D1_COLOR_F StartColorHalfRange;

		D2D1_COLOR_F EndColor;
		D2D1_COLOR_F EndColorHalfRange;

		float Mass;
		float MassHalfRange;

		float TimeToLive;
		float TimeToLiveHalfRange;

		eParticleType ParticleType;
		ID2D1Bitmap* Bitmap;
		D2D1_RECT_F UVRect;
	};

	class ParticleEmitter : public BaseEntity
	{
		friend class ParticleManager;

	public:
		void Init(const ParticleEmitterInformation& ParticleEmitterInformation);
		void Update(float deltaTime);
		void Render(RenderManager* renderManager);

		// 사용하지 않게 될 거 같다. 추후에 수정
		void AddForce(Vector2 force);
		void ApplyGravity(float gravitationalConstant);
		void ApplyRepeller(const Repeller& repeller);

		inline void MovePosition(const Vector2& move);

		inline void SetIsEmit(bool bIsEmit);
		inline void SetParticleSystemInformation(const ParticleEmitterInformation& particleInfomation);

		inline bool GetIsEmit()const;
		inline const ParticleEmitterInformation& GetParticleSysteInformation() const;

	private:
		ParticleEmitter(unsigned int id, ObjectPool<Particle>& particlePool);
		~ParticleEmitter();

		void createRandomInformaiton(ParticleInformation* outParticleInformation);

	private:
		ObjectPool<Particle>& mParticlePool;

		std::vector<Particle*> mParticles;
		ParticleEmitterInformation mParticleEmitterInformation;
		bool mbIsEmit;
	};

	void ParticleEmitter::MovePosition(const Vector2& move)
	{
		mParticleEmitterInformation.Position += move;
	}

	void ParticleEmitter::SetIsEmit(bool bIsEmit)
	{
		mbIsEmit = bIsEmit;
	}
	void ParticleEmitter::SetParticleSystemInformation(const ParticleEmitterInformation& particleInfomation)
	{
		mParticleEmitterInformation = particleInfomation;
	}

	bool ParticleEmitter::GetIsEmit() const
	{
		return mbIsEmit;
	}

	const ParticleEmitterInformation& ParticleEmitter::GetParticleSysteInformation() const
	{
		return mParticleEmitterInformation;
	}
};