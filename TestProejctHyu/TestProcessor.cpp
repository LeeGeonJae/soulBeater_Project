#include "TestProcessor.h"

#include "SceneManager.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "SceneLoader.h"
#include "AABBCollider.h"
#include "Rigidbody.h"
#include "SceneLoader.h"
#include "RenderManger.h"

namespace soulBeater
{
	SoulBeaterProcessor::SoulBeaterProcessor(UINT width, UINT height, std::wstring name)
		: GameProcessor(width, height, name)
	{
	}

	void SoulBeaterProcessor::Init()
	{
		using namespace d2dFramework;

		static unsigned int tempId = 20000;

		GameProcessor::Init();
		getSceneManager()->CreateScene(1004);
		getSceneManager()->SetCurrentScene(1004);
		Scene* a = new Scene(10001);
		Scene* b = new Scene(10002);
		Scene* c = new Scene(10003);
		Scene* d = new Scene(10004);
		Scene* e = new Scene(10005);
		Scene* f = new Scene(10006);
		
		SceneLoader::LoadScene(a);
		SceneLoader::LoadScene(b);
		SceneLoader::LoadScene(c);
		SceneLoader::LoadScene(d);
		SceneLoader::LoadScene(f);
		SceneLoader::LoadScene(a);
		SceneLoader::SaveScene(a);
		SceneLoader::SaveScene(b);
		SceneLoader::SaveScene(c);
		SceneLoader::SaveScene(d);
		SceneLoader::SaveScene(e);
		SceneLoader::SaveScene(f);
		delete a;
		delete b;
		delete c;
		delete d;
		delete e;
		delete f;
	}

	void SoulBeaterProcessor::Update()
	{
		using namespace d2dFramework;
		getRenderManager()->BeginDraw();
		{
			GameProcessor::Update();

		}
		getRenderManager()->EndDraw();
		getRenderManager()->Present();
	}

	void SoulBeaterProcessor::Release()
	{
		using namespace d2dFramework;

		GameProcessor::Release();
	}


	void SoulBeaterProcessor::initAnimationAsset()
	{
		using namespace d2dFramework;

		SceneLoader::LoadAllAnimationAssets(getRenderManager());
	}
}