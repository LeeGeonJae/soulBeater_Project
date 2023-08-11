#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include "Component.h"
#include "IRenderable.h"
#include "Vector2.h"

namespace d2dFramework
{
	class GameObject;

	enum class eSpriteType
	{
		Rectangle,
		Circle,
		Sprite
	};

	class SpriteRenderer final : public Component, public IRenderable
	{
	public:
		SpriteRenderer(unsigned int id, GameObject* owner);
		~SpriteRenderer() override = default;

		void Init() override;
		void Release() override;

		bool IsOutsideBoundingBox(const D2D1::Matrix3x2F& cameraTransform, const AABB& boundingBox) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;
		
		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;
		
		inline void SetOffSet(const Vector2& offset);
		inline void SetSize(const Vector2& size);
		inline void SetBaseColor(const D2D1_COLOR_F& baseColor);
		inline void SetBorderColor(const D2D1_COLOR_F& borderColor);
		inline void SetBitmap(ID2D1Bitmap* bitmap);
		inline void SetBitmap(ID2D1Bitmap* bitmap,std::wstring bitmapKey);

		inline void SetUVRectangle(const D2D1_RECT_F& rectangle);
		inline void SetSpriteType(eSpriteType spriteType);

		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline const Vector2& GetOffset(void) const;
		inline const Vector2& GetSize(void) const;
		inline const D2D1_COLOR_F& GetBaseColor(void) const;
		inline const D2D1_COLOR_F& GetBorderColor(void) const;
		inline ID2D1Bitmap* GetBitmap(void) const;
		inline const D2D1_RECT_F& GetUVRectangle(void) const;
		inline eSpriteType GetSpriteType(void) const;

	private:
		Vector2 mOffset;
		Vector2 mSize;

		D2D1_COLOR_F mBaseColor;
		D2D1_COLOR_F mBorderColor;

		ID2D1Bitmap* mBitmap;
		D2D1_RECT_F mUVRectangle;
		std::string mBitmapKey;
		eSpriteType mSpriteType;
	};

	void SpriteRenderer::SetOffSet(const Vector2& offset)
	{
		mOffset = offset;
	}
	void SpriteRenderer::SetSize(const Vector2& size)
	{
		mSize = size;
	}
	void SpriteRenderer::SetBaseColor(const D2D1_COLOR_F& baseColor)
	{
		mBaseColor = baseColor;
	}
	void SpriteRenderer::SetBorderColor(const D2D1_COLOR_F& borderColor)
	{
		mBorderColor = borderColor;
	}
	void SpriteRenderer::SetBitmap(ID2D1Bitmap* bitmap)
	{
		mBitmap = bitmap;
	}
	void SpriteRenderer::SetBitmap(ID2D1Bitmap* bitmap, std::wstring bitmapKey)
	{
		mBitmap = bitmap;
		std::string str(bitmapKey.begin(), bitmapKey.end());
		mBitmapKey = str;
	}
	void SpriteRenderer::SetUVRectangle(const D2D1_RECT_F& rectangle)
	{
		mUVRectangle = rectangle;
	}
	void SpriteRenderer::SetSpriteType(eSpriteType spriteType)
	{
		mSpriteType = spriteType;
	}

	GameObject* SpriteRenderer::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int SpriteRenderer::GetId() const
	{
		return BaseEntity::GetId();
	}
	const Vector2& SpriteRenderer::GetOffset() const
	{
		return mOffset;
	}
	const Vector2& SpriteRenderer::GetSize() const
	{
		return mSize;
	}
	const D2D1_COLOR_F& SpriteRenderer::GetBaseColor(void) const
	{
		return mBaseColor;
	}
	const D2D1_COLOR_F& SpriteRenderer::GetBorderColor(void) const
	{
		return mBorderColor;
	}
	ID2D1Bitmap* SpriteRenderer::GetBitmap(void) const
	{
		return mBitmap;
	}
	const D2D1_RECT_F& SpriteRenderer::GetUVRectangle(void) const
	{
		return mUVRectangle;
	}
	eSpriteType SpriteRenderer::GetSpriteType(void) const
	{
		return mSpriteType;
	}
}