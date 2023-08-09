#pragma once
#include "defalut.h"

class Texture
{
public:
	Texture();
	~Texture();

public:
	void Initialize(ID3D11Device* _device, const wstring& path);

public:
	ID3D11Texture2D* GetTexture() { return m_canvasTexture; }
	ID3D11SamplerState* const* GetSamplerState() { return &m_colorSampler; }
	ID3D11ShaderResourceView* const* GetShaderResourceView() {return &m_canvasTextureView;}
	ID3D11RenderTargetView* GetRenderTargetView() {return m_canvasRenderTargetView;}

private:
	void CreateTexture(ID3D11Device* _device, const wstring& path);

public:
	ID3D11Texture2D* m_canvasTexture = nullptr;
	ID3D11SamplerState* m_colorSampler = nullptr;
	ID3D11ShaderResourceView* m_canvasTextureView = nullptr;
	ID3D11RenderTargetView* m_canvasRenderTargetView = nullptr;

	D3D11_SAMPLER_DESC m_sampDesc;
	D3D11_TEXTURE2D_DESC m_textureDesc;
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
};

