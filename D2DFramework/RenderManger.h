#pragma once

#include "BaseEntity.h"
#include "FrameInfomation.h"
#include "eObjectType.h"

#include <d2d1.h>
#include <d2d1_1.h>
#include <dxgi1_2.h>
#include <d3d11.h>
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
		void Present();

		void Render(CameraManager* cameraManager);
		void Clear(D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Identity(), D2D1_COLOR_F color = { 1.f, 1.f, 1.f, 1.f });

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

		void DrawBitMap(float left, float top, float right, float bottom, float uvLeft, float uvTop, float uvRight, float uvBottom, ID2D1Bitmap1* bitmap);
		void DrawBitMap(const Vector2& offset, const Vector2& size, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap1* bitmap);
		void DrawBitMap(const D2D1_RECT_F& rectangle, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap1* bitmap);

		void WriteText(const wchar_t* text, float left, float top, float right, float bottom);
		void WriteText(const wchar_t* text, const D2D1_RECT_F& rectangle);

		HRESULT CreateD2DBitmapFromFile(const WCHAR* imagePath);
		HRESULT CreateD2DBitmapFromFile(const WCHAR* key, const WCHAR* imagePath);
		HRESULT CreateAnimationAsset(const WCHAR* imagePath, const std::vector<std::vector<FrameInfomation>>& frameInfo);
		HRESULT CreateAnimationAsset(const WCHAR* key, const WCHAR* imagePath, const std::vector<std::vector<FrameInfomation>>& frameInfo);

		inline void SetHwnd(HWND Hwnd);
		void SetTransform(const D2D1::Matrix3x2F& trasform);
		void SetFontSize(unsigned int fontSize);
		D2D1_COLOR_F SetColor(const D2D1_COLOR_F& color);
		void SetStrokeWidth(float strokeWidth);

		inline HWND GetHwnd() const;
		inline ID3D11Device* GetD3DDevice() const;
		inline ID3D11DeviceContext* GetD3DDeviceContext() const;
		inline ID2D1Bitmap1* GetBitmapOrNull(const WCHAR* key);
		inline AnimationAsset* GetAnimationAssetOrNull(const WCHAR* key);

	private:
		HRESULT createDeviceResources(HWND hWnd);

	private:
		enum { INIT_FONT_SIZE = 15 };
		enum { INIT_STROKE_SIZE = 2 };
		enum { RESERVE_SIZE = 2048 };

		HWND mHwnd;

		ID2D1Factory1* mFactory;
		ID2D1Device* mD2DDevice;
		ID2D1DeviceContext* mD2DDeviceContext;
		ID2D1Bitmap1* mD2DRenderTarget;

		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DDeviceContext;
		IDXGISwapChain1* mSwapChain;
		ID3D11RenderTargetView* mD3DRenderTarget;

		IWICImagingFactory* mWICFactory;
		IDWriteFactory* mWriteFactory;
		IDWriteTextFormat* mTextFormat;
		ID2D1SolidColorBrush* mBrush;
		float mStrokeWidth;
		D2D1::ColorF mBeforeColor;
		std::map<std::wstring, ID2D1Bitmap1*> mBitmapMap;
		std::map<std::wstring, AnimationAsset*> mAnimationAssetMap;

		std::unordered_map<unsigned int, IRenderable*> mRenderable[static_cast<unsigned int>(eObjectType::Size)];
	};

	void RenderManager::SetHwnd(HWND Hwnd)
	{
		mHwnd = Hwnd;
	}

	HWND RenderManager::GetHwnd() const
	{
		return mHwnd;
	}
	ID3D11Device* RenderManager::GetD3DDevice() const
	{
		return mD3DDevice;
	}
	ID3D11DeviceContext* RenderManager::GetD3DDeviceContext() const
	{
		return mD3DDeviceContext;
	}

	ID2D1Bitmap1* RenderManager::GetBitmapOrNull(const WCHAR* imangePath)
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