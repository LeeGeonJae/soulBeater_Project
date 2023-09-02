#include "ParticleEmitter.h"

#include "Particle.h"
#include "RenderManger.h"
#include "Repeller.h"

#include <random>

namespace d2dFramework
{
	ParticleEmitter::ParticleEmitter(unsigned int id, ObjectPool<Particle>& particlePool)
		: BaseEntity(id)
		, mParticlePool(particlePool)
	{
	}

	ParticleEmitter::~ParticleEmitter()
	{
		for (Particle* particle : mParticles)
		{
			mParticlePool.Return(particle);
		}

		mParticles.clear();
	}

	void ParticleEmitter::Init(const ParticleEmitterInformation& ParticleEmitterInformation)
	{
		mParticleEmitterInformation = ParticleEmitterInformation;

		mParticles.reserve(mParticleEmitterInformation.MaxParticleCount);

		ParticleInformation particleInformation;

		mbIsEmit = true;
		for (unsigned int i = 0; i < mParticleEmitterInformation.MaxParticleCount; ++i)
		{
			Particle* particle = mParticlePool.Get();
			createRandomInformaiton(&particleInformation);
			particle->Init(particleInformation);
			mParticles.push_back(particle);
		}
	}

	void ParticleEmitter::Update(float deltaTime)
	{
		ParticleInformation particleInformation;

		for (auto iter = mParticles.begin(); iter != mParticles.end(); ++iter)
		{
			Particle* particle = *iter;

			if (particle->GetIsAlive())
			{
				particle->Update(deltaTime);
			}
			else if (mbIsEmit)
			{
				createRandomInformaiton(&particleInformation);
				mParticlePool.Return(particle);
				*iter = mParticlePool.Get();
				(*iter)->Init(particleInformation);
			}
		}
	}

	void ParticleEmitter::Render(RenderManager* renderManager)
	{
		// debug render
		renderManager->DrawPoint({ mParticleEmitterInformation.Position.GetX(), mParticleEmitterInformation.Position.GetY() });

		for (Particle* particle : mParticles)
		{
			if (!particle->GetIsAlive())
			{
				continue;
			}   

			ParticleInformation& PI = particle->mParticleInformation;

			renderManager->SetTransform(
				D2D1::Matrix3x2F::Scale(PI.CurrentScale.GetX(), PI.CurrentScale.GetY())
				* D2D1::Matrix3x2F::Rotation(PI.RotationInRadian)
				* D2D1::Matrix3x2F::Translation(PI.Position.GetX(), PI.Position.GetY()));
			D2D1_COLOR_F prevColor = renderManager->SetColor(PI.CurrentColor);
			{
				switch (mParticleEmitterInformation.ParticleType)
				{
				case eParticleType::Circle:
					renderManager->DrawCircle({ 0,0 }, PI.Size);
					renderManager->FillCircle({ 0,0 }, PI.Size);
					break;
				case eParticleType::Rectangle:
					renderManager->DrawRectangle({ 0,0 }, PI.Size);
					renderManager->FillRectangle({ 0, 0 }, PI.Size);
					break;
				case eParticleType::Bitmap:
					renderManager->DrawBitMap({ 0, 0 }, PI.Size, mParticleEmitterInformation.UVRect, mParticleEmitterInformation.Bitmap, PI.CurrentColor.a);
					break;
				default:
					break;
				}
			}

			renderManager->SetColor(prevColor);
			renderManager->SetTransform(D2D1::Matrix3x2F::Identity());
		}
	}

	void ParticleEmitter::AddForce(Vector2 force)
	{
		for (auto particle : mParticles)
		{
			particle->AddForce(force);
		}
	}

	void ParticleEmitter::ApplyGravity(float gravitationalConstant)
	{
		for (auto particle : mParticles)
		{
			particle->AddForce({ 0, particle->GetMass() * gravitationalConstant });
		}
	}

	void ParticleEmitter::ApplyRepeller(const Repeller& repeller)
	{
		for (auto particle : mParticles)
		{
			Vector2 force = repeller.CalculateRepelForce(*particle);
			particle->AddForce(force);
		}
	}

	void ParticleEmitter::createRandomInformaiton(ParticleInformation* outParticleInformation)
	{
		std::random_device randomDevice; // �õ尪
		std::mt19937 generator(randomDevice());

		ParticleInformation& PI = *outParticleInformation;
		ParticleEmitterInformation PSI = mParticleEmitterInformation;

		auto makeRandomVector = [&generator](const Vector2& base, const Vector2& halfRange) -> Vector2
		{
			std::uniform_real_distribution<float> randomX(-halfRange.GetX(), halfRange.GetX());
			std::uniform_real_distribution<float> randomY(-halfRange.GetY(), halfRange.GetY());

			Vector2 result(base.GetX() + randomX(generator), base.GetY() + randomY(generator));

			return result;
		};
		auto makeRandomFloat = [&generator](float base, float halfRange) -> float
		{
			std::uniform_real_distribution<float> random(-halfRange, halfRange);

			return base + random(generator);
		};
		auto makeRandomColor = [&generator](const D2D1_COLOR_F& base, const D2D1_COLOR_F& halfRange) -> D2D1_COLOR_F
		{
			std::uniform_real_distribution<float> randomColorR(-halfRange.r, halfRange.r);
			std::uniform_real_distribution<float> randomColorG(-halfRange.g, halfRange.g);
			std::uniform_real_distribution<float> randomColorB(-halfRange.b, halfRange.b);
			std::uniform_real_distribution<float> randomColorA(-halfRange.a, halfRange.a);

			D2D1_COLOR_F result =
			{
				base.r + randomColorR(generator),
				base.g + randomColorG(generator),
				base.b + randomColorB(generator),
				base.a + randomColorA(generator)
			};

			return result;
		};

		// translate
		PI.Position = PSI.Position + makeRandomVector(PSI.Offset, PSI.OffsetHalfRange);
		PI.Velocity = makeRandomVector(PSI.Velocity, PSI.VelocityHalfRange);
		PI.Acceleration = makeRandomVector(PSI.Acceleration, PSI.AccelerationHalfRange);

		// scale
		PI.Size = makeRandomVector(PSI.Size, PSI.SizeHalfRange);
		PI.StartScale = makeRandomVector(PSI.StartScale, PSI.StartScaleHalfRange);
		PI.EndScale = makeRandomVector(PSI.EndScale, PSI.EndScaleHalfRange);

		// rotate
		PI.RotationInRadian = makeRandomFloat(PSI.RotationInRadian, PSI.RotationInRadianHalfRange);
		PI.AngularVelocity = makeRandomFloat(PSI.AngularVelocity, PSI.AngularVelocityHalfRange);

		// color
		PI.StartColor = makeRandomColor(PSI.StartColor, PSI.StartColorHalfRange);
		PI.EndColor = makeRandomColor(PSI.EndColor, PSI.EndColorHalfRange);

		// mass
		PI.Mass = makeRandomFloat(PSI.Mass, PSI.MassHalfRange);

		// lifeSpan
		PI.TimeToLive = makeRandomFloat(PSI.TimeToLive, PSI.TimeToLiveHalfRange);
		PI.LifeRatio = 1 / PI.TimeToLive;
		PI.TimeToLive = 1.f;
	}
}