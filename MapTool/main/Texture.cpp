#include "Texture.h"


Texture::Texture()
{
}

Texture::~Texture()
{
	if (m_canvasTexture) { m_canvasTexture->Release(); m_canvasTexture = NULL; }
	if (m_canvasTextureView) { m_canvasTextureView->Release(); m_canvasTextureView = NULL; }
	if (m_canvasRenderTargetView) { m_canvasRenderTargetView->Release(); m_canvasRenderTargetView = NULL; }
	if (m_colorSampler) { m_colorSampler->Release(); m_colorSampler = NULL; }
}

void Texture::Initialize(ID3D11Device* _device, const wstring& path)
{
	CreateTexture(_device, path);
}

void Texture::CreateTexture(ID3D11Device* _device, const wstring& path)
{
	// 텍스처 생성을 위한 샘플러 상태(Sampler State) 설정
	ZeroMemory(&m_sampDesc, sizeof(m_sampDesc));
	m_sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		// 텍스처 필터링 방식 설정 (최소/최대/중간값 필터링)
	m_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// U 축 텍스처 주소 모드 설정
	m_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// V 축 텍스처 주소 모드 설정
	m_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// W 축 텍스처 주소 모드 설정
	m_sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;		// 샘플과 비교할 때 사용할 함수 설정
	m_sampDesc.MinLOD = 0;									// 텍스처의 최소 LOD (레벨 오브 디테일) 설정
	m_sampDesc.MaxLOD = D3D11_FLOAT32_MAX;					// 텍스처의 최대 LOD 설정

	// 샘플러 상태 생성
	_device->CreateSamplerState(&m_sampDesc, &m_colorSampler);

	// 텍스처 생성을 위한 설정
	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
	m_textureDesc.MipLevels = m_textureDesc.ArraySize = 1;		// MIP 맵 레벨과 배열 크기 설정
	m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;		// 텍스처 형식 설정 (32비트 RGBA 부동 소수점)
	m_textureDesc.SampleDesc.Count = 1;							// 멀티 샘플링 설정
	m_textureDesc.Usage = D3D11_USAGE_DYNAMIC;					// 텍스처의 사용 방식 설정 (동적으로 업데이트 가능)
	m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;		// 텍스처를 셰이더 리소스로 바인딩
	m_textureDesc.MiscFlags = 0;								// 기타 특정 옵션 없음
	m_textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPU 엑세스 권한 설정 (텍스처 쓰기 가능)
	m_textureDesc.Width = CANVAS_WIDTH;							// 텍스처의 가로 크기 설정
	m_textureDesc.Height = CANVAS_HEIGHT;						// 텍스처의 세로 크기 설정

	// 텍스처 생성
	_device->CreateTexture2D(&m_textureDesc, nullptr, &m_canvasTexture);


	 //텍스처가 정상적으로 생성되었다면, 셰이더 리소스 뷰와 렌더 타겟 뷰를 생성한다.
	if (m_canvasTexture)
	{
		 //텍스처에 대한 셰이더 리소스 뷰 생성
		_device->CreateShaderResourceView(m_canvasTexture, nullptr, &m_canvasTextureView);

		 //텍스처에 대한 렌더 타겟 뷰 생성
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = m_textureDesc.Format;					// 렌더 타겟 뷰의 형식 설정 (텍스처 형식과 동일하게)
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처로 설정
		renderTargetViewDesc.Texture2D.MipSlice = 0;						// MIP 맵 레벨 설정 (0은 기본 레벨)

		_device->CreateRenderTargetView(m_canvasTexture, &renderTargetViewDesc, &m_canvasRenderTargetView);
	}
	else
	{
		std::cout << "CreateRenderTargetView() error" << std::endl;
	}

	// Create Textrue File
	{
		HRESULT hr = DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &md, img);
		assert(SUCCEEDED(hr));

		hr = DirectX::CreateShaderResourceView(_device, img.GetImages(), img.GetImageCount(), md, &m_canvasTextureView);
		assert(SUCCEEDED(hr));
	}
}
