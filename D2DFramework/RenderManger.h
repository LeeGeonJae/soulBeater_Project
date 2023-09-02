#pragma once

#include "BaseEntity.h"
#include "FrameInformation.h"
#include "eObjectType.h"

#include <cassert>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <string>
#include <map>
#include <unordered_map>

namespace d2dFramework
{
	class Vector2;
	class AnimationAsset;
	class IRenderable;
	class CameraManager;

	class RenderManager final : public BaseEntity
	{
		friend class GameProcessor;

	public:
		static RenderManager* GetInstance();

		void RegisterRenderable(IRenderable* renderable);
		void UnregisterRenderable(IRenderable* renderable);

		void BitmapBeginDraw();
		void BitmapEndDraw();
		void BeginDraw();
		void EndDraw();
		void Clear(D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Identity(), D2D1_COLOR_F color = { 1.f, 1.f, 1.f, 1.f });
		void CopyBitmapRenderToHwndRender(D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Identity());

		void DrawRadialGradiendBrush(const D2D1_ELLIPSE& ellipse);
		void CreateRadialGradientBrush();

		void DrawPoint(float x, float y);
		void DrawPoint(const D2D1_POINT_2F& point);

		void DrawLine(float x1, float y1, float x2, float y2);
		void DrawLine(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end);

		void DrawCircle(float x, float y, float radiusX, float radiusY);
		void DrawCircle(const Vector2& offset, const Vector2& size);
		void DrawCircle(const D2D1_ELLIPSE& ellipse);
		void FillCircle(float x, float y, float radiusX, float radiusY);
		void FillCircle(const Vector2& offset, const Vector2& size);
		void FillCircle(const D2D1_ELLIPSE& ellipse);

		void DrawRectangle(float left, float top, float right, float bottom);
		void DrawRectangle(const Vector2& offset, const Vector2& size);
		void DrawRectangle(const D2D1_RECT_F& rectangle);
		void FillRectangle(float left, float top, float right, float bottom);
		void FillRectangle(const Vector2& offset, const Vector2& size);
		void FillRectangle(const D2D1_RECT_F& rectangle);

		void DrawPolygon(const std::vector<D2D1_POINT_2F>& pointList);
		void DrawGrid(float x, float y, float width, float height, float interval);

		void DrawBitMap(float left, float top, float right, float bottom, float uvLeft, float uvTop, float uvRight, float uvBottom, ID2D1Bitmap* bitmap, float alpha = 1.f);
		void DrawBitMap(const Vector2& offset, const Vector2& size, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap, float alpha = 1.f);
		void DrawBitMap(const D2D1_RECT_F& rectangle, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap, float alpha = 1.f);

		void WriteText(const wchar_t* text, float left, float top, float right, float bottom);
		void WriteText(const wchar_t* text, const D2D1_RECT_F& rectangle);

		HRESULT CreateD2DBitmapFromFile(const WCHAR* imagePath);
		HRESULT CreateD2DBitmapFromFile(const WCHAR* key, const WCHAR* imagePath);
		HRESULT CreateAnimationAsset(const WCHAR* imagePath, const std::vector<std::vector<FrameInformation>>& frameInfo);
		HRESULT CreateAnimationAsset(const WCHAR* key, const WCHAR* imagePath, const std::vector<std::vector<FrameInformation>>& frameInfo);

		// 실제 적용까지 동작한다.
		inline void SetTransform(const D2D1::Matrix3x2F& trasform);
		inline void SetGridObjectTransform(const D2D1::Matrix3x2F& gridObjectTransform);
		inline void SetUITransform(const D2D1::Matrix3x2F& UITransform);
		inline void SetFontSize(unsigned int fontSize);
		inline D2D1_COLOR_F SetColor(const D2D1_COLOR_F& color);
		inline void SetStrokeWidth(float strokeWidth);

		inline const D2D1::Matrix3x2F& GetTransform() const;
		inline const D2D1::Matrix3x2F& GetGridObjectTransform() const;
		inline const D2D1::Matrix3x2F& GetUITransform() const;
		inline ID2D1DeviceContext* GetD2DDeviceContext() const;
		inline ID2D1Bitmap* GetBitmapOrNull(const WCHAR* key) const;
		inline AnimationAsset* GetAnimationAssetOrNull(const WCHAR* key) const;
		inline ID2D1BitmapRenderTarget* GetBitmapRenderTarget() const;
		inline ID2D1HwndRenderTarget* GetRenderTarget() const;

	private:
		RenderManager();
		~RenderManager() = default;
		RenderManager(const RenderManager&) = delete;
		RenderManager& operator=(const RenderManager&) = delete;

		void init(HWND hwnd);
		void release();
		void render(CameraManager* cameraManager);

		void DiscardDeviceResource();
		HRESULT createDeviceResources(HWND hWnd);

	private:
		enum { INIT_FONT_SIZE = 15 };
		enum { INIT_STROKE_SIZE = 2 };
		enum { RESERVE_SIZE = 2048 };

		static RenderManager* mInstance;

		HWND mHwnd;

		ID2D1Factory* mFactory;
		ID2D1HwndRenderTarget* mRenderTarget;
		ID2D1BitmapRenderTarget* mBitmapRenderTarget;
		ID2D1DeviceContext* mD2DDeviceContext;
		IWICImagingFactory* mWICFactory;
		IDWriteFactory* mWriteFactory;

		IDWriteTextFormat* mTextFormat;
		ID2D1SolidColorBrush* mBrush;
		float mStrokeWidth;
		D2D1::ColorF mBeforeColor;
		std::map<std::wstring, ID2D1Bitmap*> mBitmapMap;
		std::map<std::wstring, AnimationAsset*> mAnimationAssetMap;

		std::unordered_map<unsigned int, IRenderable*> mRenderable[static_cast<unsigned int>(eObjectType::Size)];

		// 조명효과
		ID2D1GradientStopCollection* mGradientStops;
		ID2D1RadialGradientBrush* mRadialGradientBrush;
		ID2D1LinearGradientBrush* mLinearGradientBrush;
		ID2D1Layer* mLayer;

		D2D1::Matrix3x2F mTransform;
		D2D1::Matrix3x2F mGridObjectTransform;
		D2D1::Matrix3x2F mUITransform;
	};

	void RenderManager::SetTransform(const D2D1::Matrix3x2F& trasform)
	{
		mBitmapRenderTarget->SetTransform(trasform);
	}

	void RenderManager::SetGridObjectTransform(const D2D1::Matrix3x2F& gridObjectTransform)
	{
		mGridObjectTransform = gridObjectTransform;
	}

	void RenderManager::SetUITransform(const D2D1::Matrix3x2F& UITransform)
	{
		mUITransform = UITransform;
	}

	void RenderManager::SetFontSize(unsigned int size)
	{
		if (mTextFormat != nullptr)
		{
			mTextFormat->Release();
			mTextFormat = nullptr;
		}

		HRESULT hr;
		hr = mWriteFactory->CreateTextFormat(L"Gulim", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, static_cast<float>(size), L"ko-KR", &mTextFormat);

		assert(SUCCEEDED(hr));
	}
	D2D1_COLOR_F RenderManager::SetColor(const D2D1_COLOR_F& color)
	{
		if (mBrush != nullptr)
		{
			D2D1_COLOR_F before = mBrush->GetColor();
			mBrush->SetColor(color);

			return before;
		}

		HRESULT hr = mBitmapRenderTarget->CreateSolidColorBrush(color, &mBrush);
		assert(SUCCEEDED(hr));

		return mBrush->GetColor();
	}
	void RenderManager::SetStrokeWidth(float strokeWidth)
	{
		mStrokeWidth = strokeWidth;
	}

	const D2D1::Matrix3x2F& RenderManager::GetTransform() const
	{
		return mTransform;
	}

	const D2D1::Matrix3x2F& RenderManager::GetGridObjectTransform() const
	{
		return mGridObjectTransform;
	}

	const D2D1::Matrix3x2F& RenderManager::GetUITransform() const
	{
		return mUITransform;
	}

	ID2D1DeviceContext* RenderManager::GetD2DDeviceContext() const
	{
		assert(mD2DDeviceContext != nullptr);
		return mD2DDeviceContext;
	}

	ID2D1Bitmap* RenderManager::GetBitmapOrNull(const WCHAR* imangePath) const
	{
		auto iter = mBitmapMap.find(imangePath);

		return iter == mBitmapMap.end() ? nullptr : iter->second;
	}

	AnimationAsset* RenderManager::GetAnimationAssetOrNull(const WCHAR* imangePath) const
	{
		auto iter = mAnimationAssetMap.find(imangePath);

		return iter == mAnimationAssetMap.end() ? nullptr : iter->second;
	}

	ID2D1BitmapRenderTarget* RenderManager::GetBitmapRenderTarget() const
	{
		assert(mBitmapRenderTarget != nullptr);

		return mBitmapRenderTarget;
	}

	ID2D1HwndRenderTarget* RenderManager::GetRenderTarget() const
	{
		assert(mRenderTarget != nullptr);

		return mRenderTarget;
	}
}