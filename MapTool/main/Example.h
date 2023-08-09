#pragma once

#include "defalut.h"

class Mesh;
class Texture;

class Example
{
public:
	Example(HWND window)
	{
		Initialize(window);
	}

	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader
	void InitShaders();
	void Initialize(HWND window);

	void Update(Mesh* _mesh);
	void RenderBegin();
	void MeshRender(Mesh* _mesh);
	void Clean();

	inline ID3D11DeviceContext* GetDeviceContext() const;

public:
	// 이름 짓기 규칙
	// https://github.com/Microsoft/DirectXTK/wiki/Implementation#naming-conventions
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	float backgroundColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
};

ID3D11DeviceContext* Example::GetDeviceContext() const
{
	return deviceContext;
}