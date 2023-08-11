#include "RenderManger.h"

#include "GameObject.h"
#include "IRenderable.h"
#include "eFrameworkID.h"
#include "AnimationAsset.h"
#include "MathHelper.h"
#include "Vector2.h"
#include "CameraManager.h"
#include "AABB.h"

#include <cassert>
#include <d2d1helper.h>
#include <comdef.h>
#include <wincodec.h>
#include <combaseapi.h>

#define DEFAULT_DPI 96.f

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace d2dFramework
{
	RenderManager::RenderManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::RenderManager))
		, mFactory(nullptr)
		, mRenderTarget(nullptr)
		, mWICFactory(nullptr)
		, mTextFormat(nullptr)
		, mD2DDeviceContext(nullptr)
		, mBeforeColor{ 0.f, }
		, mHwnd(NULL)
	{
	}

	void RenderManager::Init()
	{
		HRESULT hr;

		hr = CoInitialize(NULL);
		assert(SUCCEEDED(hr));

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mFactory);
		assert(SUCCEEDED(hr));

		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&mWICFactory)
		);
		assert(SUCCEEDED(hr));

		RECT rc;
		GetClientRect(mHwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			static_cast<UINT>(rc.right - rc.left),
			static_cast<UINT>(rc.bottom - rc.top)
		);
		hr = mFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&mRenderTarget);
		assert(SUCCEEDED(hr));

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&mWriteFactory));
		assert(SUCCEEDED(hr));

		SetFontSize(INIT_FONT_SIZE);
		SetColor({ 0.f,0.f,0.f,1.f });
		SetStrokeWidth(INIT_STROKE_SIZE);

		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			mRenderable[i].reserve(RESERVE_SIZE);
		}

		hr = mRenderTarget->QueryInterface(&mD2DDeviceContext);
		assert(SUCCEEDED(hr));
	}

	void RenderManager::Release()
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			mRenderable[i].clear();
		}

		mTextFormat->Release(); mTextFormat = nullptr;
		mFactory->Release(); mFactory = nullptr;
		mRenderTarget->Release(); mRenderTarget = nullptr;
		mWICFactory->Release(); mWICFactory = nullptr;
		mWriteFactory->Release(); mWriteFactory = nullptr;
		mD2DDeviceContext->Release(); mD2DDeviceContext = nullptr;

		for (auto iter = mBitmapMap.begin(); iter != mBitmapMap.end(); ++iter)
		{
			ID2D1Bitmap* bitmap = iter->second;
			int count = bitmap->Release();
		}
		for (auto iter = mAnimationAssetMap.begin(); iter != mAnimationAssetMap.end(); ++iter)
		{
			AnimationAsset* animationAssert = iter->second;
			delete animationAssert;
		}

		mBitmapMap.clear();
		mAnimationAssetMap.clear();
	}

	void RenderManager::RegisterRenderable(IRenderable* renderable)
	{
		assert(renderable != nullptr);

		GameObject* gameObject = renderable->GetGameObject();
		mRenderable[static_cast<unsigned int>(gameObject->GetObjectType())].insert({ renderable->GetId(), renderable });
	}

	void RenderManager::UnregisterRenderable(IRenderable* renderable)
	{
		GameObject* gameObject = renderable->GetGameObject();
		unsigned int index = static_cast<unsigned int>(gameObject->GetObjectType());

		mRenderable[index].erase(renderable->GetId());
	}

	void RenderManager::BeginDraw()
	{
		mRenderTarget->BeginDraw();
		mRenderTarget->Clear({ 255,255,255 });
	}

	void RenderManager::Render(CameraManager* cameraManager)
	{
		assert(cameraManager != nullptr);

		AABB cameraAABB = { {0, 0 }, cameraManager->GetScrennSize() };
		D2D1::Matrix3x2F matrix = cameraManager->GetCombineMatrix();

		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			for (auto pair : mRenderable[i])
			{
				if (pair.second->IsOutsideBoundingBox(matrix, cameraAABB))
				{
					continue;
				}

				pair.second->Render(matrix);
			}
		}
	}

	void RenderManager::Clear(D2D1::Matrix3x2F matrix, D2D1_COLOR_F color)
	{
		mRenderTarget->SetTransform(matrix);
		mRenderTarget->Clear(color);
		mRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void RenderManager::EndDraw()
	{
		mRenderTarget->EndDraw();
	}

	void RenderManager::SetTransform(const D2D1::Matrix3x2F& trasform)
	{
		mRenderTarget->SetTransform(trasform);
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
			DWRITE_FONT_STRETCH_NORMAL, size, L"ko-KR", &mTextFormat);

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

		HRESULT hr = mRenderTarget->CreateSolidColorBrush(color, &mBrush);
		assert(SUCCEEDED(hr));

		return mBrush->GetColor();
	}

	void RenderManager::SetStrokeWidth(float strokeWidth)
	{
		mStrokeWidth = strokeWidth;
	}

	void RenderManager::DrawPoint(float x, float y)
	{
		mRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), mStrokeWidth, mStrokeWidth), mBrush);
	}

	void RenderManager::DrawPoint(const D2D1_POINT_2F& point)
	{
		DrawPoint(point.x, point.y);
	}

	void RenderManager::DrawLine(float x1, float y1, float x2, float y2)
	{
		mRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), mBrush, mStrokeWidth);
	}

	void RenderManager::DrawLine(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end)
	{
		DrawLine(start.x, start.y, end.x, end.y);
	}

	void RenderManager::DrawCircle(float x, float y, float radiusX, float radiusY)
	{
		mRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY), mBrush, mStrokeWidth);
	}

	void RenderManager::DrawCircle(const Vector2& offset, const Vector2& size)
	{
		DrawCircle(offset.GetX(), offset.GetY(), size.GetX() * 0.5f, size.GetY() * 0.5f);
	}

	void RenderManager::DrawCircle(const D2D1_ELLIPSE& ellipse)
	{
		DrawCircle(ellipse.point.x, ellipse.point.y, ellipse.radiusX, ellipse.radiusY);
	}

	void RenderManager::FillCircle(float x, float y, float radiusX, float radiusY)
	{
		mRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY), mBrush);
	}

	void RenderManager::FillCircle(const Vector2& offset, const Vector2& size)
	{
		FillCircle(offset.GetX(), offset.GetY(), size.GetX() * 0.5f, size.GetY() * 0.5f);
	}

	void RenderManager::FillCircle(const D2D1_ELLIPSE& ellipse)
	{
		FillCircle(ellipse.point.x, ellipse.point.y, ellipse.radiusX, ellipse.radiusY);
	}

	void RenderManager::DrawRectangle(float left, float top, float right, float bottom)
	{
		mRenderTarget->DrawRectangle(D2D1::Rect(left, top, right, bottom), mBrush, mStrokeWidth);
	}

	void RenderManager::DrawRectangle(const Vector2& offset, const Vector2& size)
	{
		DrawRectangle(MathHelper::CreateRectangle(offset, size));
	}

	void RenderManager::DrawRectangle(const D2D1_RECT_F& rectangle)
	{
		DrawRectangle(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);
	}

	void RenderManager::FillRectangle(float left, float top, float right, float bottom)
	{
		mRenderTarget->FillRectangle(D2D1::Rect(left, top, right, bottom), mBrush);
	}

	void RenderManager::FillRectangle(const Vector2& offset, const Vector2& size)
	{
		FillRectangle(MathHelper::CreateRectangle(offset, size));
	}

	void RenderManager::FillRectangle(const D2D1_RECT_F& rectangle)
	{
		FillRectangle(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);
	}

	void RenderManager::DrawPolygon(const std::vector<D2D1_POINT_2F>& points)
	{
		if (points.size() < 3)
		{
			return;
		}

		for (auto iter = points.begin(); iter != points.end();)
		{
			auto nextIter = iter + 1;

			DrawLine(*iter, *nextIter);

			iter = nextIter;
		}

		DrawLine(*(points.end() - 1), *(points.begin()));
	}

	void RenderManager::DrawGrid(float x, float y, float width, float height, float interval)
	{
		const float X_END = x + width;
		const float Y_END = y + height;

		for (float i = x; i < X_END; i += interval)
		{
			DrawLine(i, y, i, Y_END);
		}

		for (float i = y; i < Y_END; i += interval)
		{
			DrawLine(x, i, X_END, i);
		}
	}

	void RenderManager::DrawBitMap(float left, float top, float right, float bottom, float uvLeft, float uvTop, float uvRight, float uvBottom, ID2D1Bitmap* bitmap)
	{
		assert(bitmap != nullptr);

		DrawBitMap({ left, top, right, bottom }, { uvLeft, uvTop, uvRight, uvBottom }, bitmap);
	}

	void RenderManager::DrawBitMap(const Vector2& offset, const Vector2& size, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap)
	{
		assert(bitmap != nullptr);

		DrawBitMap(MathHelper::CreateRectangle(offset, size), uvRectangle, bitmap);
	}

	void RenderManager::DrawBitMap(const D2D1_RECT_F& rectangle, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap)
	{
		assert(bitmap != nullptr);

		mRenderTarget->DrawBitmap(bitmap
			, rectangle
			, 1.0f
			, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
			, uvRectangle);
	}

	void RenderManager::WriteText(const wchar_t* text, float left, float top, float right, float bottom)
	{
		assert(text != nullptr);

		mRenderTarget->DrawText(text, wcslen(text), mTextFormat, { left, top, right, bottom }, mBrush);

	}

	void RenderManager::WriteText(const wchar_t* text, const D2D1_RECT_F& rectangle)
	{
		assert(text != nullptr);

		WriteText(text, rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);
	}

	HRESULT RenderManager::CreateD2DBitmapFromFile(const WCHAR* filePath)
	{
		auto iter = mBitmapMap.find(filePath);

		if (iter != mBitmapMap.end())
		{
			return S_FALSE;
		}

		HRESULT hr = S_OK;
		IWICBitmapDecoder* decoder = nullptr;
		IWICBitmapFrameDecode* frameDecode = nullptr;
		IWICFormatConverter* convertedBitmap = nullptr;
		ID2D1Bitmap* bitmap = nullptr;

		// 디코더 생성
		hr = mWICFactory->CreateDecoderFromFilename(filePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
		if (FAILED(hr)) { goto END; }

		// 프레임 디코더를 통해 0번쨰 프레임 얻기 (압축파일을 메모리의 비트맵으로 사용하기 위해)
		hr = decoder->GetFrame(0, &frameDecode);
		if (FAILED(hr)) { goto END; }

		// 32비트맵 픽셀 포멧으로 변경하기 위해 컨버터 생성
		hr = mWICFactory->CreateFormatConverter(&convertedBitmap);
		if (FAILED(hr)) { goto END; }

		// 32비트맵 픽셀 포멧으로 변경
		hr = convertedBitmap->Initialize(frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
		if (FAILED(hr)) { goto END; }

		// 렌더타겟 체크
		hr = createDeviceResources(mHwnd);
		if (FAILED(hr)) { goto END; }

		hr = mRenderTarget->CreateBitmapFromWicBitmap(convertedBitmap, NULL, &bitmap);
		if (FAILED(hr)) { goto END; }
		mBitmapMap.emplace(filePath, bitmap);

	END:
		if (decoder != nullptr)
		{
			decoder->Release();
		}
		if (frameDecode != nullptr)
		{
			frameDecode->Release();
		}
		if (convertedBitmap != nullptr)
		{
			convertedBitmap->Release();
		}
		if (FAILED(hr))
		{
			_com_error err(hr);
			MessageBox(mHwnd, filePath, L"비트맵 로드 에러", MB_OK);
		}

		return hr;
	}

	HRESULT RenderManager::CreateD2DBitmapFromFile(const WCHAR* key, const WCHAR* filePath)
	{
		auto iter = mBitmapMap.find(key);

		if (iter != mBitmapMap.end())
		{
			return S_FALSE;
		}

		HRESULT hr = S_OK;
		IWICBitmapDecoder* decoder = nullptr;
		IWICBitmapFrameDecode* frameDecode = nullptr;
		IWICFormatConverter* convertedBitmap = nullptr;
		ID2D1Bitmap* bitmap = nullptr;

		// 디코더 생성
		hr = mWICFactory->CreateDecoderFromFilename(filePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
		if (FAILED(hr)) { goto END; }

		// 프레임 디코더를 통해 0번쨰 프레임 얻기 (압축파일을 메모리의 비트맵으로 사용하기 위해)
		hr = decoder->GetFrame(0, &frameDecode);
		if (FAILED(hr)) { goto END; }

		// 32비트맵 픽셀 포멧으로 변경하기 위해 컨버터 생성
		hr = mWICFactory->CreateFormatConverter(&convertedBitmap);
		if (FAILED(hr)) { goto END; }

		// 32비트맵 픽셀 포멧으로 변경
		hr = convertedBitmap->Initialize(frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
		if (FAILED(hr)) { goto END; }

		// 렌더타겟 체크
		hr = createDeviceResources(mHwnd);
		if (FAILED(hr)) { goto END; }

		hr = mRenderTarget->CreateBitmapFromWicBitmap(convertedBitmap, NULL, &bitmap);
		if (FAILED(hr)) { goto END; }
		mBitmapMap.emplace(key, bitmap);

	END:
		if (decoder != nullptr)
		{
			decoder->Release();
		}
		if (frameDecode != nullptr)
		{
			frameDecode->Release();
		}
		if (convertedBitmap != nullptr)
		{
			convertedBitmap->Release();
		}
		if (FAILED(hr))
		{
			_com_error err(hr);
			MessageBox(mHwnd, filePath, L"비트맵 로드 에러", MB_OK);
		}

		return hr;
	}

	HRESULT RenderManager::CreateAnimationAsset(const WCHAR* imagePath, const std::vector<std::vector<FrameInformation>>& frameInfo)
	{
		auto iter = mAnimationAssetMap.find(imagePath);

		if (iter != mAnimationAssetMap.end())
		{
			return S_FALSE;
		}

		ID2D1Bitmap* bitmap = GetBitmapOrNull(imagePath);

		if (bitmap == nullptr)
		{
			if (FAILED(CreateD2DBitmapFromFile(imagePath)))
			{
				return E_FAIL;
			}
		}

		bitmap = GetBitmapOrNull(imagePath);
		assert(bitmap != nullptr);
		mAnimationAssetMap.emplace(imagePath, new AnimationAsset(bitmap, imagePath, frameInfo));

		return S_OK;
	}

	HRESULT RenderManager::CreateAnimationAsset(const WCHAR* key, const WCHAR* imagePath, const std::vector<std::vector<FrameInformation>>& frameInfo)
	{
		auto iter = mAnimationAssetMap.find(key);

		if (iter != mAnimationAssetMap.end())
		{
			return S_FALSE;
		}

		ID2D1Bitmap* bitmap = GetBitmapOrNull(key);

		if (bitmap == nullptr)
		{
			if (FAILED(CreateD2DBitmapFromFile(key, imagePath)))
			{
				return E_FAIL;
			}
		}

		bitmap = GetBitmapOrNull(key);
		assert(bitmap != nullptr);
		mAnimationAssetMap.emplace(key, new AnimationAsset(bitmap, key, frameInfo));

		return S_OK;
	}

	HRESULT RenderManager::createDeviceResources(HWND hWnd)
	{
		HRESULT hr = S_OK;

		if (!mRenderTarget)
		{
			RECT rc;
			hr = GetClientRect(hWnd, &rc) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				// Create a D2D render target properties
				D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

				// Set the DPI to be the default system DPI to allow direct mapping
				// between image pixels and desktop pixels in different system DPI settings
				renderTargetProperties.dpiX = DEFAULT_DPI;
				renderTargetProperties.dpiY = DEFAULT_DPI;

				// Create a D2D render target
				D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

				hr = mFactory->CreateHwndRenderTarget(
					renderTargetProperties,
					D2D1::HwndRenderTargetProperties(hWnd, size),
					&mRenderTarget
				);

			}
		}

		return hr;
	}
}