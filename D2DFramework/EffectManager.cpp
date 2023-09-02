#include "EffectManager.h"

#include "eFrameworkID.h"

#include <string>
#include <cassert>
#include <d2d1effects_2.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

namespace d2dFramework
{
	EffectManager::EffectManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::EffectManager))
		, mD2DDeviceContext(nullptr)
		, mSpotLight(nullptr)
		, mDirectionBlur(nullptr)
		, mGaussianBlur(nullptr)
		, mVignetteEffect(nullptr)
		, mAlphaBlendEffect(nullptr)
		, mOpacityEffect(nullptr)
		, mEdgeDetection(nullptr)
		, mMorphology(nullptr)
		, mInvert(nullptr)
		, mGrayScale(nullptr)
		, mImage(nullptr)
		, mBlackBackground(nullptr)
	{
	}

	EffectManager::~EffectManager()
	{
		Release();
	}

	void EffectManager::Init(ID2D1DeviceContext* d2dDeviceContext)
	{
		mD2DDeviceContext = d2dDeviceContext;
		Release();

		mD2DDeviceContext->CreateEffect(CLSID_D2D1SpotSpecular, &mSpotLight);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1DirectionalBlur, &mDirectionBlur);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &mGaussianBlur);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1Vignette, &mVignetteEffect);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1Blend, &mAlphaBlendEffect);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1Opacity, &mOpacityEffect);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1EdgeDetection, &mEdgeDetection);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1Morphology, &mMorphology);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1Invert, &mInvert);
		mD2DDeviceContext->CreateEffect(CLSID_D2D1Grayscale, &mGrayScale);

		createHelperBitmap();
	}

	void EffectManager::Release()
	{
		if (mSpotLight != nullptr) { mSpotLight->Release(); mSpotLight = nullptr; }
		if (mGaussianBlur != nullptr) { mGaussianBlur->Release(); mGaussianBlur = nullptr; }
		if (mVignetteEffect != nullptr) { mVignetteEffect->Release(); mVignetteEffect = nullptr; }
		if (mAlphaBlendEffect != nullptr) { mAlphaBlendEffect->Release(); mAlphaBlendEffect = nullptr; }
		if (mOpacityEffect != nullptr) { mOpacityEffect->Release(); mOpacityEffect = nullptr; }
		if (mEdgeDetection != nullptr) { mEdgeDetection->Release(); mEdgeDetection = nullptr; }
		if (mMorphology != nullptr) { mMorphology->Release(); mMorphology = nullptr; }
		if (mInvert != nullptr) { mInvert->Release(); mInvert = nullptr; }
		if (mGrayScale != nullptr) { mGrayScale->Release(); mGrayScale = nullptr; }
	}

	ID2D1Bitmap* EffectManager::CreateSpotLight(ID2D1Bitmap* bitmap
		, D2D1_VECTOR_3F _lightPosition
		, D2D1_VECTOR_3F _pointAt
		, float _focus
		, float _limitConeAngle
		, float _diffuse
		, D2D1_VECTOR_3F _color)
	{
		ID2D1Image* image = nullptr;

		mSpotLight->SetInput(0, bitmap);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_LIGHT_POSITION, _lightPosition);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_POINTS_AT, _pointAt);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_FOCUS, _focus);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_LIMITING_CONE_ANGLE, _limitConeAngle);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_SPECULAR_CONSTANT, _diffuse);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_COLOR, _color);
		mSpotLight->SetValue(D2D1_SPOTSPECULAR_PROP_SPECULAR_EXPONENT, 128);

		mSpotLight->GetOutput(&image);

		mAlphaBlendEffect->SetInput(0, bitmap);
		// mAlphaBlendEffect->SetInput(1, mBlackBackground);
		// mAlphaBlendEffect->SetInput(2, image);
		mAlphaBlendEffect->SetValue(D2D1_BLEND_PROP_MODE, 0);
		ID2D1Image* blendImage = nullptr;

		mAlphaBlendEffect->GetOutput(&blendImage);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
		, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(blendImage, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateDirectionBlur(ID2D1Bitmap* bitmap, float blur, float angle)
	{
		ID2D1Image* image = nullptr;

		mDirectionBlur->SetInput(0, bitmap);
		mDirectionBlur->SetValue(D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION, blur);
		mDirectionBlur->SetValue(D2D1_DIRECTIONALBLUR_PROP_ANGLE, angle);
		mDirectionBlur->GetOutput(&image);

		D2D1_SIZE_U size =
		{
			static_cast<UINT32>(bitmap->GetSize().width),
			static_cast<UINT32>(bitmap->GetSize().height)
		};

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateGaussianBlur(ID2D1Bitmap* bitmap, float blurVal)
	{
		ID2D1Image* image = nullptr;

		mGaussianBlur->SetInput(0, bitmap);
		mGaussianBlur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, blurVal);
		mGaussianBlur->GetOutput(&image);

		D2D1_SIZE_U size =
		{
			static_cast<UINT32>(bitmap->GetSize().width),
			static_cast<UINT32>(bitmap->GetSize().height)
		};

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateBlendGaussianBlur(ID2D1Bitmap* bitmap, float alpha, float blurVal)
	{
		ID2D1Image* opacityImage = nullptr;

		mOpacityEffect->SetInput(0, bitmap);
		mOpacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, alpha);
		mOpacityEffect->GetOutput(&opacityImage);

		ID2D1Image* blurImage = nullptr;
		mGaussianBlur->SetInput(0, bitmap);
		mGaussianBlur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, blurVal);
		mGaussianBlur->GetOutput(&blurImage);

		mAlphaBlendEffect->SetInput(0, blurImage);
		mAlphaBlendEffect->SetInput(1, opacityImage);
		mAlphaBlendEffect->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_MULTIPLY);

		ID2D1Image* blendImage = nullptr;
		mAlphaBlendEffect->GetOutput(&blendImage);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(blendImage, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateVignetteEffect(ID2D1Bitmap* bitmap, D2D1::ColorF rgb, float vignetteSize, float vignetteStrength)
	{
		ID2D1Image* image = nullptr;

		mVignetteEffect->SetInput(0, bitmap);
		mVignetteEffect->SetValue(D2D1_VIGNETTE_PROP_COLOR, rgb);
		mVignetteEffect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, vignetteSize);
		mVignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, vignetteStrength);

		mVignetteEffect->GetOutput(&image);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
		, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateEdgeDetection(ID2D1Bitmap* bitmap, float strength, float blurRadius)
	{
		ID2D1Image* image = nullptr;

		mEdgeDetection->SetInput(0, bitmap);
		mEdgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_STRENGTH, strength);
		mEdgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_BLUR_RADIUS, blurRadius);
		mEdgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_MODE, D2D1_EDGEDETECTION_MODE_SOBEL);
		mEdgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_OVERLAY_EDGES, false);
		mEdgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_ALPHA_MODE, D2D1_ALPHA_MODE_PREMULTIPLIED);
		mEdgeDetection->GetOutput(&image);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
		, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateMorphology(ID2D1Bitmap* bitmap, UINT width, UINT height)
	{
		ID2D1Image* image = nullptr;

		mMorphology->SetInput(0, bitmap);
		mMorphology->SetValue(D2D1_MORPHOLOGY_PROP_MODE, D2D1_MORPHOLOGY_MODE_ERODE);
		mMorphology->SetValue(D2D1_MORPHOLOGY_PROP_WIDTH, width);
		mMorphology->SetValue(D2D1_MORPHOLOGY_PROP_HEIGHT, height);
		mMorphology->GetOutput(&image);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
		, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::CreateInvert(ID2D1Bitmap* bitmap)
	{
		ID2D1Image* image = nullptr;

		mInvert->SetInput(0, bitmap);
		mInvert->GetOutput(&image);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
		, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}
	ID2D1Bitmap* EffectManager::CreateGrayScale(ID2D1Bitmap* bitmap)
	{
		ID2D1Image* image = nullptr;

		mGrayScale->SetInput(0, bitmap);
		mGrayScale->GetOutput(&image);

		D2D1_SIZE_U size = { static_cast<UINT32>(bitmap->GetSize().width)
		, static_cast<UINT32>(bitmap->GetSize().height) };

		ID2D1Bitmap* result = convertImageToBitmap(image, size);

		return result;
	}

	ID2D1Bitmap* EffectManager::convertImageToBitmap(ID2D1Image* img, D2D1_SIZE_U size)
	{
		ID2D1Image* oldTarget = NULL;
		ID2D1Bitmap1* targetBitmap = NULL;

		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			);
		HRESULT hr = mD2DDeviceContext->CreateBitmap(size, 0, 0, bitmapProperties, &targetBitmap);
		assert(SUCCEEDED(hr));

		mD2DDeviceContext->GetTarget(&oldTarget);
		mD2DDeviceContext->SetTarget(targetBitmap);
		{
			mD2DDeviceContext->BeginDraw();
			mD2DDeviceContext->Clear();
			mD2DDeviceContext->DrawImage(img);
			hr = mD2DDeviceContext->EndDraw();
			//assert(SUCCEEDED(hr));
		}
		mD2DDeviceContext->SetTarget(oldTarget);

		oldTarget->Release();

		return targetBitmap;
	}

	void EffectManager::createHelperBitmap()
	{
		ID2D1Image* oldTarget = NULL;

		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			);

		HRESULT hr = mD2DDeviceContext->CreateBitmap({ 1902, 1022 }, 0, 0, bitmapProperties, &mBlackBackground);
		assert(SUCCEEDED(hr));

		mD2DDeviceContext->GetTarget(&oldTarget);
		mD2DDeviceContext->SetTarget(mBlackBackground);
		{
			mD2DDeviceContext->BeginDraw();
			mD2DDeviceContext->Clear({ 1.f,1.f,1.f,0.3f });
			hr = mD2DDeviceContext->EndDraw();
			//assert(SUCCEEDED(hr));
		}
		mD2DDeviceContext->SetTarget(oldTarget);

		oldTarget->Release();
	}
}