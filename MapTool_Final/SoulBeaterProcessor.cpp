#include "SoulBeaterProcessor.h"

#include "defalut.h"

#include "ObjectManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MapToolWinApp.h"
#include "RenderManger.h"
#include "GameObject.h"
#include "Transform.h"
#include "AABBCollider.h"
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

		getRenderManager()->SetHwnd(MapToolWinApp::GetHwnd());
		GameProcessor::Init();
		getSceneManager()->RegisterScene("hoho", new Scene("hoho"));
		// register Register Scene;

		mImGui->CreateHWindow("Map Tool Menu", "Map Tool Class");
		mImGui->CreateDevice();
		mImGui->CreateImGui();

		HRESULT hr = getRenderManager()->CreateD2DBitmapFromFile(L"Golem", L"../Bin/image/Golem.png");
		hr = getRenderManager()->CreateD2DBitmapFromFile(L"Charactor", L"../Bin/image/Charactor.png");
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
		getRenderManager()->DrawPoint((float)pos.x, (float)pos.y);

		// 격자판 그리기
		{
			getRenderManager()->DrawGrid(0, 0, (float)MapToolGui::mWidth, (float)MapToolGui::mHeight, (float)MapToolGui::mGridDistance);
			CheckedRender();
			ColliderRender();
		}
		
		// 타일 오브젝트 생성
		{
			TileObjectCreate();
			TileObjectDelete();
		}

		// 오브젝트 생성 ( 컴포넌트 생성 ) 및 삭제
		{
			ObjectCreate();
			ColliderSetting();
			SpriteSetting();
			ObjectDelete();
		}

		getRenderManager()->SetStrokeWidth(2);
	}

	void SoulBeaterProcessor::MouseClickCheck()
	{
		using namespace d2dFramework;

		// 버튼 클릭 시 그리드에 맞는 곳 체크
		if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == eKeyState::Push && MapToolGui::mGridDistance != 0)
		{
			POINT point = InputManager::GetInstance()->GetMousePos();

			int mouseX = point.x;
			int mouseY = point.y;
			int arrX = mouseX / (float)MapToolGui::mGridDistance;
			int arrY = mouseY / (float)MapToolGui::mGridDistance;

			mRect.top = (float)arrY;
			mRect.left = (float)arrX;
			mRect.bottom = (float)arrY + MapToolGui::mGridDistance;
			mRect.right = (float)arrX + MapToolGui::mGridDistance;

			if (MapToolGui::mWidth / MapToolGui::mGridDistance > arrX && MapToolGui::mHeight / MapToolGui::mGridDistance > arrY)
			{
				if (MapToolGui::mbIsChecked[arrX][arrY] == true)
				{
					MapToolGui::mbIsChecked[arrX][arrY] = false;
					MapToolGui::ItemImfoCurrentIndex = 0;
				}
				else
					MapToolGui::mbIsChecked[arrX][arrY] = true;
			}
		}
	}

	void SoulBeaterProcessor::CheckedRender()
	{
		for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
		{
			for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
			{
				if (MapToolGui::mbIsChecked[i][j] == true)
				{
					getRenderManager()->SetStrokeWidth(5);
					getRenderManager()->DrawRectangle((float)i * MapToolGui::mGridDistance, (float)j * MapToolGui::mGridDistance,
						(float)(i + 1) * MapToolGui::mGridDistance, (float)(j + 1) * MapToolGui::mGridDistance);
					getRenderManager()->SetStrokeWidth(2);
				}
			}
		}
	}

	void SoulBeaterProcessor::ColliderRender()
	{
		using namespace d2dFramework;

		for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
		{
			for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
			{
				if (MapToolGui::mbIsObject[i][j] == true)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mObjectIdMap.find(key);

					GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);
					AABBCollider* collider = object->GetComponent<AABBCollider>();

					if (collider != nullptr)
					{
						getRenderManager()->DrawRectangle(
							(float)i * MapToolGui::mGridDistance - collider->GetSize().GetX() * 0.5f  + MapToolGui::mGridDistance * 0.5f
							, (float)j * MapToolGui::mGridDistance - collider->GetSize().GetY() * 0.5f + MapToolGui::mGridDistance * 0.5f
							, (float)i * MapToolGui::mGridDistance + collider->GetSize().GetX() * 0.5f + MapToolGui::mGridDistance * 0.5f
							, (float)j * MapToolGui::mGridDistance + collider->GetSize().GetY() * 0.5f + MapToolGui::mGridDistance * 0.5f);
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::TileObjectCreate()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsCreateTileObject())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					//체크가 되었으면서 오브젝트가 없는 곳에 오브젝트 생성
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == false)
					{
						//오브젝트 생성
						Vector2 pos =
						{
							i * static_cast<float>(MapToolGui::mGridDistance) - GetWidth() * 0.5f + MapToolGui::mGridDistance * 0.5f,
							j * -static_cast<float>(MapToolGui::mGridDistance) + GetHeight() * 0.5f - MapToolGui::mGridDistance * 0.5f
						};

						GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mTileId);
						MapToolGui::mTileObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mTileId++));
						MapToolGui::mbIsTileObject[i][j] = true;

						Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mTileId++);
						TransformComponent->SetTranslate(pos);
						SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mTileId++);
						ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::TileBitmap.c_str()));
						ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::TileSpriteWidth), static_cast<float>(MapToolGui::TileSpriteHeight) });
						ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
						ComponentRenderer->SetSpriteType(eSpriteType::Sprite);
						ComponentRenderer->SetBaseColor({ 1,0,0,1 });
					}
					else if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == true)
					{
						std::pair<int, int> key = std::make_pair(i, j);
						auto iter = MapToolGui::mTileObjectIdMap.find(key);

						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);

						if (object != nullptr)
						{
							SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();
							ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::TileBitmap.c_str()));
						}
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::TileObjectDelete()
	{
		// 타일 오브젝트 삭제
		if (mImGui->GetIsDeleteTileObject())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mTileObjectIdMap.find(key);

					// 체크되어 있고 오브젝트가 있고 Id가 있을 때에 오브젝트 삭제
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == true && iter != MapToolGui::mTileObjectIdMap.end())
					{
						MapToolGui::mbIsTileObject[i][j] = false;

						d2dFramework::ObjectManager::GetInstance()->DeletObject(iter->second);
						MapToolGui::mTileObjectIdMap.erase(iter);
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::ObjectCreate()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsCreateObject())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					//체크가 되었으면서 오브젝트가 없는 곳에 오브젝트 생성
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsObject[i][j] == false)
					{
						//오브젝트 생성
						Vector2 pos =
						{
							i * static_cast<float>(MapToolGui::mGridDistance) - GetWidth() * 0.5f + MapToolGui::mGridDistance * 0.5f,
							j * -static_cast<float>(MapToolGui::mGridDistance) + GetHeight() * 0.5f - MapToolGui::mGridDistance * 0.5f
						};

						switch (MapToolGui::mIdSetting)
						{
						case IdSet::DEFALUE:
							break;
						case IdSet::PLAYERID:
						{
							GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mPlayerId);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mPlayerId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mPlayerId++);
							TransformComponent->SetTranslate(pos);
 
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mItemId++);
							ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
							ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
							ComponentRenderer->SetBaseColor({ 1,0,0,1 });
						}
							break;
						case IdSet::MONSTERID:
						{
							GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mMonsterId);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mMonsterId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mMonsterId++);
							TransformComponent->SetTranslate(pos);
						}
							break;
						case IdSet::ITEMID:
						{
							GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mItemId);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mItemId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mItemId++);
							TransformComponent->SetTranslate(pos);
						}
							break;
						}
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::ObjectDelete()
	{
		// 오브젝트 삭제
		if (mImGui->GetIsDeleteObject())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mObjectIdMap.find(key);

					// 체크되어 있고 오브젝트가 있고 Id가 있을 때에 오브젝트 삭제
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsObject[i][j] == true && iter != MapToolGui::mObjectIdMap.end())
					{
						MapToolGui::mbIsObject[i][j] = false;

						d2dFramework::ObjectManager::GetInstance()->DeletObject(iter->second);
						MapToolGui::mObjectIdMap.erase(iter);
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::ColliderSetting()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsCreateCollider())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mObjectIdMap.find(key);

					// 해당 배열이 체크되어 있고 오브젝트가 있고 오브젝트 아이디가 있을 때 실행
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsChecked[i][j] == true && iter != MapToolGui::mObjectIdMap.end())
					{
						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);
						AABBCollider* collider = object->GetComponent< AABBCollider>();

						// 오브젝트의 콜라이더가 없을 때에만 생성 콜라이더 있으면 기존 콜라이더 세팅 값으로 변경
						if (collider == nullptr)
						{
							if (iter->second >= ITEM_ID_START)
							{
								collider = object->CreateComponent<AABBCollider>(MapToolGui::mItemId++);
								collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
							}
							else if (iter->second >= MONSTER_ID_START)
							{
								collider = object->CreateComponent<AABBCollider>(MapToolGui::mMonsterId++);
								collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
							}
							else if (iter->second >= PLAYER_ID_START)
							{
								collider = object->CreateComponent<AABBCollider>(MapToolGui::mPlayerId++);
								collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
							}
						}
						else
						{
							collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
						}
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::SpriteSetting()
	{
		using namespace d2dFramework;

		// 해당 오브젝트에 스프라이트 컴포넌트 생성
		if (mImGui->GetIsCreateSprite())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mObjectIdMap.find(key);

					// 해당 배열이 체크되어 있고 오브젝트가 있고 오브젝트 아이디가 있을 때 실행
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsChecked[i][j] == true && iter != MapToolGui::mObjectIdMap.end())
					{
						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);
						SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();

						// 오브젝트의 콜라이더가 없을 때에만 생성 콜라이더 있으면 기존 콜라이더 세팅 값으로 변경
						if (ComponentRenderer == nullptr)
						{
							if (iter->second >= ITEM_ID_START)
							{
								ComponentRenderer = object->CreateComponent<SpriteRenderer>(MapToolGui::mItemId++);
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							else if (iter->second >= MONSTER_ID_START)
							{
								ComponentRenderer = object->CreateComponent<SpriteRenderer>(MapToolGui::mMonsterId++);
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							else if (iter->second >= PLAYER_ID_START)
							{
								ComponentRenderer = object->CreateComponent<SpriteRenderer>(MapToolGui::mPlayerId++);
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
						}
						else
						{
							ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
							ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
							ComponentRenderer->SetBaseColor({ 1,0,0,1 });
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