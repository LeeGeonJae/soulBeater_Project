#include "gui.h"

#include "SceneManager.h"
#include "Scene.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "D3d9.lib")

#include "MapToolWinApp.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "AABBCollider.h"
#include "SpriteRenderer.h"
#include "GridComponent.h"

#include "SoulBeaterProcessor.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter);


// static 변수 초기화
MapToolGui* MapToolGui::mpInstance = nullptr;

// 씬 아이디
int  MapToolGui::SceneId = 1000;
int  MapToolGui::SelectedSceneId = 1000;

// 그리드 가로, 세로, 간격
unsigned int MapToolGui::mGridXCount = 0;
unsigned int MapToolGui::mGridYCount = 0;
unsigned int MapToolGui::mGridDistance = 0;

// 오브젝트 체크, 오브젝트 ID, 오브젝트 시퀀스 ( 미완성 )
std::vector<std::vector<bool>>		MapToolGui::mbIsChecked = {};
std::vector<std::vector<bool>>		MapToolGui::mbIsObject = {};
std::map<std::pair<int, int>, int>	MapToolGui::mObjectIdMap = {};

// Tile Bitmap 세팅
int MapToolGui::BitmapU = 120;
int MapToolGui::BitmapV = 120;
int MapToolGui::BitmapXNumber = 0;
int MapToolGui::BitmapYNumber = 0;

// 타일 오브젝트
std::vector<std::vector<bool>>		MapToolGui::mbIsTileObject = {};
std::map<std::pair<int, int>, int>	MapToolGui::mTileObjectIdMap = {};
int				MapToolGui::TileSpriteWidth = 110;
int				MapToolGui::TileSpriteHeight = 110;
int				MapToolGui::TileColliderWidth = 110;
int				MapToolGui::TileColliderHeight = 110;
int				MapToolGui::TileObjectItemCurrentIndex = 0;

// Id 세팅
IdSet MapToolGui::mIdSetting = IdSet::DEFALUE;
int MapToolGui::mPlayerId = PLAYER_ID_START;
int MapToolGui::mMonsterId = MONSTER_ID_START;
int MapToolGui::mItemId = ITEM_ID_START;
int MapToolGui::mTileId = TILE_ID_START;
d2dFramework::eObjectType MapToolGui::ObjectType = d2dFramework::eObjectType::None;

// 오브젝트 콜라이더 사이즈
bool MapToolGui::bIsColliderCheck = false;
int MapToolGui::ColliderWidth = 0;
int MapToolGui::ColliderHeight = 0;

// 스프라이트 세팅 값들
bool MapToolGui::bIsSpriteCheck = false;
d2dFramework::eSpriteType MapToolGui::SpriteType = d2dFramework::eSpriteType::Circle;
int	MapToolGui::CreateSpriteWidth = 0;
int	MapToolGui::CreateSpriteHeight = 0;
int MapToolGui::SpriteWidth = 0;
int MapToolGui::SpriteHeight = 0;
int	MapToolGui::ObjectItemCurrentIndex = 0;

// 그리드 세팅 값들
bool MapToolGui::mbIsGridCheck = true;

// 오브젝트 정보 출력 세팅 값들
int MapToolGui::ItemImfoCurrentIndex = 0;

// 이미지 파일
std::vector<std::string> MapToolGui::ImageListName = { "Tile", "Tile1", "Tile2", "Tile3", "Tile4", "Tile5"  };
std::vector<std::wstring> MapToolGui::WstringImageListName = { L"Tile", L"Tile1", L"Tile2", L"Tile3", L"Tile4", L"Tile5" };
std::wstring MapToolGui::wstrImageName = {};
std::wstring MapToolGui::wstrImagePath = {};
// ImGui 텍스트 박스에 입력될 static char
char MapToolGui::ImageName[100] = "Image Name";
char MapToolGui::ImagePath[256] = "Image Path";
int MapToolGui::ItemImageCurrentIndex = 0;

MapToolGui::MapToolGui(d2dFramework::SceneManager* sceneManager)
	:bOnbutton(false)
	, exit(true)
	, mHWnd(nullptr)
	, mWindowClass{}
	, mPosition{}
	, md3d(nullptr)
	, mDevice(nullptr)
	, mPresentParameters{}
	, mbIsCreateTileObjectButtonIstrue(false)
	, mbIsCreateTileColliderIstrue(false)
	, mbIsDeleteTileObjectButtonIstrue(false)
	, mbIsCreateObjectButtonIstrue(false)
	, mbIsDeleteObjectButtonIstrue(false)
	, mbIsColliderCreate(false)
	, mbIsSpriteCreate(false)
	, mbIsSaveButton(false)
	, mbIsCreateScene(false)
	, mSceneManager(sceneManager)
{
	mpInstance = this;
}

MapToolGui::~MapToolGui()
{
	mSceneManager = nullptr;
}



void MapToolGui::Render()
{
	using namespace d2dFramework;

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Map Tool Menu",
		&exit,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	GuiRender();

	ImGui::End();
}



void MapToolGui::GuiRender()
{
	GuiMenu();

	std::string text;
	ImGui::InputInt("SceneId : ", &SceneId);
	text = "Selected SceneId Number : " + std::to_string(SelectedSceneId);
	ImGui::Text(text.c_str());
	ImGui::Text("");

	// 격자판 생성 및 세팅
	if (ImGui::CollapsingHeader("Grid Setting & Create"))
		GridCreate();
	ImGui::Text("");

	// 타일 세팅
	if (ImGui::CollapsingHeader("Tile Setting"))
		ObjectTileSetting();
	ImGui::Text("");

	//// 오브젝트 세팅
	//if (ImGui::CollapsingHeader("Object Setting"))
	//	ObjectSetting();
	//ImGui::Text("");

	//// 오브젝트의 컴폰넌트 수정
	//if (ImGui::CollapsingHeader("Component Edit"))
	//	ComponentEdit();
	//ImGui::Text("");

	// 이미지 세팅
	if (ImGui::CollapsingHeader("Image Setting"))
		ImGuiImageLoading();
	ImGui::Text("");

	//// 오브젝트 정보 세팅
	//if (ImGui::CollapsingHeader("Object Impomation"))
	//	ObjectImfomation();
	//ImGui::Text("");

	// 오브젝트 정보 세팅
	if (ImGui::CollapsingHeader("Tile Object Impomation"))
		TileObjectImfomation();
	ImGui::Text("");
}



void MapToolGui::GuiMenu()
{
	if (ImGui::Button("Save"))
		mbIsSaveButton = true;
	else
		mbIsSaveButton = false;

	if (ImGui::Button("Load"))
		mbIsLoadButton = true;
	else
		mbIsLoadButton = false;
}

void MapToolGui::GridCreate()
{
	using namespace d2dFramework;

	static int GridArea[2] = { 10, 9 };
	static int GridInstance = 110;

	// 그리드 가로,세로 값세팅 및 그리드 사이 간격 길이 입력
	ImGui::InputInt2("Grid Area X, Y", GridArea);
	ImGui::InputInt("Grid Distance", &GridInstance);

	// 그리드 세팅 버튼을 누르면 그리드 칸에 맞춰서 세팅
	if (ImGui::Button("Grid Setting"))
	{
		mbIsCreateScene = true;							// Scene 생성

		for (int i = 0; i < mbIsChecked.size(); i++)
			mbIsChecked[i].clear();
		mbIsChecked.clear();

		for (int i = 0; i < mbIsObject.size(); i++)
			mbIsObject[i].clear();
		mbIsObject.clear();

		for (int i = 0; i < mbIsTileObject.size(); i++)
			mbIsTileObject[i].clear();
		mbIsTileObject.clear();

		mGridXCount = GridArea[0];
		mGridYCount = GridArea[1];
		mGridDistance = GridInstance;

		// 그리드에 맞게 리사이즈
		if (mGridYCount != 0 && mGridDistance != 0 && mGridXCount != 0 && mGridDistance != 0)
		{
			mbIsChecked.resize(mGridXCount, std::vector<bool>(mGridYCount, false));
			mbIsObject.resize(mGridXCount, std::vector<bool>(mGridYCount, false));
			mbIsTileObject.resize(mGridXCount, std::vector<bool>(mGridYCount, false));
		}
		else
		{
			mbIsChecked.clear();
			mbIsObject.clear();
			mbIsTileObject.clear();
		}

		// 수정 홍지환, 현재 씬이 nullptr이 아니라면 등록된 모든 오브젝트 삭제
		Scene* scene = mSceneManager->GetCurrentScene();
		SelectedSceneId = SceneId;

		if (scene != nullptr)
		{// 모든 오브젝트 삭제
			for (auto e : mObjectIdMap)
				ObjectManager::GetInstance()->DeleteObject(e.second);
			mObjectIdMap.clear();
			for (auto e : mTileObjectIdMap)				
				ObjectManager::GetInstance()->DeleteObject(e.second);
			mTileObjectIdMap.clear();
		}
	}
	else
		mbIsCreateScene = false;
}



// 오브젝트 생성 및 삭제 및 컴포넌트 세팅
void MapToolGui::ObjectSetting()
{
	static int ObjectItemCurrentIndex = 0;
	static std::string ListName[] = { "Defalut", "PlayerId", "MonsterId", "ItemId" };

	// 오브젝트 종류에 맞는 ID 시작값 세팅
	if (ImGui::BeginListBox("Id Setting"))
	{
		for (int n = 0; n < (int)IdSet::END; n++)
		{
			const bool Selected = (ObjectItemCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ObjectItemCurrentIndex = n;

				switch (ObjectItemCurrentIndex)
				{
				case 0:
					mIdSetting = IdSet::DEFALUE;
					break;
				case 1:
					mIdSetting = IdSet::PLAYERID;
					break;
				case 2:
					mIdSetting = IdSet::MONSTERID;
					break;
				case 3:
					mIdSetting = IdSet::ITEMID;
					break;
				}
			}
		}

		ImGui::EndListBox();
	}

	ImGui::Checkbox("Collider Component", &bIsColliderCheck);
	ImGui::Checkbox("Sprite Component", &bIsSpriteCheck);
	ImGui::Checkbox("Grid Component", &mbIsGridCheck);

	if (bIsColliderCheck)
	{
		ImGui::InputInt("Collider Width", &ColliderWidth);
		ImGui::InputInt("Collider Hight", &ColliderHeight);
	}
	ImGui::Text("");
	if (bIsSpriteCheck)
	{
		ObjectSpriteSetting();
	}
	ImGui::Text("");

	// 오브젝트 생성 버튼
	if (ImGui::Button("Create Object"))
		mbIsCreateObjectButtonIstrue = true;
	else
		mbIsCreateObjectButtonIstrue = false;

	// 오브젝트 삭제 버튼
	if (ImGui::Button("Delete Object"))
		mbIsDeleteObjectButtonIstrue = true;
	else
		mbIsDeleteObjectButtonIstrue = false;
}



void MapToolGui::ObjectSpriteSetting()
{
	static int ItemSpriteCurrentIndex = 0;
	static std::vector<std::string> ListName = { "Rectangle", "Circle", "Sprite" };

	// 스프라이트 타입 리스트
	if (ImGui::BeginListBox("SpriteType List"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ItemSpriteCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ItemSpriteCurrentIndex = n;

				switch (ItemSpriteCurrentIndex)
				{
				case 0:
					SpriteType = d2dFramework::eSpriteType::Rectangle;
					break;
				case 1:
					SpriteType = d2dFramework::eSpriteType::Circle;
					break;
				case 2:
					SpriteType = d2dFramework::eSpriteType::Sprite;
					break;
				}
			}
		}
		ImGui::EndListBox();
	}

	// 타일 Bitmap 선택
	if (ImGui::BeginListBox("Sprite Bitmap Setting"))
	{
		for (int n = 0; n < ImageListName.size(); n++)
		{
			const bool Selected = (ObjectItemCurrentIndex == n);
			if (ImGui::Selectable(ImageListName[n].c_str(), Selected))
			{
				ObjectItemCurrentIndex = n;
			}
		}

		ImGui::EndListBox();
	}

	// 스프라이트 가로, 세로 길이 세팅
	ImGui::InputInt("Sprite Width", &CreateSpriteWidth);
	ImGui::InputInt("Sprite Height", &CreateSpriteHeight);
}



void MapToolGui::ObjectTileSetting()
{
	ImGui::InputInt("Tile Sprite Width", &TileSpriteWidth);
	ImGui::InputInt("Tile Sprite Height", &TileSpriteHeight);
	ImGui::Text("");

	ImGui::InputInt("Tile BitmapU Coordinate", &BitmapU);
	ImGui::InputInt("Tile BitmapV Coordinate", &BitmapV);
	ImGui::InputInt("Tile BitmapNumber \'X\'", &BitmapXNumber);
	ImGui::InputInt("Tile BitmapNumber \'Y\'", &BitmapYNumber);
	ImGui::Text("");

	ImGui::Checkbox("TileCollider Is Valid", &mbIsCreateTileColliderIstrue);
	ImGui::Text("");

	// 타일 Bitmap 선택
	if (ImGui::BeginListBox("Tile Bitmap Setting"))
	{
		for (int n = 0; n < ImageListName.size(); n++)
		{
			const bool Selected = (TileObjectItemCurrentIndex == n);
			if (ImGui::Selectable(ImageListName[n].c_str(), Selected))
			{
				TileObjectItemCurrentIndex = n;
			}
		}

		ImGui::EndListBox();
	}

	// 타일 오브젝트 생성 버튼
	if (ImGui::Button("Create Tile Object"))
		mbIsCreateTileObjectButtonIstrue = true;
	else
		mbIsCreateTileObjectButtonIstrue = false;

	// 타일 오브젝트 삭제 버튼
	if (ImGui::Button("Delete Tile Object"))
		mbIsDeleteTileObjectButtonIstrue = true;
	else
		mbIsDeleteTileObjectButtonIstrue = false;
}



// 콜라이더 세팅
void MapToolGui::ColliderSetting()
{
	// 콜라이더 가로 세로 세팅
	ImGui::InputInt("Collider Width", &ColliderWidth);
	ImGui::InputInt("Collider Hight", &ColliderHeight);

	// 콜라이더 생성
	if (ImGui::Button("Create Collider"))
		mbIsColliderCreate = true;
	else
		mbIsColliderCreate = false;

	ImGui::Text("");
	ImGui::TreePop();
}



void MapToolGui::SpriteSetting()
{
	static int ItemSpriteCurrentIndex = 0;
	static std::vector<std::string> ListName = { "Rectangle", "Circle", "Sprite" };

	// 스프라이트 타입 리스트
	if (ImGui::BeginListBox("SpriteType List"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ItemSpriteCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ItemSpriteCurrentIndex = n;

				switch (ItemSpriteCurrentIndex)
				{
				case 0:
					SpriteType = d2dFramework::eSpriteType::Rectangle;
					break;
				case 1:
					SpriteType = d2dFramework::eSpriteType::Circle;
					break;
				case 2:
					SpriteType = d2dFramework::eSpriteType::Sprite;
					break;
				}
			}
		}
		ImGui::EndListBox();
	}

	// 타일 Bitmap 선택
	if (ImGui::BeginListBox("Sprite Bitmap Setting"))
	{
		for (int n = 0; n < ImageListName.size(); n++)
		{
			const bool Selected = (ObjectItemCurrentIndex == n);
			if (ImGui::Selectable(ImageListName[n].c_str(), Selected))
			{
				ObjectItemCurrentIndex = n;
			}
		}

		ImGui::EndListBox();
	}

	// 스프라이트 가로, 세로 길이 세팅
	ImGui::InputInt("Sprite Width", &SpriteWidth);
	ImGui::InputInt("Sprite Height", &SpriteHeight);

	// 스프라이트 생성
	if (ImGui::Button("Create Sprite"))
		mbIsSpriteCreate = true;
	else
		mbIsSpriteCreate = false;

	ImGui::TreePop();
}



void MapToolGui::ComponentEdit()
{
	// 콜라이더 컴포넌트 세팅
	if (ImGui::TreeNode("Collider Setting"))
		ColliderSetting();

	// 스프라이트 컴포넌트 세팅
	if (ImGui::TreeNode("Sprite Setting"))
		SpriteSetting();
}

void MapToolGui::ObjectImfomation()
{
	using namespace d2dFramework;

	static std::vector<std::string> ListName;
	static int ObjectListCount = 0;
	std::vector<int> ListId;

	ListName.clear();
	ListId.clear();
	ObjectListCount = 0;

	// 해당 오브젝트 Id값 및 오브젝트 종류 출력 정보
	for (int i = 0; i < mbIsChecked.size(); i++)
	{
		for (int j = 0; j < mbIsChecked[i].size(); j++)
		{
			if (mbIsChecked[i][j] == true && mbIsObject[i][j] == true)
			{
				std::pair<int, int> key = std::make_pair(i, j);
				auto iter = mObjectIdMap.find(key);

				std::string idlist;

				if (iter->second >= ITEM_ID_START)
				{
					idlist = "Item Object " + std::to_string(ObjectListCount);
					ObjectListCount++;
				}
				else if (iter->second >= MONSTER_ID_START)
				{
					idlist = "Monster Object " + std::to_string(ObjectListCount);
					ObjectListCount++;
				}
				else if (iter->second >= PLAYER_ID_START)
				{
					idlist = "Player Object " + std::to_string(ObjectListCount);
					ObjectListCount++;
				}

				ListName.push_back(idlist);
				ListId.push_back(iter->second);
			}
		}
	}

	// 오브젝트 정보 리스트 목록
	if (ImGui::BeginListBox("Object imfomartion List"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ItemImfoCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ItemImfoCurrentIndex = n;
			}
		}
		ImGui::EndListBox();
	}

	// 선택된 오브젝트를 찾고 해당 오브젝트의 정보 출력
	if (ListId.size() != 0)
	{
		GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(ListId[ItemImfoCurrentIndex]);

		// 수정 : 홍지환, 지연 생성으로 인해 인포창 켜고 생성 시 터짐 
		if (object == nullptr)
		{
			return;
		}

		Transform* TransformComponent = object->GetComponent<Transform>();
		SpriteRenderer* SpriteComponent = object->GetComponent<SpriteRenderer>();
		AABBCollider* ColliderComponent = object->GetComponent<AABBCollider>();

		std::string text = ListName[ItemImfoCurrentIndex] + ", Id : " + std::to_string(ListId[ItemImfoCurrentIndex]);
		ImGui::Text(text.c_str());
		text.clear();

		// 트랜스폼 위치 값 출력
		if (TransformComponent != nullptr)
		{
			ImGui::Text("Transform Is Valid");
			text = "Location X : " + std::to_string(TransformComponent->GetTransform().dx);
			ImGui::Text(text.c_str());
			text.clear();
			text = "Location Y : " + std::to_string(TransformComponent->GetTransform().dy);
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("Transform Is not Valid");

		// 스프라이트 정보 출력
		if (SpriteComponent != nullptr)
		{
			ImGui::Text("SpriteComponent Is Valid");
			if (SpriteComponent->GetBitmap() != nullptr)
				ImGui::Text("Bitmap Is Valid");
			else
				ImGui::Text("Bitmap Is not Valid");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Rectangle)
				ImGui::Text("SpriteType Is Rectangle");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Circle)
				ImGui::Text("SpriteType Is Circle");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Sprite)
				ImGui::Text("SpriteType Is Sprite");
			text = "Size X : " + std::to_string(SpriteComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
			text = "Size Y : " + std::to_string(SpriteComponent->GetSize().GetY());
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("SpriteComponnet Is not Valid");

		// 콜라이더 정보 출력 ( AABB ) 
		if (ColliderComponent != nullptr)
		{
			ImGui::Text("ColliderComponent Is Valid");
			ImGui::Text("ColliderComponent Type Is AABBCollider");
			text = "Size X : " + std::to_string(ColliderComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
			text = "Size X : " + std::to_string(ColliderComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("ColliderComponent Is not Valid");
	}

}

void MapToolGui::TileObjectImfomation()
{
	using namespace d2dFramework;

	static std::vector<std::string> ListName;
	static int TileObjectListCount = 0;
	std::vector<int> ListId;

	ListName.clear();
	ListId.clear();
	TileObjectListCount = 0;

	// 해당 오브젝트 Id값 및 오브젝트 종류 출력 정보
	for (int i = 0; i < mbIsChecked.size(); i++)
	{
		for (int j = 0; j < mbIsChecked[i].size(); j++)
		{
			if (mbIsChecked[i][j] == true && mbIsTileObject[i][j] == true)
			{
				std::pair<int, int> key = std::make_pair(i, j);
				auto iter = mTileObjectIdMap.find(key);

				std::string idlist;

				idlist = "Tile Object " + std::to_string(TileObjectListCount);
				TileObjectListCount++;

				ListName.push_back(idlist);
				ListId.push_back(iter->second);
			}
		}
	}

	// 오브젝트 정보 리스트 목록
	if (ImGui::BeginListBox("Tile Object imfomartion List"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ItemImfoCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ItemImfoCurrentIndex = n;
			}
		}
		ImGui::EndListBox();
	}

	// 선택된 오브젝트를 찾고 해당 오브젝트의 정보 출력
	if (ListId.size() != 0)
	{
		GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(ListId[ItemImfoCurrentIndex]);

		// 수정 : 홍지환, 인포메이션 창 켠 채로 타일 오브젝트 생성 시 지연 생성으로 인해 터짐
		if (object == nullptr)
		{
			return;
		}

		Transform* TransformComponent = object->GetComponent<Transform>();
		SpriteRenderer* SpriteComponent = object->GetComponent<SpriteRenderer>();
		AABBCollider* ColliderComponent = object->GetComponent<AABBCollider>();
		GridComponent* Grid = object->GetComponent<GridComponent>();

		std::string text = ListName[ItemImfoCurrentIndex] + ", Id : " + std::to_string(ListId[ItemImfoCurrentIndex]);
		ImGui::Text(text.c_str());
		text.clear();

		// 트랜스폼 위치 값 출력
		if (TransformComponent != nullptr)
		{
			ImGui::Text("Transform Is Valid");
			text = "Location X : " + std::to_string(TransformComponent->GetTransform().dx);
			ImGui::Text(text.c_str());
			text.clear();
			text = "Location Y : " + std::to_string(TransformComponent->GetTransform().dy);
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("Transform Is not Valid");

		// 스프라이트 정보 출력
		if (SpriteComponent != nullptr)
		{
			ImGui::Text("SpriteComponent Is Valid");
			if (SpriteComponent->GetBitmap() != nullptr)
				ImGui::Text("Bitmap Is Valid");
			else
				ImGui::Text("Bitmap Is not Valid");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Rectangle)
				ImGui::Text("SpriteType Is Rectangle");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Circle)
				ImGui::Text("SpriteType Is Circle");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Sprite)
				ImGui::Text("SpriteType Is Sprite");
			text = "Size X : " + std::to_string(SpriteComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
			text = "Size Y : " + std::to_string(SpriteComponent->GetSize().GetY());
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
		{
			ImGui::Text("SpriteComponnet Is not Valid");
		}

		// 콜라이더 정보 출력 ( AABB ) 
		if (ColliderComponent != nullptr)
		{
			ImGui::Text("ColliderComponent Is Valid");
			ImGui::Text("ColliderComponent Type Is AABBCollider");
			text = "Size X : " + std::to_string(ColliderComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
			text = "Size X : " + std::to_string(ColliderComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
		{
			ImGui::Text("ColliderComponent Is not Valid");
		}

		if (Grid != nullptr)
		{
			ImGui::Text("Grid Is Valid");
			text = "Grid X : " + std::to_string(Grid->GetPosition().X);
			ImGui::Text(text.c_str());
			text.clear();
			text = "Grid Y : " + std::to_string(Grid->GetPosition().Y);
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
		{
			ImGui::Text("Grid Is not Valid");
		}
	}
}



void MapToolGui::ImGuiImageLoading()
{
	using namespace d2dFramework;

	static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

	// 텍스트 박스 생성
	ImGui::InputTextMultiline("##source1", ImageName, IM_ARRAYSIZE(ImageName), ImVec2(100.f, 23.f), flags);
	ImGui::InputTextMultiline("##source2", ImagePath, IM_ARRAYSIZE(ImagePath), ImVec2(-FLT_MIN, 23.f), flags);

	if (ImGui::Button("Image Create Button"))
	{
		mbIsImageLoading = true;

		// 로케일 설정
		std::locale loc("");

		// char 배열을 wchar_t 배열로 변환
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		wstrImageName = converter.from_bytes(ImageName);
		wstrImagePath = converter.from_bytes(ImagePath);
	}
	else
		mbIsImageLoading = false;
	ImGui::Text("");

	// 이미지 리스트 목록
	if (ImGui::BeginListBox("Image List"))
	{
		for (int n = 0; n < ImageListName.size(); n++)
		{
			const bool Selected = (ItemImageCurrentIndex == n);
			if (ImGui::Selectable(ImageListName[n].c_str(), Selected))
			{
				ItemImageCurrentIndex = n;
				mbIsImageSelected = true;
			}
			else
				mbIsImageSelected = false;
		}
		ImGui::EndListBox();
	}
}




void MapToolGui::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = mDevice->Reset(&mPresentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void MapToolGui::DestroyDevice()
{
	if (mDevice)
	{
		mDevice->Release();
		mDevice = nullptr;
	}

	if (md3d)
	{
		md3d->Release();
		md3d = nullptr;
	}
}

// handle ImGui creation & destruction
void MapToolGui::CreateImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(mHWnd);
	ImGui_ImplDX9_Init(mDevice);
}

void MapToolGui::DestroyImGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void MapToolGui::BeginRender()
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Start ther Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MapToolGui::EndRender()
{
	ImGui::EndFrame();

	mDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	mDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (mDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		mDevice->EndScene();
	}

	const auto result = mDevice->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && mDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

// handle window creation & destruction
void MapToolGui::CreateHWindow(const char* windowName, const char* className)
{
	mWindowClass.cbSize = sizeof(WNDCLASSEXA);
	mWindowClass.style = CS_CLASSDC;
	mWindowClass.lpfnWndProc = WindowProcess;
	mWindowClass.cbClsExtra = 0;
	mWindowClass.cbWndExtra = 0;
	mWindowClass.hInstance = GetModuleHandleA(0);
	mWindowClass.hIcon = 0;
	mWindowClass.hCursor = 0;
	mWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	mWindowClass.lpszMenuName = 0;
	mWindowClass.lpszClassName = "Gui";
	mWindowClass.hIconSm = 0;

	RegisterClassExA(&mWindowClass);

	mHWnd = CreateWindowW(
		L"Gui",
		L"ImGui",
		WS_POPUP,
		1000,
		200,
		WIDTH,
		HEIGHT,
		0,
		0,
		mWindowClass.hInstance,
		0
	);

	ShowWindow(mHWnd, SW_SHOWDEFAULT);
	UpdateWindow(mHWnd);
}

void MapToolGui::DestroyHWindow()
{
	DestroyWindow(mHWnd);
	UnregisterClass(L"Gui", mWindowClass.hInstance);
}

// handle device creation & destruction
bool MapToolGui::CreateDevice()
{
	md3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!md3d)
		return false;

	ZeroMemory(&mPresentParameters, sizeof(mPresentParameters));

	mPresentParameters.Windowed = TRUE;
	mPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	mPresentParameters.EnableAutoDepthStencil = TRUE;
	mPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	mPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// 디스플레이 어댑터를 나타내는 디바이스를 만드는 함수입니다.
	if (md3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		mHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&mPresentParameters,
		&mDevice) < 0)
		return false;

	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE:
	{
		if (MapToolGui::mpInstance->GetDevice() && wideParameter != SIZE_MINIMIZED)
		{
			MapToolGui::mpInstance->SetBackBufferWidth(LOWORD(longParameter), HIWORD(longParameter));
			MapToolGui::mpInstance->ResetDevice();
		}
	}
	return 0;

	case WM_SYSCOMMAND:
	{
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		//MapToolGui::mpInstance->GetGuiPosition() = MAKEPOINTS(longParameter);	// set click points;
		MapToolGui::mpInstance->SetClickPoint(longParameter);	// set click points;
	}
	return 0;

	case WM_MOUSEMOVE:
	{
		// 창이동?
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{};

			GetWindowRect(MapToolGui::mpInstance->GetHwnd(), &rect);

			rect.left += points.x - MapToolGui::mpInstance->GetGuiPosition().x;
			rect.top += points.y - MapToolGui::mpInstance->GetGuiPosition().y;

			if (MapToolGui::mpInstance->GetGuiPosition().x >= 0 &&
				MapToolGui::mpInstance->GetGuiPosition().x <= MapToolGui::mpInstance->WIDTH &&
				MapToolGui::mpInstance->GetGuiPosition().y >= 0 && MapToolGui::mpInstance->GetGuiPosition().y <= 19)
				SetWindowPos(
					MapToolGui::mpInstance->GetHwnd(),
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}
	}
	}

	return DefWindowProcW(window, message, wideParameter, longParameter);
}
