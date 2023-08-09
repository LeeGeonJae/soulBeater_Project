#include "Example.h"
#include "Texture.h"
#include "Mesh.h"

void Example::InitShaders()
{
	// 정점 쉐이더 컴파일
	ID3DBlob* vertexBlob = nullptr;
	ID3DBlob* pixelBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// "VS.hlsl" 파일을 컴파일하여 정점 쉐이더를 생성 (vs_5_0 프로파일 사용)
	if (FAILED(D3DCompileFromFile(L"../Bin/Binaries/VS.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
	{
		// 컴파일 오류가 발생한 경우 오류 메시지 출력
		if (errorBlob) {
			std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	// "PS.hlsl" 파일을 컴파일하여 픽셀 쉐이더를 생성 (ps_5_0 프로파일 사용)
	if (FAILED(D3DCompileFromFile(L"../Bin/Binaries/PS.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
	{
		// 컴파일 오류가 발생한 경우 오류 메시지 출력
		if (errorBlob) {
			std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	// 컴파일된 정점 쉐이더와 픽셀 쉐이더로부터 DirectX 11에서 사용 가능한 정점 쉐이더와 픽셀 쉐이더를 생성
	device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader);
	device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);

	// 입력 레이아웃 생성
	// 정점 쉐이더 입력 레이아웃을 정의하는 D3D11_INPUT_ELEMENT_DESC 배열을 생성
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// 입력 레이아웃 생성 및 정점 쉐이더와 연결
	device->CreateInputLayout(ied, 3, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &layout);

	// 입력 레이아웃을 사용하도록 설정
	deviceContext->IASetInputLayout(layout);
}

void Example::Initialize(HWND window)
{
	// DXGI_SWAP_CHAIN_DESC 구조체 생성 및 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = WIDTH;                          // 백 버퍼의 가로 크기 설정
	swapChainDesc.BufferDesc.Height = HEIGHT;                        // 백 버퍼의 세로 크기 설정
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // 32비트 색상 형식 사용
	swapChainDesc.BufferCount = 2;                                  // 백 버퍼 개수 설정 (더블 버퍼링)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;           // 주사율 분자 (60 FPS)
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;          // 주사율 분모 (60 FPS)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // 스왑 체인의 사용 방식 설정 (렌더 타겟으로 사용)
	swapChainDesc.OutputWindow = window;                            // 렌더링 대상 윈도우 핸들 설정
	swapChainDesc.SampleDesc.Count = 1;                             // 멀티 샘플링 설정 (비활성화)
	swapChainDesc.Windowed = TRUE;                                 // 윈도우 모드로 설정
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // 전체 화면 전환 허용
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;           // 스왑 체인 효과 설정 (DISCARD 방식)

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// 특정 기능 수준에서 D3D 장치와 스왑 체인 생성
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

	// 렌더 타겟 생성
	ID3D11Texture2D* pBackBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
	{
		// 백 버퍼 텍스처를 이용하여 렌더 타겟 뷰 생성
		device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
		pBackBuffer->Release();
	}
	else
	{
		std::cout << "CreateRenderTargetView() error" << std::endl;
		exit(-1);
	}

	// 뷰포트 설정
	// 화면 뷰포트를 생성하고 초기화
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(WIDTH);   // 뷰포트의 가로 크기 설정
	viewport.Height = float(HEIGHT); // 뷰포트의 세로 크기 설정
	viewport.MinDepth = 0.0f;        // 뷰포트의 최소 깊이 값 설정
	viewport.MaxDepth = 1.0f;        // 뷰포트의 최대 깊이 값 설정 (중요: 깊이 버퍼링에 중요한 값)
	deviceContext->RSSetViewports(1, &viewport);

	// 셰이더 초기화
	InitShaders(); // 사용자 정의 함수 (셰이더 초기화 등의 작업)
}

void Example::Update(Mesh* _mesh)
{
	//// 색상 저장을 위한 픽셀 데이터 배열 생성 및 초기화
	//std::vector<Vec4> pixels(CANVAS_WIDTH * CANVAS_HEIGHT, Vec4{ 0.8f, 0.8f, 0.8f, 1.0f });

	//// 특정 픽셀 위치에 색상 값 할당 (예: (0, 0) 위치는 빨간색, (1, 0) 위치는 노란색, (2, 0) 위치는 흰색)
	//pixels[0 + CANVAS_WIDTH * 0] = Vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
	//pixels[1 + CANVAS_WIDTH * 0] = Vec4{ 1.0f, 1.0f, 0.0f, 1.0f };
	//pixels[2 + CANVAS_WIDTH * 0] = Vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

	// 텍스처 버퍼 업데이트를 위해 텍스처 매핑
	D3D11_MAPPED_SUBRESOURCE ms;
	deviceContext->Map(_mesh->GetTexture()->GetTexture(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// CPU 데이터를 GPU로 복사하여 텍스처에 새로운 색상 데이터를 기록함
	//memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(Vec4));

	// 텍스처 매핑 해제 (업데이트 완료)
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
		// rect 정보를 돌면서
		// vectex 버퍼 업데이트하고
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
