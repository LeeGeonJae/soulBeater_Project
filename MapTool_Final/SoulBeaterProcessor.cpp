#include "SoulBeaterProcessor.h"

#include "defalut.h"

#include "ObjectManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MapToolWinApp.h"
#include "RenderManger.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "Collider.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "gui.h"

namespace mapTool
{
	SoulBeaterProcessor::SoulBeaterProcessor(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mWidth(0)
		, mHeight(0)
		, mGridDistance(0)
		, mRect{}
	{
	}

	void SoulBeaterProcessor::Init()
	{
		const int canvasWidth = GetWidth() / 80, canvasHeight = GetHeight() / 80;

		using namespace d2dFramework;

		static unsigned int tempId = 20000;

		getRenderManager()->SetHwnd(MapToolWinApp::GetHwnd());
		GameProcessor::Init();
		getSceneManager()->RegisterScene("hoho", new Scene("hoho"));
		// register Register Scene;

		mImGui->CreateHWindow("Map Tool Menu", "Map Tool Class");
		mImGui->CreateDevice();
		mImGui->CreateImGui();
	}

	void SoulBeaterProcessor::Update()
	{
		using namespace d2dFramework;

		// 게임 루프 관련 처리는 해당 인덴트 안에서 처리하도록 해야한다.
		getRenderManager()->BeginDraw(); // 얘는 렌더링 초기화
		{
			MouseClickCheck();
			GameProcessor::Update(); // 얘는 프레임워크 렌더링 로직까지 포함한 게임루프 
			Render();
		}
		getRenderManager()->EndDraw();

		// switch the back buffer and the front buffer
		getRenderManager()->Present();

		mImGui->BeginRender();
		mImGui->Render();
		mImGui->EndRender();
	}

	void SoulBeaterProcessor::Release()
	{
		using namespace d2dFramework;

		GameProcessor::Release();

		mImGui->DestroyImGui();
		mImGui->DestroyDevice();
		mImGui->DestroyHWindow();
	}

	void SoulBeaterProcessor::Render()
	{
		POINT pos = d2dFramework::InputManager::GetInstance()->GetMousePos();
		getRenderManager()->DrawPoint(pos.x, pos.y);

		// 격자판 그리기
		getRenderManager()->DrawGrid(0, 0, mImGui->GetGridWidth(), mImGui->GetGridHeight(), mImGui->GetGridDistance());
		CheckedRender();

		// 오브젝트 생성 및 삭제
		ObjectCreate();
		ObjectDelete();

		getRenderManager()->SetStrokeWidth(2);
	}

	void SoulBeaterProcessor::MouseClickCheck()
	{
		using namespace d2dFramework;

		// 버튼 클릭 시 그리드에 맞는 곳 체크
		if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == eKeyState::Push && mImGui->GetGridDistance() != 0)
		{
			POINT point = InputManager::GetInstance()->GetMousePos();

			int mouseX = point.x;
			int mouseY = point.y;
			int arrX = mouseX / (float)mImGui->GetGridDistance();
			int arrY = mouseY / (float)mImGui->GetGridDistance();

			mRect.top = arrY;
			mRect.left = arrX;
			mRect.bottom = arrY + mImGui->GetGridDistance();
			mRect.right = arrX + mImGui->GetGridDistance();

			if (mImGui->GetGridWidth() / mImGui->GetGridDistance() > arrX && mImGui->GetGridHeight() / mImGui->GetGridDistance() > arrY)
			{
				if (mImGui->GetIsChecked()[arrX][arrY] == true)
					mImGui->SetIsChecked(arrX, arrY, false);
				else
					mImGui->SetIsChecked(arrX, arrY, true);
			}
		}
	}

	void SoulBeaterProcessor::CheckedRender()
	{
		for (int i = 0; i < mImGui->GetIsChecked().size(); i++)
		{
			for (int j = 0; j < mImGui->GetIsChecked()[i].size(); j++)
			{
				if (mImGui->GetIsChecked()[j][i] == true)
				{
					getRenderManager()->SetStrokeWidth(5);
					getRenderManager()->DrawRectangle(j * mImGui->GetGridDistance(), i * mImGui->GetGridDistance(), 
						(j + 1) * mImGui->GetGridDistance(), (i + 1) * mImGui->GetGridDistance());
					getRenderManager()->SetStrokeWidth(2);
				}
			}
		}
	}

	void SoulBeaterProcessor::ObjectCreate()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsCreateObject())
		{
			//체크가 되었으면서 오브젝트가 없는 곳에 오브젝트 생성
			for (int i = 0; i < mImGui->GetIsChecked().size(); i++)
			{
				for (int j = 0; j < mImGui->GetIsChecked()[i].size(); j++)
				{
					if (mImGui->GetIsChecked()[j][i] == true && mImGui->GetIsObject()[j][i] == false)
					{
						mImGui->SetIsObject(j, i, true);

						//오브젝트 생성
						Vector2 pos =
						{
							j * static_cast<float>(mImGui->GetGridDistance()) - GetWidth() * 0.5f + mImGui->GetGridDistance() * 0.5f,
							i * -static_cast<float>(mImGui->GetGridDistance()) + GetHeight() * 0.5f - mImGui->GetGridDistance() * 0.5f
						};

						GameObject* Object = ObjectManager::GetInstance()->CreateObject(tempId);
						mImGui->SetObjectMapInsert(j, i, tempId++);

						Transform* TransformComponent = Object->CreateComponent<Transform>(tempId++);
						TransformComponent->SetTranslate(pos);
						SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(tempId++);
						ComponentRenderer->SetSize({ static_cast<float>(mImGui->GetGridDistance()), static_cast<float>(mImGui->GetGridDistance()) });
						ComponentRenderer->SetSpriteType(eSpriteType::Circle);
						ComponentRenderer->SetBaseColor({ 1,0,0,1 });
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::ObjectDelete()
	{
		if (mImGui->GetIsDeleteObject())
		{
			for (int i = 0; i < mImGui->GetIsChecked().size(); i++)
			{
				for (int j = 0; j < mImGui->GetIsChecked()[i].size(); j++)
				{
					if (mImGui->GetIsChecked()[i][j] == true && mImGui->GetIsObject()[i][j] == true)
					{
						std::pair<int, int> key = std::make_pair(i, j);

						if (mImGui->mObjectIdMap.find(key) != mImGui->mObjectIdMap.end())
						{
							mImGui->SetIsObject(i, j, false);

							auto iter = mImGui->mObjectIdMap.find(key);

							d2dFramework::ObjectManager::GetInstance()->DeletObject(iter->second);
							mImGui->mObjectIdMap.erase(iter);
						}
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::ImGuiImageLoading()
	{
		using namespace d2dFramework;

		// ImGui 텍스트 박스에 입력될 static char
		static char ImageName[100] = "Image Name";
		static char ImagePath[256] = "Image Path";

		// ImGui의 리스트 이름(함수와 호환이 될 자료형 리스트 이름)
		static std::vector<std::string> ListName = {};
		static std::vector<std::wstring> WstringListName = {};

		static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

		// 텍스트 박스 생성
		ImGui::InputTextMultiline("##source1", ImageName, IM_ARRAYSIZE(ImageName), ImVec2(100.f, 23.f), flags);
		ImGui::InputTextMultiline("##source2", ImagePath, IM_ARRAYSIZE(ImagePath), ImVec2(-FLT_MIN, 23.f), flags);

		if (ImGui::Button("Image Create Button"))
		{
			// 로케일 설정
			std::locale loc("");

			// char 배열을 wchar_t 배열로 변환
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wstrName = converter.from_bytes(ImageName);
			std::wstring wstrPath = converter.from_bytes(ImagePath);

			const WCHAR* const ImageNameWchar = wstrName.c_str();
			const WCHAR* const ImagePathWchar = wstrPath.c_str();

			// 비트맵을 찾고 비트맵이 있으면 종료
			auto iter = getRenderManager()->GetBitmapOrNull(ImageNameWchar);

			if (iter == nullptr)
			{
				// 파일 경로에 이미지 파일 있으면 생성
				HRESULT hr = getRenderManager()->CreateD2DBitmapFromFile(ImageNameWchar, ImagePathWchar);

				if (SUCCEEDED(hr))
				{
					// 이미지 리스트에 넣을 문자열 저장
					ListName.push_back(ImageName);
					WstringListName.push_back(ImageNameWchar);
				}
			}
		}

		// 이미지 리스트 생성 및 이미지 클릭 시 이미지 적용
		static int ItemCurrentIndex = 0;

		if (ImGui::BeginListBox("Image List"))
		{
			for (int n = 0; n < ListName.size(); n++)
			{
				const bool Selected = (ItemCurrentIndex == n);
				if (ImGui::Selectable(ListName[n].c_str(), Selected))
				{
					ItemCurrentIndex = n;

					// 선택 된 오브젝트 비트맵 수정
					for (auto ObjectMap : mObjectIdMap)
					{
						if (mbIsChecked[ObjectMap.first.second][ObjectMap.first.first] == true)
						{
							GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(ObjectMap.second);

							if (object == nullptr)
								return;

							// 비트맵으로 수정
							SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();
							ComponentRenderer->SetSpriteType(eSpriteType::Sprite);
							ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(WstringListName[n].c_str()));
							ComponentRenderer->SetSize({ static_cast<float>(mGridDistance), static_cast<float>(mGridDistance) });
							ComponentRenderer->SetUVRectangle({ 0,0, 500, 500 });
						}
					}
				}
			}
		}
		ImGui::EndListBox();

		ImGui::TreePop();
	}
}

//{
//	void SoulBeaterProcessor::ImGuiResetGridSetting()
//	{
//		static int GridArea[2] = {};
//		static int GridInstance = 0;
//
//		// 그리드 가로,세로 값세팅 및 그리드 사이 간격 길이 입력
//		ImGui::InputInt2("Grid Area Width, Height", GridArea);
//		ImGui::InputInt("Grid Distance", &GridInstance);
//
//		// 그리드 세팅 버튼을 누르면 그리드 칸에 맞춰서 세팅
//		if (ImGui::Button("Grid Setting"))
//		{
//			for (int i = 0; i < mbIsChecked.size(); i++)
//				mbIsChecked[i].clear();
//			mbIsChecked.clear();
//
//			for (int i = 0; i < mbIsObject.size(); i++)
//				mbIsObject[i].clear();
//			mbIsObject.clear();
//
//			mWidth = GridArea[0];
//			mHeight = GridArea[1];
//			mGridDistance = GridInstance;
//
//			// 그리드에 맞게 리사이즈
//			mImGui->mbIsChecked.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
//			mImGui->mbIsObject.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
//
//			// 모든 오브젝트 삭제
//			for (auto e : mObjectIdMap)
//				d2dFramework::ObjectManager::GetInstance()->DeletObject(e.second);
//			mObjectIdMap.clear();
//		}
//
//		ImGui::TreePop();
//	}
//
//void SoulBeaterProcessor::ImGuiRender()
//{
//	ImGui::Begin("Map Tool Menu");
//
//	// 문자열{ 이미지 이름, 파일 경로 } 을 입력받아 비트맵 로딩하여 이미지 이름으로 리스트에 추가하기 
//	if (ImGui::TreeNode("Image Create"))
//		ImGuiImageLoading();
//
//	// 격자판 생성 시
//	if (ImGui::TreeNode("Grid Setting & Create"))
//		ImGuiResetGridSetting();
//}
//}