#include "SoulBeaterScene.h"

#include "SceneLoader.h"

namespace d2dFramework
{
	SoulBeaterScene::SoulBeaterScene(unsigned int id)
		:Scene(id)
	{
	}

	void SoulBeaterScene::Enter()
	{
		SceneLoader::LoadScene(this); //타일맵 로드
		// 타일맵 아이디 세팅 -> 

		switch (mSceceType)
		{
		case eSceneType::Start:
			enterStart(); 
			break;
		case eSceneType::Stage1_1:
			break;
		case eSceneType::Stage1_2:
			break;
		case eSceneType::Stage1_3:
			break;
		case eSceneType::CutScene1:
			break;
		case eSceneType::Stage2_1:
			break;
		case eSceneType::Stage2_2:
			break;
		case eSceneType::Stage2_3:
			break;
		case eSceneType::CutScene2:
			break;
		case eSceneType::Stage3_1:
			break;
		case eSceneType::Stage3_2:
			break;
		case eSceneType::Stage3_3:
			break;
		case eSceneType::CutScene3:
			break;
		case eSceneType::Ending:
			break;
		default:
			break;
		}
	}

	void SoulBeaterScene::Exit()
	{
		Scene::Exit();
	}

	void SoulBeaterScene::SerializeIn(nlohmann::ordered_json& object)
	{
		//mSceneType = object["InitButtonManagerType"];
		Scene::SerializeIn(object);
	}

	void SoulBeaterScene::SerializeOut(nlohmann::ordered_json& object)
	{
		//object["InitButtonManagerType"] = mSceneType;
		Scene::SerializeOut(object);
	}
}
