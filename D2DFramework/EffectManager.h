#pragma once

#include "BaseEntity.h"

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1effects.h>

namespace d2dFramework
{
	class EffectManager final : public BaseEntity
	{
	public:
		EffectManager();
		~EffectManager();
		EffectManager(const EffectManager&) = delete;
		EffectManager& operator=(const EffectManager&) = delete;

		void Init(ID2D1DeviceContext* d2dDeviceContext);
		void Release();

		ID2D1Bitmap* CreateSpotLight(ID2D1Bitmap* bitmap
			, D2D1_VECTOR_3F _lightPosition = { 0.f, 0.f, 0.f }
			, D2D1_VECTOR_3F _pointAt = { 0.f, 0.f ,0.f }
			, float _focus = 1.f // 0 ~ 200
			, float _limitConeAngle = 90.f // 0 ~ 90
			, float _diffuse = 1.f // 1 ~ 10000
			, D2D1_VECTOR_3F _color = { 1.f, 1.f ,1.f });
		ID2D1Bitmap* CreateDirectionBlur(ID2D1Bitmap* bitmap, float blur = 3.f, float angle = 0.f);
		ID2D1Bitmap* CreateGaussianBlur(ID2D1Bitmap* bitmap, float blurVal = 3.f); // 가우시안 필터만 비트맵 생성이 안된다.
		ID2D1Bitmap* CreateBlendGaussianBlur(ID2D1Bitmap* bitmap, float alpha = 0.5f, float blurVal = 3.f);
		ID2D1Bitmap* CreateVignetteEffect(ID2D1Bitmap* bitmap, D2D1::ColorF rgb = { 0.f, 0.f, 0.f }, float vignetteSize = 0.1f, float vignetteStrength = 0.5f);
		ID2D1Bitmap* CreateEdgeDetection(ID2D1Bitmap* bitmap, float strength = 0.5f, float blurRadius = 0.f); // strength 0 ~ 1, blurRadius 0 ~ 10
		ID2D1Bitmap* CreateMorphology(ID2D1Bitmap* bitmap, UINT width = 1u, UINT height = 1u);
		ID2D1Bitmap* CreateInvert(ID2D1Bitmap* bitmap);
		ID2D1Bitmap* CreateGrayScale(ID2D1Bitmap* bitmap);

		// 비트맵에 대해서 모든 효과 연속적으로 가하는 함수 만들기

	private:
		ID2D1Bitmap* convertImageToBitmap(ID2D1Image* pImg, D2D1_SIZE_U size);
		void createHelperBitmap();

	private:
		ID2D1DeviceContext* mD2DDeviceContext;

		ID2D1Effect* mSpotLight;
		ID2D1Effect* mDirectionBlur;
		ID2D1Effect* mGaussianBlur;
		ID2D1Effect* mVignetteEffect;
		ID2D1Effect* mAlphaBlendEffect;
		ID2D1Effect* mOpacityEffect;
		ID2D1Effect* mEdgeDetection;
		ID2D1Effect* mMorphology;
		ID2D1Effect* mInvert;
		ID2D1Effect* mGrayScale;

		ID2D1Image* mImage;
		ID2D1Bitmap1* mBlackBackground;
	};
}