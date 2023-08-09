#include "Example.h"
#include "Texture.h"
#include "Mesh.h"

void Example::InitShaders()
{
	// ���� ���̴� ������
	ID3DBlob* vertexBlob = nullptr;
	ID3DBlob* pixelBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// "VS.hlsl" ������ �������Ͽ� ���� ���̴��� ���� (vs_5_0 �������� ���)
	if (FAILED(D3DCompileFromFile(L"../Bin/Binaries/VS.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
	{
		// ������ ������ �߻��� ��� ���� �޽��� ���
		if (errorBlob) {
			std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	// "PS.hlsl" ������ �������Ͽ� �ȼ� ���̴��� ���� (ps_5_0 �������� ���)
	if (FAILED(D3DCompileFromFile(L"../Bin/Binaries/PS.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
	{
		// ������ ������ �߻��� ��� ���� �޽��� ���
		if (errorBlob) {
			std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	// �����ϵ� ���� ���̴��� �ȼ� ���̴��κ��� DirectX 11���� ��� ������ ���� ���̴��� �ȼ� ���̴��� ����
	device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader);
	device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);

	// �Է� ���̾ƿ� ����
	// ���� ���̴� �Է� ���̾ƿ��� �����ϴ� D3D11_INPUT_ELEMENT_DESC �迭�� ����
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// �Է� ���̾ƿ� ���� �� ���� ���̴��� ����
	device->CreateInputLayout(ied, 3, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &layout);

	// �Է� ���̾ƿ��� ����ϵ��� ����
	deviceContext->IASetInputLayout(layout);
}

void Example::Initialize(HWND window)
{
	// DXGI_SWAP_CHAIN_DESC ����ü ���� �� �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = WIDTH;                          // �� ������ ���� ũ�� ����
	swapChainDesc.BufferDesc.Height = HEIGHT;                        // �� ������ ���� ũ�� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // 32��Ʈ ���� ���� ���
	swapChainDesc.BufferCount = 2;                                  // �� ���� ���� ���� (���� ���۸�)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;           // �ֻ��� ���� (60 FPS)
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;          // �ֻ��� �и� (60 FPS)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // ���� ü���� ��� ��� ���� (���� Ÿ������ ���)
	swapChainDesc.OutputWindow = window;                            // ������ ��� ������ �ڵ� ����
	swapChainDesc.SampleDesc.Count = 1;                             // ��Ƽ ���ø� ���� (��Ȱ��ȭ)
	swapChainDesc.Windowed = TRUE;                                 // ������ ���� ����
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // ��ü ȭ�� ��ȯ ���
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;           // ���� ü�� ȿ�� ���� (DISCARD ���)

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// Ư�� ��� ���ؿ��� D3D ��ġ�� ���� ü�� ����
	const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevelArray,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		NULL,
		&deviceContext)))
	{
		std::cout << "D3D11CreateDeviceAndSwapChain() error" << std::endl;
	}

	// ���� Ÿ�� ����
	ID3D11Texture2D* pBackBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
	{
		// �� ���� �ؽ�ó�� �̿��Ͽ� ���� Ÿ�� �� ����
		device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
		pBackBuffer->Release();
	}
	else
	{
		std::cout << "CreateRenderTargetView() error" << std::endl;
		exit(-1);
	}

	// ����Ʈ ����
	// ȭ�� ����Ʈ�� �����ϰ� �ʱ�ȭ
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(WIDTH);   // ����Ʈ�� ���� ũ�� ����
	viewport.Height = float(HEIGHT); // ����Ʈ�� ���� ũ�� ����
	viewport.MinDepth = 0.0f;        // ����Ʈ�� �ּ� ���� �� ����
	viewport.MaxDepth = 1.0f;        // ����Ʈ�� �ִ� ���� �� ���� (�߿�: ���� ���۸��� �߿��� ��)
	deviceContext->RSSetViewports(1, &viewport);

	// ���̴� �ʱ�ȭ
	InitShaders(); // ����� ���� �Լ� (���̴� �ʱ�ȭ ���� �۾�)
}

void Example::Update(Mesh* _mesh)
{
	//// ���� ������ ���� �ȼ� ������ �迭 ���� �� �ʱ�ȭ
	//std::vector<Vec4> pixels(CANVAS_WIDTH * CANVAS_HEIGHT, Vec4{ 0.8f, 0.8f, 0.8f, 1.0f });

	//// Ư�� �ȼ� ��ġ�� ���� �� �Ҵ� (��: (0, 0) ��ġ�� ������, (1, 0) ��ġ�� �����, (2, 0) ��ġ�� ���)
	//pixels[0 + CANVAS_WIDTH * 0] = Vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
	//pixels[1 + CANVAS_WIDTH * 0] = Vec4{ 1.0f, 1.0f, 0.0f, 1.0f };
	//pixels[2 + CANVAS_WIDTH * 0] = Vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

	// �ؽ�ó ���� ������Ʈ�� ���� �ؽ�ó ����
	D3D11_MAPPED_SUBRESOURCE ms;
	deviceContext->Map(_mesh->GetTexture()->GetTexture(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// CPU �����͸� GPU�� �����Ͽ� �ؽ�ó�� ���ο� ���� �����͸� �����
	//memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(Vec4));

	// �ؽ�ó ���� ���� (������Ʈ �Ϸ�)
	deviceContext->Unmap(_mesh->GetTexture()->GetTexture(), NULL);
}

void Example::RenderBegin()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

	// set the shader objects
	deviceContext->VSSetShader(vertexShader, 0, 0);
	deviceContext->PSSetShader(pixelShader, 0, 0);


}

void Example::MeshRender(Mesh* _mesh)
{
	// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);

	// https://github.com/Microsoft/DirectXTK/wiki/Getting-Started
	// https://github.com/microsoft/Xbox-ATG-Samples/tree/main/PCSamples/IntroGraphics/SimpleTexturePC
	deviceContext->PSSetSamplers(0, 1, _mesh->GetTexture()->GetSamplerState());//TODO: samplers to array
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// for ()
	{
		// rect ������ ���鼭
		// vectex ���� ������Ʈ�ϰ�
		deviceContext->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer(), &stride, &offset);
		deviceContext->PSSetShaderResources(0, 1, _mesh->GetTexture()->GetShaderResourceView());
		// drawIndexed();
	}

	deviceContext->DrawIndexed(_mesh->GetIndexCount(), 0, 0);
}

void Example::Clean()
{
	if (layout) { layout->Release(); layout = NULL; }
	if (vertexShader) { vertexShader->Release(); vertexShader = NULL; }
	if (pixelShader) { pixelShader->Release(); pixelShader = NULL; }
	if (renderTargetView) { renderTargetView->Release(); renderTargetView = NULL; }
	if (swapChain) { swapChain->Release(); swapChain = NULL; }
	if (deviceContext) { deviceContext->Release(); deviceContext = NULL; }
	if (device) { device->Release(); device = NULL; }
}
