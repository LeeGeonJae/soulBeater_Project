#include "SoulBeaterProcessor.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "SpriteRenderer.h"
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
		// register Register Scene;

		GameObject* gm = ObjectManager::GetInstance()->CreateObject(tempId++);
		gm->CreateComponent<Transform>(tempId++);
		SpriteRenderer* renderer = gm->CreateComponent<SpriteRenderer>(tempId++);

		getRenderManager()->CreateD2DBitmapFromFile(L"enemy.png");

		renderer->SetSize({ 100, 100 });
		renderer->SetSpriteType(eSpriteType::Rectangle);
		renderer->SetBitmap(getRenderManager()->GetBitmapOrNull(L"enemy.png"));  
	}

	void SoulBeaterProcessor::Release()
	{
		using namespace d2dFramework;

		GameProcessor::Release();
	}
}