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
	public:
		RenderManager();
		~RenderManager() = default;

		void Init();
		void Release();

		void RegisterRenderable(IRenderable* renderable);
		void UnregisterRenderable(IRenderable* renderable);

		void BeginDraw();
		void EndDraw();
		void Clear(D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Identity(), D2D1_COLOR_F color = { 1.f, 1.f, 1.f, 1.f });

		void Render(CameraManager* cameraManager);

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

		void DrawBitMap(float left, float top, float right, float bottom, float uvLeft, float uvTop, float uvRight, float uvBottom, ID2D1Bitmap* bitmap);
		void DrawBitMap(const Vector2& offset, const Vector2& size, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap);
		void DrawBitMap(const D2D1_RECT_F& rectangle, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap);

		void WriteText(const wchar_t* text, float left, float top, float right, float bottom);
		void WriteText(const wchar_t* text, const D2D1_RECT_F& rectangle);

		HRESULT CreateD2DBitmapFromFile(const WCHAR* imagePath);
		HRESULT CreateD2DBitmapFromFile(const WCHAR* key, const WCHAR* imagePath);
		HRESULT CreateAnimationAsset(const WCHAR* imagePath, const std::vector<std::vector<FrameInformation>>& frameInfo);
		HRESULT CreateAnimationAsset(const WCHAR* key, const WCHAR* imagePath, const std::vector<std::vector<FrameInformation>>& frameInfo);

		inline void SetHwnd(HWND Hwnd);
		void SetTransform(const D2D1::Matrix3x2F& trasform);
		void SetFontSize(unsigned int fontSize);
		D2D1_COLOR_F SetColor(const D2D1_COLOR_F& color);
		void SetStrokeWidth(float strokeWidth);

		inline ID2D1DeviceContext* GetD2DDeviceContext() const;
		inline ID2D1Bitmap* GetBitmapOrNull(const WCHAR* key);
		inline AnimationAsset* GetAnimationAssetOrNull(const WCHAR* key);

	private:
		HRESULT createDeviceResources(HWND hWnd);

	private:
		enum { INIT_FONT_SIZE = 15 };
		enum { INIT_STROKE_SIZE = 2 };
		enum { RESERVE_SIZE = 2048 };

		HWND mHwnd;

		ID2D1Factory* mFactory;
		ID2D1HwndRenderTarget* mRenderTarget;
		ID2D1DeviceContext* mD2DDeviceContext;
		IWICImagingFactory* mWICFactory;
		IDWriteFactory* mWriteFactory;

		IDWriteTextFormat* mTextFormat;
		ID2D1SolidColorBrush* mBrush;
		float mStrokeWidth;
		D2D1::ColorF mBeforeColor;

		std::unordered_map<unsigned int, IRenderable*> mRenderable[static_cast<unsigned int>(eObjectType::Size)];
		std::map<std::wstring, ID2D1Bitmap*> mBitmapMap;
		std::map<std::wstring, AnimationAsset*> mAnimationAssetMap;
	};

	void RenderManager::SetHwnd(HWND Hwnd)
	{
		mHwnd = Hwnd;
	}

	ID2D1DeviceContext* RenderManager::GetD2DDeviceContext() const
	{
		assert(mD2DDeviceContext != nullptr);
		return mD2DDeviceContext;
	}

	ID2D1Bitmap* RenderManager::GetBitmapOrNull(const WCHAR* imangePath)
	{
		auto iter = mBitmapMap.find(imangePath);

		return iter == mBitmapMap.end() ? nullptr : iter->second;
	}

	inline AnimationAsset* RenderManager::GetAnimationAssetOrNull(const WCHAR* imangePath)
	{
		auto iter = mAnimationAssetMap.find(imangePath);

		return iter == mAnimationAssetMap.end() ? nullptr : iter->second;
	}
}