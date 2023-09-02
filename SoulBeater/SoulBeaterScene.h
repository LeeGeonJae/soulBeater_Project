#pragma once
#include "Scene.h"
#include "eUIManagerType.h"
#include "eSceneType.h"

namespace soulBeater
{
	class SoulBeaterScene :public d2dFramework::Scene
	{
	public:
		SoulBeaterScene(unsigned int id);
		~SoulBeaterScene() override = default;

		void Enter() override;
		void Exit() override;

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		inline void SetUIType(eUIManagerType UIType);
		inline void SetSceneType(eSceneType sceneType);
		inline void SetNextSceneID(unsigned int nextSceneID);

		inline eUIManagerType GetUIType() const;
		inline eSceneType GetSceneType() const;
		inline unsigned int GetNextSceneID() const;

	private:
		void enterIndependentObject();
		void enterCreateObject();
		void enterSoundPlay();

	private:
		eUIManagerType mUIType;
		eSceneType mSceceType;
		unsigned int mNextSceneID;
	};

	void SoulBeaterScene::SetUIType(eUIManagerType UIType)
	{
		mUIType = UIType;
	}
	void SoulBeaterScene::SetSceneType(eSceneType sceneType)
	{
		mSceceType = sceneType;
	}
	void SoulBeaterScene::SetNextSceneID(unsigned int nextSceneID)
	{
		mNextSceneID = nextSceneID;
	}

	eUIManagerType SoulBeaterScene::GetUIType() const
	{
		return mUIType;
	}
	eSceneType SoulBeaterScene::GetSceneType() const
	{
		return mSceceType;
	}
	unsigned int SoulBeaterScene::GetNextSceneID() const
	{
		return mNextSceneID;
	}
}

