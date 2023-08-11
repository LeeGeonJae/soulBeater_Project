#pragma once

#include "Component.h"
#include "ICollideable.h"
#include "Vector2.h"

#include <map>

namespace d2dFramework
{
	class GameObject;

	class Collider abstract : public ICollideable, public ISerialize
	{
	public:
		Collider(eColliderType colliderType);
		virtual ~Collider() = default;
		virtual void SerializeOut(nlohmann::ordered_json& object) override;
		virtual void SerializeIn(nlohmann::ordered_json& object) override;
		virtual void UpdateCollider() = 0;
		virtual bool CheckCollision(ICollideable* other, Manifold* outManifold) = 0;
		virtual inline GameObject* GetGameObject() const = 0;
		virtual inline unsigned int GetId() const = 0;

		virtual void Init() override;
		virtual void Release() override;

		void HandleRigidbody(ICollideable* other, const Manifold& manifold) final;
		void OnEnterCollision(ICollideable* other) override final;
		void OnCollision(ICollideable* other) override final;
		void OnExitCollision(ICollideable* other) override final;

		inline void RegisterOnEnterCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback) override final;
		inline void RegisterOnCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback) override final;
		inline void RegisterOnExitCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback) override final;

		inline void UnregisterOnEnterCollisionCallBack(unsigned int id) override final;
		inline void UnregisterOnCollisionCallBack(unsigned int id) override final;
		inline void UnregisterOnExitCollisionCallBack(unsigned int id) override final;

		inline void SetIsTrigger(bool bisTrigger);
		inline void SetOffSet(const Vector2& offset);

		inline eColliderType GetColliderType() const override final;
		inline bool GetIsTrigger() const;
		inline const Vector2& GetOffset() const;

	protected:
		bool mbIsTrigger;

		Vector2 mOffset;
		Vector2 mSize;

		std::map<unsigned int, std::function<void(unsigned int)>> mOnEnterCollisionCallBaak;
		std::map<unsigned int, std::function<void(unsigned int)>> mOnCollisionCallBaak;
		std::map<unsigned int, std::function<void(unsigned int)>> mOnExitCollisionCallBaak;

		eColliderType mColliderType;
	};

	void Collider::RegisterOnEnterCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback)
	{
		mOnEnterCollisionCallBaak.insert({ id, callback });
	}
	void Collider::RegisterOnCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback)
	{
		mOnCollisionCallBaak.insert({ id, callback });
	}
	void Collider::RegisterOnExitCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback)
	{
		mOnExitCollisionCallBaak.insert({ id, callback });
	}

	void Collider::UnregisterOnEnterCollisionCallBack(unsigned int id)
	{
		mOnEnterCollisionCallBaak.erase(id);
	}
	void Collider::UnregisterOnCollisionCallBack(unsigned int id)
	{
		mOnCollisionCallBaak.erase(id);
	}
	void Collider::UnregisterOnExitCollisionCallBack(unsigned int id)
	{
		mOnExitCollisionCallBaak.erase(id);
	}

	void Collider::SetIsTrigger(bool bisTrigger)
	{
		mbIsTrigger = bisTrigger;
	}
	void Collider::SetOffSet(const Vector2& offset)
	{
		mOffset = offset;
	}

	eColliderType Collider::GetColliderType() const
	{
		return mColliderType;
	}
	bool Collider::GetIsTrigger() const
	{
		return mbIsTrigger;
	}
	const Vector2& Collider::GetOffset() const
	{
		return mOffset;
	}
}