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
	RenderManager* RenderManager::mInstance = nullptr;

	RenderManager* RenderManager::GetInstance()
	{
		assert(mInstance != nullptr);
		return mInstance;
	}

	RenderManager::RenderManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::RenderManager))
		, mFactory(nullptr)
		, mBitmapRenderTarget(nullptr)
		, mWICFactory(nullptr)
		, mTextFormat(nullptr)
		, mD2DDeviceContext(nullptr)
		, mBeforeColor{ 0, }
		, mHwnd(NULL)
		, mGradientStops(nullptr)
		, mRenderTarget(nullptr)
		, mWriteFactory(nullptr)
		, mBrush(nullptr)
		, mStrokeWidth()
		, mRenderable{}
		, mRadialGradientBrush()
		, mLinearGradientBrush()
		, mLayer()
	{
	}

	void RenderManager::init(HWND hwnd)
	{
		HRESULT hr;

		mHwnd = hwnd;

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

		hr = createDeviceResources(mHwnd);

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&mWriteFactory));
		assert(SUCCEEDED(hr));

		SetFontSize(INIT_FONT_SIZE);
		SetStrokeWidth(INIT_STROKE_SIZE);

		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			mRenderable[i].reserve(RESERVE_SIZE);
		}
	}

	void RenderManager::release()
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			mRenderable[i].clear();
		}

		if (mTextFormat != nullptr) { mTextFormat->Release(); mTextFormat = nullptr; }
		if (mFactory != nullptr) { mFactory->Release(); mFactory = nullptr; }
		if (mBitmapRenderTarget != nullptr) { mBitmapRenderTarget->Release(); mBitmapRenderTarget = nullptr; }
		if (mWICFactory != nullptr) { mWICFactory->Release(); mWICFactory = nullptr; }
		if (mWriteFactory != nullptr) { mWriteFactory->Release(); mWriteFactory = nullptr; }
		if (mD2DDeviceContext != nullptr) { mD2DDeviceContext->Release(); mD2DDeviceContext = nullptr; }

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

		mHwnd = NULL;
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

	void RenderManager::BitmapBeginDraw()
	{
		HRESULT hr = createDeviceResources(mHwnd);
		if (FAILED(hr))
		{
			return;
		}

		mBitmapRenderTarget->BeginDraw();
		mBitmapRenderTarget->Clear({ 1, 1,1, 1 });
	}

	void RenderManager::BitmapEndDraw()
	{
		HRESULT hr = mBitmapRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceResource();
			createDeviceResources(mHwnd);
		}
	}

	void RenderManager::BeginDraw()
	{
		HRESULT hr = createDeviceResources(mHwnd);
		if (FAILED(hr))
		{
			return;
		}

		mRenderTarget->BeginDraw();
		mRenderTarget->Clear({ 0,0,0,1 });
	}

	void RenderManager::EndDraw()
	{
		HRESULT  hr = mRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceResource();
			createDeviceResources(mHwnd);
		}
	}

	void RenderManager::Clear(D2D1::Matrix3x2F matrix, D2D1_COLOR_F color)
	{
		mBitmapRenderTarget->SetTransform(matrix);
		mBitmapRenderTarget->Clear(color);
		mBitmapRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void RenderManager::CopyBitmapRenderToHwndRender(D2D1::Matrix3x2F matrix)
	{
		ID2D1Bitmap* bitmap = NULL;
		mBitmapRenderTarget->GetBitmap(&bitmap);
		assert(bitmap != NULL);
		mRenderTarget->SetTransform(matrix);
		mRenderTarget->DrawBitmap(bitmap);
		mRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void RenderManager::DrawRadialGradiendBrush(const D2D1_ELLIPSE& ellipse)
	{
		//mBitmapRenderTarget->FillEllipse(ellipse, mLinearGradientBrush);
		//
		//D2D1_ELLIPSE e = { {400,300},200,200 };
		//
		//mBitmapRenderTarget->FillEllipse(e, mRadialGradientBrush);	
		//
		//mBitmapRenderTarget->FillRectangle(D2D1::RectF(0, 0, 100, 100), mBrush);
	}

	void RenderManager::CreateRadialGradientBrush()
	{
		D2D1_GRADIENT_STOP gradientStops[2];
		gradientStops[0] = D2D1::GradientStop(0.0f, D2D1::ColorF(D2D1::ColorF::White));
		//gradientStops[0].position = 0.0f;
		gradientStops[1] = D2D1::GradientStop(1.0f, D2D1::ColorF(D2D1::ColorF::Black));
		//gradientStops[1].position = 1.0f;

		HRESULT hr = mBitmapRenderTarget->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&mGradientStops
		);
		assert(SUCCEEDED(hr));

		assert(mGradientStops != nullptr);
		hr = mBitmapRenderTarget->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(0, 0),
				D2D1::Point2F(150, 150)),
			mGradientStops,
			&mLinearGradientBrush
		);
		assert(SUCCEEDED(hr));

		hr = mBitmapRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(400, 300), D2D1::Point2F(400, 300),
				200, 200),
			mGradientStops,
			&mRadialGradientBrush
		);
		assert(SUCCEEDED(hr));

	}

	void RenderManager::render(CameraManager* cameraManager)
	{
		assert(cameraManager != nullptr);

		// AABB cameraAABB = { {0, 0 }, cameraManager->GetScrennSize() };
		AABB cameraAABB = { {-1000, -1000 }, {2000, 2000 } };
		D2D1::Matrix3x2F matrix = cameraManager->GetCombineMatrix();

		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			for (auto pair : mRenderable[i])
			{
				//if (pair.second->IsOutsideBoundingBox(matrix, cameraAABB))
				//{
				//	continue;
				//}

				pair.second->Render(matrix);
			}
		}
	}


	void RenderManager::DrawPoint(float x, float y)
	{
		mBitmapRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), mStrokeWidth, mStrokeWidth), mBrush);
	}

	void RenderManager::DrawPoint(const D2D1_POINT_2F& point)
	{
		DrawPoint(point.x, point.y);
	}

	void RenderManager::DrawLine(float x1, float y1, float x2, float y2)
	{
		mBitmapRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), mBrush, mStrokeWidth);
	}

	void RenderManager::DrawLine(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end)
	{
		DrawLine(start.x, start.y, end.x, end.y);
	}

	void RenderManager::DrawCircle(float x, float y, float radiusX, float radiusY)
	{
		mBitmapRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY), mBrush, mStrokeWidth);
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
		mBitmapRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY), mBrush);
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
		mBitmapRenderTarget->DrawRectangle(D2D1::Rect(left, top, right, bottom), mBrush, mStrokeWidth);
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
		mBitmapRenderTarget->FillRectangle(D2D1::Rect(left, top, right, bottom), mBrush);
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

	void RenderManager::DrawBitMap(float left, float top, float right, float bottom, float uvLeft, float uvTop, float uvRight, float uvBottom, ID2D1Bitmap* bitmap, float alpha)
	{
		assert(bitmap != nullptr);

		DrawBitMap({ left, top, right, bottom }, { uvLeft, uvTop, uvRight, uvBottom }, bitmap, alpha);
	}

	void RenderManager::DrawBitMap(const Vector2& offset, const Vector2& size, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap, float alpha)
	{
		assert(bitmap != nullptr);

		DrawBitMap(MathHelper::CreateRectangle(offset, size), uvRectangle, bitmap, alpha);
	}

	void RenderManager::DrawBitMap(const D2D1_RECT_F& rectangle, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap* bitmap, float alpha)
	{
		assert(bitmap != nullptr);
		/// D2D1_BITMAP_INTERPOLATION_MODE_LINEAR 에서 수정 
		mBitmapRenderTarget->DrawBitmap(bitmap
			, rectangle
			, alpha
			, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
			, uvRectangle);
	}

	void RenderManager::WriteText(const wchar_t* text, float left, float top, float right, float bottom)
	{
		assert(text != nullptr);

		mBitmapRenderTarget->DrawText(text, (UINT)wcslen(text), mTextFormat, { left, top, right, bottom }, mBrush);
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

		hr = mBitmapRenderTarget->CreateBitmapFromWicBitmap(convertedBitmap, NULL, &bitmap);
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

		hr = mBitmapRenderTarget->CreateBitmapFromWicBitmap(convertedBitmap, NULL, &bitmap);
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

	void RenderManager::DiscardDeviceResource()
	{
		mRenderTarget->Release(); mRenderTarget = nullptr;
		mBitmapRenderTarget->Release(); mBitmapRenderTarget = nullptr;
		mBrush->Release(); mBrush = nullptr;

	}

	HRESULT RenderManager::createDeviceResources(HWND hWnd)
	{
		HRESULT hr = S_OK;

		if (!mBitmapRenderTarget)
		{
			RECT rc;
			hr = GetWindowRect(hWnd, &rc) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

				D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();
				renderTargetProperties.dpiX = DEFAULT_DPI;
				renderTargetProperties.dpiY = DEFAULT_DPI;

				hr = mFactory->CreateHwndRenderTarget(
					renderTargetProperties,
					D2D1::HwndRenderTargetProperties(hWnd, size),
					&mRenderTarget
				);
				assert(SUCCEEDED(hr));

				hr = mRenderTarget->CreateCompatibleRenderTarget(&mBitmapRenderTarget);
				assert(SUCCEEDED(hr));

				hr = mBitmapRenderTarget->QueryInterface(&mD2DDeviceContext);
				assert(SUCCEEDED(hr));

				SetColor({ 0.f,0.f,0.f,1.f });
			}
		}

		return hr;
	}
}