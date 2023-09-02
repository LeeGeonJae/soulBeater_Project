#pragma once

#include <cassert>

#include "Vector2.h"
#include "Component.h"
#include "GameObject.h"
#include "ISerialize.h"

namespace d2dFramework
{
	class Transform final : public Component
	{
	public:
		Transform(unsigned int id, GameObject* owner);
		virtual ~Transform() = default;

		virtual void Init() {}
		virtual void Release() {}

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		inline void SetScale(const Vector2& scale);
		inline void SetRotate(float rotateInDegree);
		inline void SetTranslate(const Vector2& translate);

		inline void AddScale(const Vector2& scale);
		inline void AddRotate(float rotateInDegree);
		inline void AddTranslate(const Vector2& translate);

		inline const Vector2& GetScale(void) const;
		inline float GetRotate(void) const;
		inline const Vector2& GetTranslate(void) const;

		inline const D2D1::Matrix3x2F GetSTTansform(void) const;
		inline const D2D1::Matrix3x2F GetTransform(void) const;
		inline const D2D1::Matrix3x2F GetInverseTransform(void) const;

	private:
		Vector2 mScale;
		float mRotateInDegree;
		Vector2 mTranslate;
	};

	void Transform::SetScale(const Vector2& scale)
	{
		mScale = scale;
	}
	void Transform::SetRotate(float rotateInDegree)
	{
		mRotateInDegree = rotateInDegree;
	}
	void Transform::SetTranslate(const Vector2& translate)
	{
		mTranslate = translate;
	}

	void Transform::AddScale(const Vector2& scale)
	{
		mScale += scale;
	}
	void Transform::AddRotate(float rotateInDegree)
	{
		mRotateInDegree += rotateInDegree;
	}
	void Transform::AddTranslate(const Vector2& translate)
	{
		mTranslate += translate;
	}

	const Vector2& Transform::GetScale(void) const
	{
		return mScale;
	}
	float Transform::GetRotate(void) const
	{
		return mRotateInDegree;
	}
	const Vector2& Transform::GetTranslate(void) const
	{
		return mTranslate;
	}

	const D2D1::Matrix3x2F Transform::GetSTTansform(void) const
	{
		GameObject* parent = GetGameObject()->GetParentOrNull();

		D2D1::Matrix3x2F result = D2D1::Matrix3x2F::Scale({ mScale.GetX(), mScale.GetY() }) * D2D1::Matrix3x2F::Translation({ mTranslate.GetX(), mTranslate.GetY() });

		if (parent == nullptr)
		{
			return result;
		}
		else
		{
			return result * parent->GetComponent<Transform>()->GetTransform();
		}
	}

	const D2D1::Matrix3x2F Transform::GetTransform(void) const
	{
		GameObject* parent = GetGameObject()->GetParentOrNull();

		D2D1::Matrix3x2F result = D2D1::Matrix3x2F::Scale({ mScale.GetX(), mScale.GetY() }) * D2D1::Matrix3x2F::Rotation(mRotateInDegree) * D2D1::Matrix3x2F::Translation({ mTranslate.GetX(), mTranslate.GetY() });

		if (parent == nullptr)
		{
			return result;
		}
		else
		{
			return result * parent->GetComponent<Transform>()->GetTransform();
		}
	}
	const D2D1::Matrix3x2F Transform::GetInverseTransform(void) const
	{
		D2D1::Matrix3x2F matrix = GetTransform();
		D2D1InvertMatrix(&matrix);

		return matrix;
	}
}