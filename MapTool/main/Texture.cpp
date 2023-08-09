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
	// �ؽ�ó ������ ���� ���÷� ����(Sampler State) ����
	ZeroMemory(&m_sampDesc, sizeof(m_sampDesc));
	m_sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		// �ؽ�ó ���͸� ��� ���� (�ּ�/�ִ�/�߰��� ���͸�)
	m_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// U �� �ؽ�ó �ּ� ��� ����
	m_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// V �� �ؽ�ó �ּ� ��� ����
	m_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// W �� �ؽ�ó �ּ� ��� ����
	m_sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;		// ���ð� ���� �� ����� �Լ� ����
	m_sampDesc.MinLOD = 0;									// �ؽ�ó�� �ּ� LOD (���� ���� ������) ����
	m_sampDesc.MaxLOD = D3D11_FLOAT32_MAX;					// �ؽ�ó�� �ִ� LOD ����

	// ���÷� ���� ����
	_device->CreateSamplerState(&m_sampDesc, &m_colorSampler);

	// �ؽ�ó ������ ���� ����
	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
	m_textureDesc.MipLevels = m_textureDesc.ArraySize = 1;		// MIP �� ������ �迭 ũ�� ����
	m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;		// �ؽ�ó ���� ���� (32��Ʈ RGBA �ε� �Ҽ���)
	m_textureDesc.SampleDesc.Count = 1;							// ��Ƽ ���ø� ����
	m_textureDesc.Usage = D3D11_USAGE_DYNAMIC;					// �ؽ�ó�� ��� ��� ���� (�������� ������Ʈ ����)
	m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;		// �ؽ�ó�� ���̴� ���ҽ��� ���ε�
	m_textureDesc.MiscFlags = 0;								// ��Ÿ Ư�� �ɼ� ����
	m_textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPU ������ ���� ���� (�ؽ�ó ���� ����)
	m_textureDesc.Width = CANVAS_WIDTH;							// �ؽ�ó�� ���� ũ�� ����
	m_textureDesc.Height = CANVAS_HEIGHT;						// �ؽ�ó�� ���� ũ�� ����

	// �ؽ�ó ����
	_device->CreateTexture2D(&m_textureDesc, nullptr, &m_canvasTexture);


	 //�ؽ�ó�� ���������� �����Ǿ��ٸ�, ���̴� ���ҽ� ��� ���� Ÿ�� �並 �����Ѵ�.
	if (m_canvasTexture)
	{
		 //�ؽ�ó�� ���� ���̴� ���ҽ� �� ����
		_device->CreateShaderResourceView(m_canvasTexture, nullptr, &m_canvasTextureView);

		 //�ؽ�ó�� ���� ���� Ÿ�� �� ����
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = m_textureDesc.Format;					// ���� Ÿ�� ���� ���� ���� (�ؽ�ó ���İ� �����ϰ�)
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó�� ����
		renderTargetViewDesc.Texture2D.MipSlice = 0;						// MIP �� ���� ���� (0�� �⺻ ����)

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
