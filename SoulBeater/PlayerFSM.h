#pragma once

#include "BaseEntity.h"
#include "BeatManager.h"
#include "TimeManager.h"
#include "KeyInformation.h"

#include "AnimationAsset.h"

#include <queue>
#include <string>


namespace d2dFramework
{
	class GameObject;
	class GridMoveComponent;
	class AnimationAsset;
}

namespace soulBeater
{
	const double BEAT_TOLERANCE = 0.2;
	class PlayerComponent;

	class PlayerFSM : public d2dFramework::BaseEntity
	{
	public:
		PlayerFSM();
		~PlayerFSM();

		void Init(d2dFramework::BeatManager* beatmanager, d2dFramework::TimeManager* timemanager, unsigned int playerid, unsigned int playernodeid);
		void Update(float deltaTime, std::queue<d2dFramework::KeyInformation>& keyQueue);
		void Release();

	private:
		void nodeUpdate(float deltaTime);
		bool checkMinMax(int x, int y);

	public:
		inline void SetPlayerAnimationAsset(std::wstring animationName, d2dFramework::AnimationAsset* playerAnimationAsset);

	private:
		void handleState(std::queue<d2dFramework::KeyInformation>& keyQueue);
		void enter();
		void exit();

	private:
		// 이동 처리 로직
		bool checkBeat(std::queue<d2dFramework::KeyInformation>& keyQueue);

	private:
		// 비트 매니저
		d2dFramework::TimeManager* mTimeManager;
		d2dFramework::BeatManager* mBeatManager;

		// 비트 지속 시간
		double mBeatTime;

		// 플레이어 오브젝트 참조
		unsigned int mPlayerId;
		unsigned int mPlayerNodeId;

		std::map<std::wstring, d2dFramework::AnimationAsset*> mPlayerAnimation;

		int mMovePositionX;
		int mMovePositionY;

		// 플레이어 노드 현재 방향
		int mDriection;
		bool mNodeAround;

		// 노드 방향 전환시에 카운트 ++
		unsigned int mNodeAroundCount;

		// 콤보
		unsigned int mComboCount;
	};

	void PlayerFSM::SetPlayerAnimationAsset(std::wstring animationName, d2dFramework::AnimationAsset* playerAnimationAsset)
	{
		mPlayerAnimation.insert(std::make_pair(animationName, playerAnimationAsset));
	}
}