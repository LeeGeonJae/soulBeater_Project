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
#include"SceneLoader.h"
#include "gui.h"

namespace mapTool
{
	SoulBeaterProcessor::SoulBeaterProcessor(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
		, mRect{}
	{
	}

	void SoulBeaterProcessor::Init()
	{
		const int canvasWidth = GetWidth() / 80, canvasHeight = GetHeight() / 80;

		using namespace d2dFramework;

		getRenderManager()->SetHwnd(MapToolWinApp::GetHwnd());
		GameProcessor::Init();
		getSceneManager()->CreateScene(50000);
		// register Register Scene;

		mImGui->CreateHWindow("Map Tool Menu", "Map Tool Class");
		mImGui->CreateDevice();
		mImGui->CreateImGui();

	/*	HRESULT hr = getRenderManager()->CreateD2DBitmapFromFile(L"Golem", L"../Bin/image/Golem.png");
		hr = getRenderManager()->CreateD2DBitmapFromFile(L"Charactor", L"../Bin/image/Charactor.png");*/

		SceneLoader::LoadAllBitmaps(getRenderManager());
	}

	void SoulBeaterProcessor::Update()
	{
		using namespace d2dFramework;

		// ���� ���� ���� ó���� �ش� �ε�Ʈ �ȿ��� ó���ϵ��� �ؾ��Ѵ�.
		getRenderManager()->BeginDraw(); // ��� ������ �ʱ�ȭ
		{
			MouseClickCheck();
			GameProcessor::Update(); // ��� �����ӿ�ũ ������ �������� ������ ���ӷ��� 
			Render();
		}
		getRenderManager()->EndDraw();

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
		getRenderManager()->DrawPoint((float)pos.x, (float)pos.y);

		// ������ �׸���
		{
			CreateScene();
			getRenderManager()->DrawGrid(0, 0, (float)MapToolGui::mWidth, (float)MapToolGui::mHeight, (float)MapToolGui::mGridDistance);
			CheckedRender();
			ColliderRender();
		}

		// Ÿ�� ������Ʈ ����
		{
			TileObjectCreate();
			TileObjectDelete();

			TileColliderCreate();
		}

		// ������Ʈ ���� ( ������Ʈ ���� ) �� ����
		{
			ObjectCreate();
			ColliderSetting();
			SpriteSetting();
			ObjectDelete();
		}

		// ���̺�
		{
			SaveAndLoad();
		}

		getRenderManager()->SetStrokeWidth(2);
	}

	void SoulBeaterProcessor::MouseClickCheck()
	{
		using namespace d2dFramework;

		// ��ư Ŭ�� �� �׸��忡 �´� �� üũ
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
							(float)i * MapToolGui::mGridDistance - collider->GetSize().GetX() * 0.5f + MapToolGui::mGridDistance * 0.5f
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
					//üũ�� �Ǿ����鼭 ������Ʈ�� ���� ���� ������Ʈ ����
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == false)
					{
						//������Ʈ ����
						Vector2 pos =
						{
							i * static_cast<float>(MapToolGui::mGridDistance) - GetWidth() * 0.5f + MapToolGui::mGridDistance * 0.5f,
							j * -static_cast<float>(MapToolGui::mGridDistance) + GetHeight() * 0.5f - MapToolGui::mGridDistance * 0.5f
						};

						GameObject* Object = getSceneManager()->GetCurrentScene().CreateObject(MapToolGui::mTileId);
						Object->SetObjectType(eObjectType::Background);
						MapToolGui::mTileObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mTileId++));
						MapToolGui::mbIsTileObject[i][j] = true;

						Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mTileId++);
						TransformComponent->SetTranslate(pos);
						SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mTileId++);
						ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
						ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::TileSpriteWidth), static_cast<float>(MapToolGui::TileSpriteHeight) });
						ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
 						ComponentRenderer->SetSpriteType(eSpriteType::Sprite);
						ComponentRenderer->SetBaseColor({ 1,0,0,1 });

						if (mImGui->GetIsCreateTileCollider())
						{
							AABBCollider* collider = Object->CreateComponent<AABBCollider>(MapToolGui::mTileId++);
							collider->SetSize({ static_cast<float>(MapToolGui::mGridDistance), static_cast<float>(MapToolGui::mGridDistance) });
						}
					}
					// ������Ʈ�� ������ �Ǿ� �ִٸ� ��Ʈ�ʸ� ����
					else if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsTileObject[i][j] == true)
					{
						std::pair<int, int> key = std::make_pair(i, j);
						auto iter = MapToolGui::mTileObjectIdMap.find(key);

						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);

						if (object != nullptr)
						{
							SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();
							ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ItemImageCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
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

					// �ش� Ÿ���� üũ�ǰ� Ÿ�� ������Ʈ�� ���� ��
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
		// Ÿ�� ������Ʈ ����
		if (mImGui->GetIsDeleteTileObject())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mTileObjectIdMap.find(key);

					// üũ�Ǿ� �ְ� ������Ʈ�� �ְ� Id�� ���� ���� ������Ʈ ����
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
					//üũ�� �Ǿ����鼭 ������Ʈ�� ���� ���� ������Ʈ ����
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsObject[i][j] == false)
					{
						//������Ʈ ����
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
							GameObject* Object = getSceneManager()->GetCurrentScene().CreateObject(MapToolGui::mPlayerId);
							Object->SetObjectType(eObjectType::Player);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mPlayerId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mPlayerId++);
							TransformComponent->SetTranslate(pos);
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mPlayerId++);
							AABBCollider* collider = Object->CreateComponent<AABBCollider>(MapToolGui::mPlayerId++);

							if (MapToolGui::bIsSpriteCheck)
							{
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::CreateSpriteWidth), static_cast<float>(MapToolGui::CreateSpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
								ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							if (MapToolGui::bIsColliderCheck)
							{
								collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
							}
						}
						break;
						case IdSet::MONSTERID:
						{
							GameObject* Object = getSceneManager()->GetCurrentScene().CreateObject(MapToolGui::mMonsterId);
							Object->SetObjectType(eObjectType::Enemy);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mMonsterId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mMonsterId++);
							TransformComponent->SetTranslate(pos);
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mMonsterId++);
							AABBCollider* collider = Object->CreateComponent<AABBCollider>(MapToolGui::mMonsterId++);

							if (MapToolGui::bIsSpriteCheck)
							{
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::CreateSpriteWidth), static_cast<float>(MapToolGui::CreateSpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
								ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							if (MapToolGui::bIsColliderCheck)
							{
								collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
							}
						}
						break;
						case IdSet::ITEMID:
						{
							GameObject* Object = getSceneManager()->GetCurrentScene().CreateObject(MapToolGui::mItemId);
							Object->SetObjectType(eObjectType::Item);
							MapToolGui::mObjectIdMap.insert(std::make_pair(std::make_pair(i, j), MapToolGui::mItemId++));
							MapToolGui::mbIsObject[i][j] = true;

							Transform* TransformComponent = Object->CreateComponent<Transform>(MapToolGui::mItemId++);
							TransformComponent->SetTranslate(pos);
							SpriteRenderer* ComponentRenderer = Object->CreateComponent<SpriteRenderer>(MapToolGui::mItemId++);
							AABBCollider* collider = Object->CreateComponent<AABBCollider>(MapToolGui::mItemId++);

							if (MapToolGui::bIsSpriteCheck)
							{
								ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::CreateSpriteWidth), static_cast<float>(MapToolGui::CreateSpriteHeight) });
								ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
								ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
								ComponentRenderer->SetUVRectangle({ 0,0,500,500 });
								ComponentRenderer->SetBaseColor({ 1,0,0,1 });
							}
							if (MapToolGui::bIsColliderCheck)
							{
								collider->SetSize({ static_cast<float>(MapToolGui::ColliderWidth), static_cast<float>(MapToolGui::ColliderHeight) });
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
		// ������Ʈ ����
		if (mImGui->GetIsDeleteObject())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mObjectIdMap.find(key);

					// üũ�Ǿ� �ְ� ������Ʈ�� �ְ� Id�� ���� ���� ������Ʈ ����
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

					// �ش� �迭�� üũ�Ǿ� �ְ� ������Ʈ�� �ְ� ������Ʈ ���̵� ���� �� ����
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsChecked[i][j] == true && iter != MapToolGui::mObjectIdMap.end())
					{
						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);
						AABBCollider* collider = object->GetComponent< AABBCollider>();

						// ������Ʈ�� �ݶ��̴��� ���� ������ ���� �ݶ��̴� ������ ���� �ݶ��̴� ���� ������ ����
						if (collider != nullptr)
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

		// �ش� ������Ʈ�� ��������Ʈ ������Ʈ ����
		if (mImGui->GetIsCreateSprite())
		{
			for (int i = 0; i < MapToolGui::mbIsChecked.size(); i++)
			{
				for (int j = 0; j < MapToolGui::mbIsChecked[i].size(); j++)
				{
					std::pair<int, int> key = std::make_pair(i, j);
					auto iter = MapToolGui::mObjectIdMap.find(key);

					// �ش� �迭�� üũ�Ǿ� �ְ� ������Ʈ�� �ְ� ������Ʈ ���̵� ���� �� ����
					if (MapToolGui::mbIsChecked[i][j] == true && MapToolGui::mbIsChecked[i][j] == true && iter != MapToolGui::mObjectIdMap.end())
					{
						GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(iter->second);
						SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();

						// ������Ʈ�� ��������Ʈ�� ���� ������ ���� �ݶ��̴� ������ ���� ��������Ʈ ���� ������ ����
						if (ComponentRenderer != nullptr)
						{
							ComponentRenderer->SetSize({ static_cast<float>(MapToolGui::SpriteWidth), static_cast<float>(MapToolGui::SpriteHeight) });
							ComponentRenderer->SetSpriteType(MapToolGui::SpriteType);
							ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ObjectItemCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
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
			// ��Ʈ���� ã�� ��Ʈ���� ������ ����
			auto iter = getRenderManager()->GetBitmapOrNull(MapToolGui::wstrImageName.c_str());

			if (iter == nullptr)
			{
				// ���� ��ο� �̹��� ���� ������ ����
				HRESULT hr = getRenderManager()->CreateD2DBitmapFromFile(MapToolGui::wstrImageName.c_str(), MapToolGui::wstrImagePath.c_str());

				if (SUCCEEDED(hr))
				{
					// �̹��� ����Ʈ�� ���� ���ڿ� ����
					MapToolGui::ImageListName.push_back(MapToolGui::ImageName);
					MapToolGui::WstringImageListName.push_back(MapToolGui::wstrImageName);
				}
			}
		}

		if (mImGui->GetIsImageSeleted())
		{
			// ���� �� ������Ʈ ��Ʈ�� ����
			for (auto ObjectMap : MapToolGui::mObjectIdMap)
			{
				if (MapToolGui::mbIsChecked[ObjectMap.first.first][ObjectMap.first.second] == true)
				{
					GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(ObjectMap.second);

					if (object == nullptr)
						return;

					// ��Ʈ������ ����
					SpriteRenderer* ComponentRenderer = object->GetComponent<SpriteRenderer>();
					ComponentRenderer->SetSpriteType(eSpriteType::Sprite);
					ComponentRenderer->SetBitmap(getRenderManager()->GetBitmapOrNull(MapToolGui::WstringImageListName[MapToolGui::ItemImageCurrentIndex].c_str()), MapToolGui::WstringImageListName[MapToolGui::TileObjectItemCurrentIndex]);
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
			MapToolScene = getSceneManager()->CreateScene(MapToolGui::SceneId);
			getSceneManager()->SetCurrentScene(MapToolGui::SceneId);
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
			if (MapToolScene != nullptr)
				SceneLoader::LoadScene(MapToolScene);
		}
	}
}

