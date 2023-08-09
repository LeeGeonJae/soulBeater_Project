#include "RenderManger.h"

#include "GameObject.h"
#include "IRenderable.h"
#include "eFrameworkID.h"
#include "AnimationAsset.h"
#include "WinApp.h"
#include "MathHelper.h"
#include "Vector2.h"
#include "CameraManager.h"
#include "AABB.h"

#include <d2d1_1helper.h>
#include <d3d11.h>
#include <cassert>
#include <d2d1helper.h>
#include <comdef.h>

#define DEFAULT_DPI 96.f

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace d2dFramework
{
	RenderManager::RenderManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::RenderManager))
		, mFactory(nullptr)
		, mD2DDevice(nullptr)
		, mD2DDeviceContext(nullptr)
		, mD2DRenderTarget(nullptr)
		, mD3DDevice(nullptr)
		, mD3DDeviceContext(nullptr)
		, mSwapChain(nullptr)
		, mD3DRenderTarget(nullptr)
		, mWICFactory(nullptr)
		, mTextFormat(nullptr)
		, mBeforeColor{ 0.f, }
	{
	}

	void RenderManager::Init()
	{
		HRESULT hr;

		hr = CoInitialize(NULL);
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

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&mWriteFactory));
		assert(SUCCEEDED(hr));

		hr = createDeviceResources(mHwnd);
		assert(SUCCEEDED(hr));

		SetFontSize(INIT_FONT_SIZE);
		SetColor({ 0.f,0.f,0.f,1.f });
		SetStrokeWidth(INIT_STROKE_SIZE);

		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			mRenderable[i].reserve(RESERVE_SIZE);
		}

	}

	void RenderManager::Release()
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(eObjectType::Size); ++i)
		{
			mRenderable[i].clear();
		}

		mTextFormat->Release(); mTextFormat = nullptr;
		mFactory->Release(); mFactory = nullptr;
		mWICFactory->Release(); mWICFactory = nullptr;
		mWriteFactory->Release(); mWriteFactory = nullptr;

		for (auto iter = mBitmapMap.begin(); iter != mBitmapMap.end(); ++iter)
		{
			ID2D1Bitmap1* bitmap = iter->second;
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
		mD2DDeviceContext->BeginDraw();
		mD2DDeviceContext->Clear({ 255,255,255 });
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

		Present();
	}

	void RenderManager::Clear(D2D1::Matrix3x2F matrix, D2D1_COLOR_F color)
	{
		mD2DDeviceContext->SetTransform(matrix);
		mD2DDeviceContext->Clear(color);
		mD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void RenderManager::EndDraw()
	{
		mD2DDeviceContext->EndDraw();
		//float color[4] = { 1,1,1,1.f };
		//mD3DDeviceContext->ClearRenderTargetView(mD3DRenderTarget, color);
		mD3DDeviceContext->OMSetRenderTargets(1, &mD3DRenderTarget, nullptr);

		IDXGISurface* surfcae;
		mD2DRenderTarget->GetSurface(&surfcae);

		ID3D11Resource* d3dResource;
		mD3DRenderTarget->GetResource(&d3dResource);

		// D2D 렌더타겟 서피스에서 ID3D11Texture2D 가져오기
		ID3D11Texture2D* d2dResource;
		HRESULT hr = surfcae->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&d2dResource));

		mD3DDeviceContext->CopyResource(d3dResource, d2dResource);
		surfcae->Release();
	}

	void RenderManager::Present()
	{
		mSwapChain->Present(0, 0);
	}

	void RenderManager::SetTransform(const D2D1::Matrix3x2F& trasform)
	{
		mD2DDeviceContext->SetTransform(trasform);
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

		HRESULT hr = mD2DDeviceContext->CreateSolidColorBrush(color, &mBrush);
		assert(SUCCEEDED(hr));

		return mBrush->GetColor();
	}

	void RenderManager::SetStrokeWidth(float strokeWidth)
	{
		mStrokeWidth = strokeWidth;
	}

	void RenderManager::DrawPoint(float x, float y)
	{
		mD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), mStrokeWidth, mStrokeWidth), mBrush);
	}

	void RenderManager::DrawPoint(const D2D1_POINT_2F& point)
	{
		DrawPoint(point.x, point.y);
	}

	void RenderManager::DrawLine(float x1, float y1, float x2, float y2)
	{
		mD2DDeviceContext->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), mBrush, mStrokeWidth);
	}

	void RenderManager::DrawLine(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end)
	{
		DrawLine(start.x, start.y, end.x, end.y);
	}

	void RenderManager::DrawCircle(float x, float y, float radiusX, float radiusY)
	{
		mD2DDeviceContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY), mBrush, mStrokeWidth);
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
		mD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY), mBrush);
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
		mD2DDeviceContext->DrawRectangle(D2D1::Rect(left, top, right, bottom), mBrush, mStrokeWidth);
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
		mD2DDeviceContext->FillRectangle(D2D1::Rect(left, top, right, bottom), mBrush);
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

	void RenderManager::DrawBitMap(float left, float top, float right, float bottom, float uvLeft, float uvTop, float uvRight, float uvBottom, ID2D1Bitmap1* bitmap)
	{
		assert(bitmap != nullptr);

		DrawBitMap({ left, top, right, bottom }, { uvLeft, uvTop, uvRight, uvBottom }, bitmap);
	}

	void RenderManager::DrawBitMap(const Vector2& offset, const Vector2& size, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap1* bitmap)
	{
		assert(bitmap != nullptr);

		DrawBitMap(MathHelper::CreateRectangle(offset, size), uvRectangle, bitmap);
	}

	void RenderManager::DrawBitMap(const D2D1_RECT_F& rectangle, const D2D1_RECT_F& uvRectangle, ID2D1Bitmap1* bitmap)
	{
		assert(bitmap != nullptr);

		mD2DDeviceContext->DrawBitmap(bitmap
			, rectangle
			, 1.0f
			, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
			, uvRectangle);
	}

	void RenderManager::WriteText(const wchar_t* text, float left, float top, float right, float bottom)
	{
		assert(text != nullptr);

		mD2DDeviceContext->DrawText(text, wcslen(text), mTextFormat, { left, top, right, bottom }, mBrush);

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
		ID2D1Bitmap1* bitmap = nullptr;

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

		hr = mD2DDeviceContext->CreateBitmapFromWicBitmap(convertedBitmap, NULL, &bitmap);
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
		ID2D1Bitmap1* bitmap = nullptr;

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

		hr = mD2DDeviceContext->CreateBitmapFromWicBitmap(convertedBitmap, NULL, &bitmap);
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

	HRESULT RenderManager::CreateAnimationAsset(const WCHAR* imagePath, const std::vector<std::vector<FrameInfomation>>& frameInfo)
	{
		auto iter = mAnimationAssetMap.find(imagePath);

		if (iter != mAnimationAssetMap.end())
		{
			return S_FALSE;
		}

		ID2D1Bitmap1* bitmap = GetBitmapOrNull(imagePath);

		if (bitmap == nullptr)
		{
			if (FAILED(CreateD2DBitmapFromFile(imagePath)))
			{
				return E_FAIL;
			}
		}

		bitmap = GetBitmapOrNull(imagePath);
		assert(bitmap != nullptr);
		mAnimationAssetMap.emplace(imagePath, new AnimationAsset(bitmap, frameInfo));

		return S_OK;
	}

	HRESULT RenderManager::CreateAnimationAsset(const WCHAR* key, const WCHAR* imagePath, const std::vector<std::vector<FrameInfomation>>& frameInfo)
	{
		auto iter = mAnimationAssetMap.find(key);

		if (iter != mAnimationAssetMap.end())
		{
			return S_FALSE;
		}

		ID2D1Bitmap1* bitmap = GetBitmapOrNull(key);

		if (bitmap == nullptr)
		{
			if (FAILED(CreateD2DBitmapFromFile(key, imagePath)))
			{
				return E_FAIL;
			}
		}

		bitmap = GetBitmapOrNull(key);
		assert(bitmap != nullptr);
		mAnimationAssetMap.emplace(key, new AnimationAsset(bitmap, frameInfo));

		return S_OK;
	}

	HRESULT RenderManager::createDeviceResources(HWND hWnd)
	{
		HRESULT hr = S_OK;

		if (mD2DDevice == nullptr)
		{
			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			D3D_FEATURE_LEVEL featureLevel;
			D3D11CreateDevice(
				nullptr,                    // specify null to use the default adapter
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				creationFlags,              // optionally set debug and Direct2D compatibility flags
				featureLevels,              // list of feature levels this app can support
				ARRAYSIZE(featureLevels),   // number of possible feature levels
				D3D11_SDK_VERSION,
				&mD3DDevice,              // returns the Direct3D device created
				&featureLevel,            // returns feature level of device created
				&mD3DDeviceContext		// returns the device immediate context
			);

			assert(mD3DDevice);

			// Obtain the underlying DXGI device of the Direct3D11 device.
			IDXGIDevice* dxgiDevice;
			mD3DDevice->QueryInterface(&dxgiDevice);	/// ComPtr.As()
			assert(dxgiDevice);

			// Obtain the Direct2D device for 2-D rendering.
			mFactory->CreateDevice(dxgiDevice, &mD2DDevice);

			// Get Direct2D device's corresponding device context object.
			hr = mD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&mD2DDeviceContext);

			// Allocate a descriptor.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = 1920;                           // 0 : use automatic sizing
			swapChainDesc.Height = 1080;
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;

			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // all apps must use this SwapEffect
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 주사율 고정 해제
			swapChainDesc.Flags = 0;

			// Identify the physical adapter (GPU or card) this device is runs on.
			IDXGIAdapter* dxgiAdapter;
			dxgiDevice->GetAdapter(&dxgiAdapter);

			// Get the factory object that created the DXGI device.
			IDXGIFactory2* dxgiFactory;
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

			// Get the final swap chain for this window from the DXGI factory.
			DXGI_RATIONAL rate{};
			rate.Denominator = 60;
			rate.Numerator = 1;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainDescFull{};
			swapChainDescFull.RefreshRate = rate;
			//swapChainDescFull.Scaling = DXGI_MODE_SCALING_STRETCHED; // 화면 해상도에에 맞게 화면을 늘린는 설정
			swapChainDescFull.Scaling = DXGI_MODE_SCALING_CENTERED; // 화면 해상도에에 맞게 화면을 늘린는 설정

			swapChainDescFull.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST;
			swapChainDescFull.Windowed = true; // 창모드 형식
			//swapChainDescFull.Windowed = false; // 전체화면 형식

			dxgiFactory->CreateSwapChainForHwnd(mD3DDevice,
				mHwnd,
				&swapChainDesc,
				&swapChainDescFull, // 전체화면 스왑체인 설정
				//NULL, // 창모드 화면 설정 
				nullptr,
				&mSwapChain);

			// Ensure that DXGI doesn't queue more than one frame at a time.
			//dxgiDevice->SetMaximumFrameLatency(1);

			// Get the backbuffer for this window which is be the final 3D render target.
			ID3D11Texture2D* _backBuffer;
			mSwapChain->GetBuffer(0, IID_PPV_ARGS(&_backBuffer));

			//	Now we set up the Direct2D render target bitmap linked to the swapchain. 
			// Whenever we render to this bitmap, it is directly rendered to the 
			// swap chain associated with the window.
			float dpi = 96;
			D2D1_BITMAP_PROPERTIES1 _bitmapProperties =
				D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
					dpi,
					dpi
				);

			// Direct2D needs the dxgi version of the backbuffer surface pointer.
			IDXGISurface* backBuffer;
			mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
			assert(backBuffer);

			ID3D11Texture2D* d3dResource;
			backBuffer->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&d3dResource));

			// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
			HRESULT hr = mD2DDeviceContext->CreateBitmapFromDxgiSurface(
				backBuffer,
				&_bitmapProperties,
				&mD2DRenderTarget
			);
			mD3DDevice->CreateRenderTargetView(d3dResource, nullptr, &mD3DRenderTarget);

			backBuffer->Release();


			// Now we can set the Direct2D render target.
			mD2DDeviceContext->SetTarget(mD2DRenderTarget);
			mD3DDeviceContext->OMSetRenderTargets(1, &mD3DRenderTarget, nullptr);
		}

		return hr;
	}
}