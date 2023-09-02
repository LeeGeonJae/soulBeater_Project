#include "TestProcessor.h"

#include "../SoulBeater/SoulBeaterScene.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "GameObject.h"
#include "IncludeManager.h"
#include "IncludeComponent.h"
#include "SceneLoader.h"
#include "RenderManger.h"
#include "TimeManager.h"
#include "UIManager.h"


namespace TestProjectLDH
{
	TestProjectLDH::TestProjectLDH(UINT width, UINT height, std::wstring name)
		:GameProcessor(width, height, name)
	, mUIManager(new d2dFramework::UIManager())
	{
	}

	TestProjectLDH::~TestProjectLDH()
	{
		delete mUIManager;
		mUIManager = nullptr;
	}

	void TestProjectLDH::Init(HWND hwnd)
	{
		using namespace d2dFramework;
		GameProcessor::Init(hwnd);
		SceneLoader::LoadAllBitmaps();
		mUIManager->Init(getSceneManager());

		// 코드 생각해보기
		soulBeater::SoulBeaterScene* temp = getSceneManager()->CreateScene<soulBeater::SoulBeaterScene>(700);
		//getSceneManager()->CreateScene<soulBeater::SoulBeaterScene>(701);
		//Scene* test=getSceneManager()->CreateScene<soulBeater::SoulBeaterScene>(702);
		//Scene* test2 = getSceneManager()->CreateScene<soulBeater::SoulBeaterScene>(703);

		getSceneManager()->SetCurrentScene(700);
		getSceneManager()->GetCurrentScene()->Enter();

	}

	void TestProjectLDH::Update()
	{
		using namespace d2dFramework;

		//auto cc = ObjectManager::GetInstance()->GetValidObjectMaps();
		RenderManager::GetInstance() ->BitmapBeginDraw();
		RenderManager::GetInstance()->Clear(D2D1::Matrix3x2F::Identity(), { 1,1,1,0.5 });
		{
			GameProcessor::Update();
			mUIManager->Update(getTimeManager()->GetDeltaTime());
		}
		RenderManager::GetInstance()->BitmapEndDraw();

		RenderManager::GetInstance()->BeginDraw();
		RenderManager::GetInstance()->CopyBitmapRenderToHwndRender();
		RenderManager::GetInstance()->EndDraw();
	}

	void TestProjectLDH::Release()
	{
		using namespace d2dFramework;

		GameProcessor::Release();
	}


	void TestProjectLDH::initAnimationAsset()
	{
		using namespace d2dFramework;
		SceneLoader::LoadAllAnimationAssets();
	}
}
