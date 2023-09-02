#pragma once

#include "BossState.h"
#include "GridPosition.h"

namespace d2dFramework
{
	class GameObject;
	class AnimationAsset;
	class BeatManager;
}

namespace soulBeater
{
	struct GridInformantion
	{
		unsigned int CellDistnace;
		unsigned int MaxX;
		unsigned int MaxY;
		unsigned int X;
		unsigned int Y;
		float MoveSpeed;
	};

	enum class eCutSceneObjectType
	{
		CutScene1,
		CutScene2,
		CutScene3,
		EndingCreding,
		GameOver
	};

	class ObjectHelper
	{
	public:
		static d2dFramework::GameObject* CreatePlayer(const d2dFramework::GridPosition& position = { 3, 3 });
		static d2dFramework::GameObject* CreateEve(const d2dFramework::GridPosition& position);
		static d2dFramework::GameObject* CreateYs(const d2dFramework::GridPosition& position);
		static d2dFramework::GameObject* CreateLilith(const d2dFramework::GridPosition& position);
		static d2dFramework::GameObject* CreateBealzebub(const d2dFramework::GridPosition& position);
		static d2dFramework::GameObject* CreateMonster(const d2dFramework::GridPosition& position, eMonsterType monsterType, unsigned int detectArea = 3u, unsigned int moveableBeatCount = 2u);
		static d2dFramework::GameObject* CreateStageNameObject();
		static d2dFramework::GameObject* CreateCutScene(eCutSceneObjectType cutSceneObjectType);

		static void SetBeatManager(d2dFramework::BeatManager* beatManager);

	private:
		static d2dFramework::BeatManager* mBeatManager;
	};
}