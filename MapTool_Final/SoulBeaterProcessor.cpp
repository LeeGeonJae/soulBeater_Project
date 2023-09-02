#include "SoulBeaterProcessor.h"

#include "defalut.h"

#include "gui.h"

#include "MapToolWinApp.h"
#include"SceneLoader.h"

#include "ObjectManager.h"
#include "RenderManger.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "AABBCollider.h"
#include "CircleCollider.h"
#include "SpriteRenderer.h"
#include "Collider.h"
#include "GridComponent.h"

namespace mapTool
{
	SoulBeaterProcessor::SoulBeaterProcessor(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mRect{}
		, mImGui(new MapToolGui(getSceneManager()))
	{
	}

	void SoulBeaterProcessor::Init(HWND hwnd)
	{
		const int canvasWidth = GetWidth() / 80, canvasHeight = GetHeight() / 80;

		using namespace d2dFramework;

		GameProcessor::Init(hwnd);
		getSceneManager()->CreateScene<Scene>(50000);
		// register Register Scene;

		mImGui->CreateHWindow("Map Tool Menu", "Map Tool Class");
		mImGui->CreateDevice();
		mImGui->CreateImGui();

		SceneLoader::LoadAllBitmaps();
	}

	void SoulBeaterProcessor::Update()
	{
		using namespace d2dFramework;

		// 게임 루프 관련 처리는 해당 인덴트 안에서 처리하도록 해야한다.
		RenderManager::GetInstance()->BitmapBeginDraw(); // 얘는 렌더링 초기화
		{
			MouseClickCheck();
			GameProcessor::Update(); // 얘는 프레임워크 렌더링 로직까지 포함한 게임루프 
			Render();
		}
		RenderManager::GetInstance()->BitmapEndDraw();
		RenderManager::GetInstance()->BeginDraw();
		RenderManager::GetInstance()->CopyBitmapRenderToHwndRender();
		RenderManager::GetInstance()->EndDraw();

		// switch the back buffer and the front buffer

		mImGui->BeginRender();
		mImGui->Render();
		ImGuiImageLoading();
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
		d2dFramework::RenderManager::GetInstance()->DrawPoint((float)pos.x, (float)pos.y);

		// 격자판 그리기
		{
			CreateScene();
			d2dFramework::RenderManager::GetInstance()->DrawGrid(0, 0, (float)MapToolGui::mGridXCount * MapToolGui::mGridDistance, (float)MapToolGui::mGridYCount * MapToolGui::mGridDistance, (float)MapToolGui::mGridDistance);
			CheckedRender();
			ColliderRender();
		}

		// 타일 오브젝트 생성
		{
			TileObjectCreate();
			TileObjectDelete();
			//TileColliderCreate();
		}

		// 오브젝트 생성 ( 컴포넌트 생성 ) 및 삭제
		{
			ObjectCreate();
			ColliderSetting();
			SpriteSetting();
			ObjectDelete();
		}

		// 세이브
		{
			SaveAndLoad();
		}

		d2dFramework::RenderManager::GetInstance()->SetStrokeWidth(2);
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

			if (MapToolGui::mGridXCount > arrX && MapToolGui::mGridYCount > arrY)
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
		using namespace d2dFramework;

		for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
		{
			for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
			{
				if (MapToolGui::mbIsChecked[i][j] == true)
				{
					RenderManager::GetInstance()->SetStrokeWidth(5);
					D2D1_COLOR_F prevColor = d2dFramework::RenderManager::GetInstance()->SetColor({ 1,0,0,1 });
					RenderManager::GetInstance()->DrawRectangle((float)i * MapToolGui::mGridDistance, (float)j * MapToolGui::mGridDistance,
						(float)(i + 1) * MapToolGui::mGridDistance, (float)(j + 1) * MapToolGui::mGridDistance);
					d2dFramework::RenderManager::GetInstance()->SetColor(prevColor);
					RenderManager::GetInstance()->SetStrokeWidth(2);
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
					CircleCollider* collider = object->GetComponent<CircleCollider>();

					if (collider != nullptr)
					{
						RenderManager::GetInstance()->DrawCircle(
							(float)i * MapToolGui::mGridDistance + MapToolGui::mGridDistance * 0.5f
							, (float)j * MapToolGui::mGridDistance + MapToolGui::mGridDistance * 0.5f
							, collider->GetRadius()
							, collider->GetRadius());
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
						Object->SetObjectType(eObjectType::Background);
						MapToolGui::mTileObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mTileId++));
						MapToolGui::mbIsTileObject[i][j] = true;
						Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mTileId++);
						GridComponent* Grid = Object->CreateComponent<GridComponent>(MapToolGui::mTileId++);
						Grid->SetCellDistance(MapToolGui::mGridDistance);
						Grid->SetMaxX(MapToolGui::mGridXCount);
						Grid->SetMaxY(MapToolGui::mGridYCount);
						Grid->SetPosition({ static_cast<unsigned int>(i), static_cast<unsigned int>(MapToolGui::mbIsTileObject[i].size() - j - 1) });
						TransformComponent->SetTranslate(pos);
						SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mTileId++);
						ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
						ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::TileSpriteWidth), static_cast<float>(MapToolGui::TileSpriteHeight) });
						ComponentRenderer->SetUVRectangle({ (float)((0 + MapToolGui::BitmapXNumber) * MapToolGui::BitmapU)
							,(float)((0 + MapToolGui::BitmapYNumber) * MapToolGui::BitmapV)
							,(float)((1 + MapToolGui::BitmapXNumber) * MapToolGui::BitmapU)
							,(float)((1 + MapToolGui::BitmapYNumber) * MapToolGui::BitmapV) });
						ComponentRenderer->SetSpriteType(eSpriteType::Sprite);
						ComponentRenderer->SetBaseColor({ 1,0,0,1 });

						if (mImGui->GetIsCreateTileCollider())
						{
							AABBCollider* collider = Object->CreateComponent<AABBCollider>(MapToolGui::mTileId++);
							collider->SetSize({ static_cast<float>(MapToolGui::mGridDistance), static_cast<float>(MapToolGui::mGridDistance) });
						}
					}
					// 오브젝트가 생성이 되어 있다면 비트맵만 변경
					else if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == true)
					{
						std::pair<int, int> key = std::make_pair(i, j);
						auto iter = MapToolGui::mTileObjectIdMap.find(key);

						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);

						if (object != nullptr)
						{
							SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();
							ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
						}
					}
				}
			}
		}
	}

	void SoulBeaterProcessor::TileColliderCreate()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsCreateTileCollider())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mTileObjectIdMap.find(key);

					// 해당 타일이 체크되고 타일 오브젝트가 있을 때
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == true && iter != MapToolGui::mTileObjectIdMap.end())
					{
						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);
						AABBCollider* Collider = object->GetComponent<AABBCollider>();
						int a = 0;
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

						d2dFramework::ObjectManager::GetInstance()->DeleteObject(iter->second);
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

						switch (MapToolGui::mIdSetting)		// 오브젝트 타입별 Id, 오브젝트 타입, 오브젝트 생성
						{
						case IdSet::DEFALUE:
							break;
						case IdSet::PLAYERID:
						{
							GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mPlayerId);
							Object->SetObjectType(eObjectType::Player);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mPlayerId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mPlayerId++);
							TransformComponent->SetTranslate(pos);
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mPlayerId++);
							CircleCollider* collider = Object->CreateComponent<CircleCollider>(MapToolGui::mPlayerId++);
							GridComponent* Gird = Object->CreateComponent<GridComponent>(MapToolGui::mPlayerId++);

							if (MapToolGui::bIsSpriteCheck)		// 스프라이트 컴포넌트 세팅
							{
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::CreateSpriteWidth), static_cast<float>(MapToolGui::CreateSpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
								ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							if (MapToolGui::bIsColliderCheck)	// 콜라이더 컴포넌트 세팅
							{
								collider->SetRadius(MapToolGui::ColliderWidth);
								collider->SetRadius({ static_cast<float>(MapToolGui::ColliderWidth) });
							}
							if (MapToolGui::mbIsGridCheck)		// 그리드 컴포넌트 세팅
							{
								Gird->SetCellDistance(MapToolGui::mGridDistance);
								Gird->SetMaxX(MapToolGui::mGridXCount);
								Gird->SetMaxY(MapToolGui::mGridYCount);
								Gird->SetPosition({ static_cast<unsigned int>(i), static_cast<unsigned int>(j) });
								TransformComponent->SetTranslate(pos);
							}
						}
						break;
						case IdSet::MONSTERID:
						{
							GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mMonsterId);
							Object->SetObjectType(eObjectType::Enemy);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mMonsterId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mMonsterId++);
							TransformComponent->SetTranslate(pos);
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mMonsterId++);
							CircleCollider* collider = Object->CreateComponent<CircleCollider>(MapToolGui::mMonsterId++);
							GridComponent* Gird = Object->CreateComponent<GridComponent>(MapToolGui::mMonsterId++);

							if (MapToolGui::bIsSpriteCheck)		// 스프라이트 컴포넌트 세팅
							{
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::CreateSpriteWidth), static_cast<float>(MapToolGui::CreateSpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
								ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							if (MapToolGui::bIsColliderCheck)	// 콜라이더 컴포넌트 세팅
							{
								collider->SetRadius({ static_cast<float>(MapToolGui::ColliderWidth) });
							}
							if (MapToolGui::mbIsGridCheck)		// 그리드 컴포넌트 세팅
							{
								Gird->SetCellDistance(MapToolGui::mGridDistance);
								Gird->SetMaxX(MapToolGui::mGridXCount);
								Gird->SetMaxY(MapToolGui::mGridYCount);
								Gird->SetPosition({ static_cast<unsigned int>(i), static_cast<unsigned int>(j) });
								TransformComponent->SetTranslate(pos);
							}
						}
						break;
						case IdSet::ITEMID:
						{
							GameObject* Object = ObjectManager::GetInstance()->CreateObject(MapToolGui::mItemId);
							Object->SetObjectType(eObjectType::Item);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mItemId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mItemId++);
							TransformComponent->SetTranslate(pos);
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mItemId++);
							CircleCollider* collider = Object->CreateComponent<CircleCollider>(MapToolGui::mItemId++);
							GridComponent* Gird = Object->CreateComponent<GridComponent>(MapToolGui::mItemId++);

							if (MapToolGui::bIsSpriteCheck)		// 스프라이트 컴포넌트 세팅
							{
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::CreateSpriteWidth), static_cast<float>(MapToolGui::CreateSpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
								ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							if (MapToolGui::bIsColliderCheck)	// 콜라이더 컴포넌트 세팅
							{
								collider->SetRadius(static_cast<float>(MapToolGui::ColliderWidth));
							}
							if (MapToolGui::mbIsGridCheck)		// 그리드 컴포넌트 세팅
							{
								Gird->SetCellDistance(MapToolGui::mGridDistance);
								Gird->SetMaxX(MapToolGui::mGridXCount);
								Gird->SetMaxY(MapToolGui::mGridYCount);
								Gird->SetPosition({ static_cast<unsigned int>(i), static_cast<unsigned int>(j) });
								TransformComponent->SetTranslate(pos);
							}
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

						d2dFramework::ObjectManager::GetInstance()->DeleteObject(iter->second);
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
						CircleCollider* collider = object->GetComponent< CircleCollider>();

						// 오브젝트의 콜라이더가 없을 때에만 생성 콜라이더 있으면 기존 콜라이더 세팅 값으로 변경
						if (collider != nullptr)
						{
							collider->SetRadius({ static_cast<float>(MapToolGui::ColliderWidth) });
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

						// 오브젝트의 스프라이트가 없을 때에만 생성 콜라이더 있으면 기존 스프라이트 세팅 값으로 변경
						if (ComponentRenderer != nullptr)
						{
							ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
							ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
							ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
							ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
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

		if (mImGui->GetIsImageLoading())
		{
			// 비트맵을 찾고 비트맵이 있으면 종료
			auto iter = RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::wstrImageName.c_str());

			if (iter == nullptr)
			{
				// 파일 경로에 이미지 파일 있으면 생성
				HRESULT hr = RenderManager::GetInstance()->CreateD2DBitmapFromFile(MapToolGui::wstrImageName.c_str(), MapToolGui::wstrImagePath.c_str());

				if (SUCCEEDED(hr))
				{
					// 이미지 리스트에 넣을 문자열 저장
					MapToolGui::ImageListName.push_back(MapToolGui::ImageName);
					MapToolGui::WstringImageListName.push_back(MapToolGui::wstrImageName);
				}
			}
		}

		if (mImGui->GetIsImageSeleted())
		{
			// 선택 된 오브젝트 비트맵 수정
			for (auto ObjectMap : MapToolGui::mObjectIdMap)
			{
				if (MapToolGui::mbIsChecked[ObjectMap.first.first][ObjectMap.first.second] == true)
				{
					GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(ObjectMap.second);

					if (object == nullptr)
						return;

					// 비트맵으로 수정
					SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();
					ComponentRenderer->SetSpriteType(eSpriteType::Sprite);
					ComponentRenderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ItemImageCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
					ComponentRenderer->SetUVRectangle({ 0,0, 500, 500 });
				}
			}
		}
	}

	void SoulBeaterProcessor::CreateScene()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsCreateScene())
		{
			// 수정 홍지환, 새로운 그리드 생성 시 실제 id값이 바뀌도록 로직 연결
			if (MapToolScene == nullptr)
			{
				MapToolScene = getSceneManager()->CreateScene<Scene>(MapToolGui::SceneId);
				getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
			}
			else if (getSceneManager()->FindSceneOrNull(MapToolGui::SceneId) != nullptr)
			{
				MapToolScene->Exit();
				MapToolScene = getSceneManager()->FindSceneOrNull(MapToolGui::SceneId);
				getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
			}
			else
			{
				MapToolScene->Exit();
				MapToolScene = getSceneManager()->CreateScene<Scene>(MapToolGui::SceneId);
				getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
			}
		}
	}

	void SoulBeaterProcessor::SaveAndLoad()
	{
		using namespace d2dFramework;

		if (mImGui->GetIsSave())
		{
			if (MapToolScene != nullptr)
				SceneLoader::SaveScene(MapToolScene);
		}
		if (mImGui->GetIsLoad())
		{
			MapToolGui::SelectedSceneId = MapToolGui::SceneId;

			MapToolGui::mTileObjectIdMap.clear();

			//Scene* scene = getSceneManager()->GetCurrentScene();
			// 수정 : 홍지환, id값만 입력해서 load할 경우 대응
			if (MapToolScene == nullptr)
			{
				MapToolScene = getSceneManager()->CreateScene<Scene>(MapToolGui::SceneId);
				getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
			}
			else if (MapToolScene->GetId() == MapToolGui::SceneId) // 현재 씬과 같은 아이디로 로드할 경우 빠른 반환			
			{
				return;
			}
			else if (getSceneManager()->FindSceneOrNull(MapToolGui::SceneId) != nullptr)
			{
				MapToolScene->Exit();
				MapToolScene = getSceneManager()->FindSceneOrNull(MapToolGui::SceneId);
				getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
			}
			else
			{
				MapToolScene->Exit();
				MapToolScene = getSceneManager()->CreateScene<Scene>(MapToolGui::SceneId);
				getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
			}

			MapToolGui::mObjectIdMap.clear();
			MapToolGui::mTileObjectIdMap.clear();
			MapToolGui::mbIsChecked.clear();
			MapToolGui::mbIsObject.clear();
			MapToolGui::mbIsTileObject.clear();

			if (MapToolScene != nullptr)
			{
				ObjectManager::GetInstance()->Release();
				SceneLoader::LoadScene(MapToolScene);
			}

			// 수정 홍지환, 지연 생성되므로 현재 생성된 오브젝트 큐를 받아서 로드 처리
			std::queue<GameObject*> createObjectQueue = ObjectManager::GetInstance()->GetCreateObjectQueue();

			// 수정 홍지환, 비어 있는 씬을 로드했을 때는 마지막 사용한 그리드 크기를 유지하도록 처리
			if (createObjectQueue.empty())
			{
				MapToolGui::mbIsChecked.resize(MapToolGui::mGridXCount, std::vector<bool>(MapToolGui::mGridYCount, false));
				MapToolGui::mbIsObject.resize(MapToolGui::mGridXCount, std::vector<bool>(MapToolGui::mGridYCount, false));
				MapToolGui::mbIsTileObject.resize(MapToolGui::mGridXCount, std::vector<bool>(MapToolGui::mGridYCount, false));

				return;
			}

			GameObject* object = createObjectQueue.front();
			GridComponent* grid = object->GetComponent<GridComponent>();

			MapToolGui::mbIsChecked.resize(grid->GetMaxX(), std::vector<bool>(grid->GetMaxY(), false));
			MapToolGui::mbIsObject.resize(grid->GetMaxX(), std::vector<bool>(grid->GetMaxY(), false));
			MapToolGui::mbIsTileObject.resize(grid->GetMaxX(), std::vector<bool>(grid->GetMaxY(), false));

			while (!createObjectQueue.empty())
			{
				GameObject* object = createObjectQueue.front();
				createObjectQueue.pop();

				unsigned int objectId = object->GetId();

				GridComponent* grid = object->GetComponent<GridComponent>();

				if (grid != nullptr)
				{
					MapToolGui::mGridDistance = grid->GetCellDistance();
					MapToolGui::mGridXCount = grid->GetMaxX();
					MapToolGui::mGridYCount = grid->GetMaxY();


					if (objectId >= TILE_ID_START)
					{
						MapToolGui::mTileObjectIdMap.insert(std::make_pair(std::make_pair(grid->GetPosition().X, grid->GetMaxY() - grid->GetPosition().Y - 1), objectId));
						MapToolGui::mbIsTileObject[grid->GetPosition().X][grid->GetMaxY() - grid->GetPosition().Y - 1] = true;
						MapToolGui::mTileId = objectId;
					}
					else if (objectId >= ITEM_ID_START)
					{
						MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(grid->GetPosition().X, grid->GetPosition().Y), objectId));
						MapToolGui::mbIsObject[grid->GetPosition().X][grid->GetPosition().Y] = true;
						MapToolGui::mPlayerId = objectId;
					}
					else if (objectId >= MONSTER_ID_START)
					{
						MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(grid->GetPosition().X, grid->GetPosition().Y), objectId));
						MapToolGui::mbIsObject[grid->GetPosition().X][grid->GetPosition().Y] = true;
						MapToolGui::mMonsterId = objectId;
					}
					else if (objectId >= PLAYER_ID_START)
					{
						MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(grid->GetPosition().X, grid->GetPosition().Y), objectId));
						MapToolGui::mbIsObject[grid->GetPosition().X][grid->GetPosition().Y] = true;
						MapToolGui::mPlayerId = objectId;
					}
				}
			}

			MapToolGui::mTileId += OBJECT_COMPONENT_COUNT;
			MapToolGui::mPlayerId += OBJECT_COMPONENT_COUNT;
			MapToolGui::mMonsterId += OBJECT_COMPONENT_COUNT;
			MapToolGui::mItemId += OBJECT_COMPONENT_COUNT;
		}
	}
}

