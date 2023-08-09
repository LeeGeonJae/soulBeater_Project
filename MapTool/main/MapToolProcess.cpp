#include "MapToolProcess.h"
#include "Mesh.h"
#include "Texture.h"
#include "Example.h"

MapToolProcess::MapToolProcess()
	: m_Mesh()
	, m_Example(nullptr)
	, m_hwnd(nullptr)
{
}

MapToolProcess::~MapToolProcess()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_Example->Clean();
	DestroyWindow(m_hwnd);

	for (auto mesh : m_Mesh)
		delete mesh;
}

void MapToolProcess::Initialize(HWND _hwnd)
{
	m_hwnd = _hwnd;

	m_Example = new Example(_hwnd);
	ResourceManager::m_pInstance->Init(m_Example->device);
	KeyManager::m_pInstance->Initialize();

	Mesh* mesh1 = new Mesh;
	mesh1->SetXYPosition(2, 1);
	mesh1->Initialize(m_Example->device);
	mesh1->SetTexture(ResourceManager::m_pInstance->FindTexture(L"Golem.png"));

	Mesh* mesh2 = new Mesh;
	mesh2->SetXYPosition(5, 5);
	mesh2->Initialize(m_Example->device);
	mesh2->SetTexture(ResourceManager::m_pInstance->FindTexture(L"Charactor.png"));

	m_Mesh.push_back(mesh1);
	m_Mesh.push_back(mesh2);

	// ImGui 생성
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(WIDTH, HEIGHT);
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplDX11_Init(m_Example->device, m_Example->deviceContext);
	ImGui_ImplWin32_Init(_hwnd);
}

void MapToolProcess::Loop()
{

	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			Data::MousePos.x = LOWORD(msg.lParam);
			Data::MousePos.y = HIWORD(msg.lParam);
		}
		else
		{
			KeyManager::m_pInstance->Update();

			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();//TODO: IMGUI 사용
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Background Color");

			// ImGui렌더러
			ImGuiRender();

			ImGui::End();
			ImGui::Render();

			// 게임 엔진 업데이트 및 렌더러
			Update();
			Render();

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());		//TODO: IMGUI 사용

			// switch the back buffer and the front buffer
			m_Example->swapChain->Present(1, 0);
		}
	}
}

void MapToolProcess::Update()
{
	for (auto mesh : m_Mesh)
		m_Example->Update(mesh);

	if (KeyManager::m_pInstance->GetKeyState(KEY::LBTN) == KEY_STATE::TAP)
	{
		for (auto mesh = m_Mesh.begin(); mesh != m_Mesh.end(); mesh++)
		{
			if (Data::MousePos.x > (*mesh)->GetMeshRect().left &&
				Data::MousePos.x < (*mesh)->GetMeshRect().right &&
				Data::MousePos.y >(*mesh)->GetMeshRect().top &&
				Data::MousePos.y < (*mesh)->GetMeshRect().bottom)
			{
				bool Check = true;

				for (auto selected = m_SelectedMesh.begin(); selected != m_SelectedMesh.end(); selected++)
				{
					if ((*selected) == *(mesh))
					{
						(*mesh)->SetIsSelected(false, m_Example->GetDeviceContext());

						m_SelectedMesh.erase(selected);
						Check = false;
						break;
					}
				}

				if (Check)
				{
					(*mesh)->SetIsSelected(true, m_Example->GetDeviceContext());
					m_SelectedMesh.push_back(*mesh);
				}
			}
		}
	}
}

void MapToolProcess::Render()
{
	m_Example->RenderBegin();

	for (auto mesh : m_Mesh)
	{
		mesh->Render();
		m_Example->MeshRender(mesh);
	}

	for (auto selectedmesh : m_SelectedMesh)
		m_Example->MeshRender(selectedmesh);

	//selectedmesh->SelectedRender();
}

void MapToolProcess::ImGuiRender()
{
	// ImGui 렌더러
	// Grid X, Y값 맞추기
	ImGui::Text("----------------------------------------");
	ImGui::Text("Grid Setting");
	ImGui::InputInt2("Grid x, y", Data::Grid);

	// 세팅된 Grid 값에 따라 메쉬 삭제 후 생성
	if (ImGui::Button("Grid Apply"))
	{
		// 모든 메쉬 삭제
		for (auto m : m_Mesh)
			delete m;
		m_Mesh.clear();
		m_SelectedMesh.clear();

		// 그리드에 맞게 메쉬 생성 후 기본 값 세팅
		for (int i = 0; i < Data::Grid[1]; i++)
		{
			for (int j = 0; j < Data::Grid[0]; j++)
			{
				Mesh* mesh = new Mesh;
				mesh->SetXYPosition(j, i);
				mesh->Initialize(m_Example->device);
				mesh->SetTexture(ResourceManager::m_pInstance->FindTexture(L"Golem.png"));

				m_Mesh.push_back(mesh);
			}
		}
	}

	ImGui::Text("----------------------------------------");
	ImGui::Text("Mesh Texture Setting");
	// 메쉬 텍스쳐 변경
	static int ItemCurrentIndex = 0;
	const char* item[] = { "Golem.png", "Charactor.png" };
	wstring items[] = { L"Golem.png", L"Charactor.png" };
	if (ImGui::BeginListBox("ImageBox"))
	{
		for (int n = 0; n < 2; n++)
		{
			const bool Selected = (ItemCurrentIndex == n);
			if (ImGui::Selectable(item[n], Selected))
			{
				ItemCurrentIndex = n;
				cout << item[n] << endl;

				for (int i = 0; i < m_SelectedMesh.size(); i++)
				{
					m_SelectedMesh[i]->SetTexture(ResourceManager::m_pInstance->FindTexture(items[n]));
					m_SelectedMesh[i]->Initialize(m_Example->device);
				}
			}
		}
	}
	ImGui::EndListBox();
}