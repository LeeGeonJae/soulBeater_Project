#pragma once

#include "Vector2.h"
#include "Component.h"
#include "IFixedUpdateable.h"
#include "GameObject.h"

namespace d2dFramework
{
	class Rigidbody final : public Component, public IFixedUpdateable
	{
	public:
		Rigidbody(unsigned int id, GameObject* owner);
		~Rigidbody() = default;

		void Init() override;
		void fixedUpdate(float deltaTime) override;
		void Release() override;
		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;
		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;

		inline void SetVelocity(const Vector2& velocity);
		inline void SetAcceleartion(const Vector2& acceleration);
		inline void SetMass(float mass);
		inline void SetCOR(float COR);
		inline void SetGravityScale(float gravityScale);

		inline void AddVelocity(const Vector2& velocity);
		inline void AddAcceleartion(const Vector2& acceleration);

		inline const Vector2& GetVelocity(void);
		inline const Vector2& GetAcceleartion(void);
		inline float GetMass(void);
		inline float GetInvMass(void);
		inline float GetCOR(void);

	private:
		static float mGravityWeight;

		Vector2 mVelocity;
		Vector2 mAcceleartion;

		float mMass;
		float mInvMass;
		float mCOR; // 반발계수 0~1
		float mGravityScale;
	};

	GameObject* Rigidbody::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int Rigidbody::GetId() const
	{
		return BaseEntity::GetId();
	}

	void Rigidbody::SetVelocity(const Vector2& velocity)
	{
		mVelocity = velocity;
	}
	void Rigidbody::SetAcceleartion(const Vector2& acceleration)
	{
		mAcceleartion = acceleration;
	}
	void Rigidbody::SetMass(float mass)
	{
		mMass = mass;

		if (mass != 0.f)
		{
			mInvMass = 1 / mass;
		}
		else
		{
			mInvMass = 0.f;
		}
	}
	void Rigidbody::SetCOR(float COR)
	{
		mCOR = COR;

		if (COR > 1.f)
		{
			COR = 1.f;
		}
		else if (COR < 0.f)
		{
			COR = 0.f;
		}
	}

	void Rigidbody::SetGravityScale(float gravityScale)
	{
		mGravityScale = gravityScale;
	}

	void Rigidbody::AddVelocity(const Vector2& velocity)
	{
		mVelocity += velocity;
	}
	void Rigidbody::AddAcceleartion(const Vector2& acceleration)
	{
		mAcceleartion += acceleration;
	}

	const Vector2& Rigidbody::GetVelocity(void)
	{
		return mVelocity;
	}
	const Vector2& Rigidbody::GetAcceleartion(void)
	{
		return mAcceleartion;
	}
	float Rigidbody::GetMass(void)
	{
		return mMass;
	}
	float Rigidbody::GetInvMass(void)
	{
		return mInvMass;
	}
	float Rigidbody::GetCOR(void)
	{
		return mCOR;
	}
}