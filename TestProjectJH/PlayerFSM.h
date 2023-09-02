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
	const double BEAT_TOLERANCE = 0.2;

	class GameObject;
	class GridMoveComponent;
	class AnimationAsset;

	enum class ePlayerState
	{
		Idle = 0,
		Dash,
		Attack,
		BeAttacked,
		Death,
	};

	class PlayerFSM : public BaseEntity
	{
	public:
		PlayerFSM(unsigned int id);
		~PlayerFSM();

		void Init(GameObject* player, GameObject* playernode, BeatManager* beatmanager, TimeManager* timemanager);
		void Update(float deltaTime, std::queue<KeyInformation>& keyQueue);
		void Release();

	private:
		void nodeInit();
		void nodeUpdate(float deltaTime);

		void debugRender();

	public:
		inline void SetPlayerAnimationAsset(std::wstring animationName, AnimationAsset* playerAnimationAsset);

	public:
		int GetPlayerHP() const { return mPlayerHp; }

	private:
		void handleState(std::queue<KeyInformation>& keyQueue);
		void updateState(float deltaTime);
		void enter();
		void exit();

	private:
		void BeginOverlap(GameObject* monster);

	private:
		// 이동 처리 로직
		bool checkBeat(std::queue<KeyInformation>& keyQueue);
		bool checkDirectionNode(int x, int y);
		bool checkMonster(int mMovePositionX, int mMovePositionY);

		// 죽음 판정 로직
		bool checkDeath(int damage = 5);

	private:
		// 비트 매니저
		TimeManager* mTimeManager;
		BeatManager* mBeatManager;

		// 플레이어 오브젝트 참조
		GameObject* mPlayer;
		GameObject* mPlayerNode;
		GridMoveComponent* mOwnerMoveComponent;

		std::map<std::wstring, AnimationAsset*> mPlayerAnimation;

		// 플레이어 상태
		int mPlayerHp;
		ePlayerState mPlayerCurrentState;
		ePlayerState mPlayerNextState;

		// 이동 방향
		unsigned int mGridCellSize;
		int mMovePositionX;
		int mMovePositionY;

		// 플레이어 노드 현재 방향
		int mDriection;
		bool mNodeAround;

		// 노드 방향 전환시에 카운트 ++
		unsigned int mNodeAroundCount;

		// 콤보
		unsigned int mComboCount;

		// 비트 지속 시간
		double mBeatTime;
	};

	void PlayerFSM::SetPlayerAnimationAsset(std::wstring animationName, AnimationAsset* playerAnimationAsset)
	{
		mPlayerAnimation.insert(std::make_pair(animationName, playerAnimationAsset));
	}
}