#include "PlayerFSM.h"

#include "InputManager.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "RenderManger.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "HPComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridPosition.h"
#include "GridMoveComponent.h"
#include "SpriteRenderer.h"
#include "NodeComponent.h"
#include "AnimationRenderer.h"
#include "CircleCollider.h"
#include "Transform.h"
#include "DaggerComponent.h"
#include "PlayerComponent.h"
#include "ConstantTable.h"

#include "MathHelper.h"
#include "eSoulBeaterID.h"
#include "RenderManger.h"

namespace soulBeater
{
	PlayerFSM::PlayerFSM()
		: BaseEntity::BaseEntity(static_cast<unsigned int>(eSoulBeaterID::PlayerFSM))
		, mPlayerId(0)
		, mPlayerNodeId(0)
		, mBeatManager(nullptr)
		, mTimeManager(nullptr)
		, mNodeAround(false)
		, mMovePositionX(0)
		, mMovePositionY(0)
		, mComboCount(0)
		, mNodeAroundCount(0)
		, mDriection(0)
		, mBeatTime(0.0)
	{
	}

	PlayerFSM::~PlayerFSM()
	{
	}

	void PlayerFSM::Init(BeatManager* beatmanager, TimeManager* timemanager, unsigned int playerid, unsigned int playernodeid)
	{
		mPlayerId = playerid;
		mPlayerNodeId = playernodeid;
		mBeatManager = beatmanager;
		mTimeManager = timemanager;
	}

	void PlayerFSM::Update(float deltaTime, std::queue<KeyInformation>& keyQueue)
	{
		using namespace d2dFramework;

		assert(mPlayerId != static_cast<unsigned int>(-1));
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerId);

		if (player == nullptr)
		{
			return;
		}
		PlayerComponent* playecomponent = player->GetComponent<PlayerComponent>();

		GameObject* hpObject = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::HPObject), true);
		if (hpObject != nullptr)
		{
			HPComponent* hpComponent = hpObject->GetComponent<HPComponent>();
			hpComponent->SetHP(playecomponent->GetPlayerHp());
		}

		handleState(keyQueue);
		nodeUpdate(deltaTime);

		if (playecomponent->GetPlayerCurrentState() != playecomponent->GetPlayerNextState())
		{
			exit();
			playecomponent->SetPlayerCurrentState(playecomponent->GetPlayerNextState());
			enter();
		}
	}

	void PlayerFSM::nodeUpdate(float deltaTime)
	{
		// 현지 비트와 다음 비트가 달라지면 비트 카운트++
		static int CurrentBeat = static_cast<int>(mBeatTime / mBeatManager->GetIntervalTime());
		static int NextBeat = CurrentBeat;
		static int BeatCount = 0;

		// 유령 노드 위치
		float currentXPos = 0;
		float NextXPos = 0;
		float currentYPos = 0;
		float NextYPos = 0;

		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerId);
		GameObject* playernode = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerNodeId);

		PlayerComponent* playecomponent = player->GetComponent<PlayerComponent>();

		// 노드 위치 = 플레이어 위치 + 노드 이동 위치
		Transform* nodeTransform = playernode->GetComponent<Transform>();
		Transform* playerTransform = player->GetComponent<Transform>();
		GridComponent* playerGrid = player->GetComponent<GridComponent>();
		NodeComponent* playernoderender = playernode->GetComponent<NodeComponent>();

		assert(player != nullptr);
		assert(playernode != nullptr);

		// 지속 시간 구하기
		mBeatTime = static_cast<double>(mTimeManager->GetCurTime().QuadPart - mBeatManager->GetLastBPMChaneTime().QuadPart) / mTimeManager->GetFrequency().QuadPart;

		// 직선
		{
			// 지속 시간 / 비트 시간
			// 지속 시간 / 비트 의 나머지 값 ( 보간 값 )
			NextBeat = static_cast<unsigned int>(mBeatTime / mBeatManager->GetIntervalTime());
			double InterforlationBeat = d2dFramework::MathHelper::GetDoubleRemainder(mBeatTime, mBeatManager->GetIntervalTime());
			InterforlationBeat /= mBeatManager->GetIntervalTime();

			//// 비트가 달라지면 비트 카운트 ++
			//if (CurrentBeat != NextBeat)
			//{
			//	CurrentBeat = NextBeat;
			//	BeatCount++;

			//	if (mPlayerCurrentState == ePlayerState::Idle)
			//		mNodeAroundCount++;
			//}

			if (playecomponent->GetPlayerCurrentState() == ePlayerState::Idle)
			{
				if (InterforlationBeat >= 0.7 || InterforlationBeat <= 0.3)
				{
					playernoderender->SetNodeState(eNodeState::Moveable);
					playernoderender->SetIsActive(true);
				}
				else
				{
					playernoderender->SetNodeState(eNodeState::UnableMove);
					playernoderender->SetIsActive(false);
				}
			}

			// 4방향
			mDriection = BeatCount % 4;

			//int CellDistance = playerGrid->GetCellDistance();
			//int PlayergridX = playerTransform->GetTranslate().GetX();
			//int PlayergridY = playerTransform->GetTranslate().GetY();

			//switch (mDriection)
			//{
			//case 0:
			//	currentXPos = PlayergridX;
			//	currentYPos = PlayergridY + CellDistance;
			//	NextXPos = PlayergridX - CellDistance;
			//	NextYPos = PlayergridY;
			//	break;
			//case 1:
			//	currentXPos = PlayergridX - CellDistance;
			//	currentYPos = PlayergridY;
			//	NextXPos = PlayergridX;
			//	NextYPos = PlayergridY - CellDistance;
			//	break;
			//case 2:
			//	currentXPos = PlayergridX;
			//	currentYPos = PlayergridY - CellDistance;
			//	NextXPos = PlayergridX + CellDistance;
			//	NextYPos = PlayergridY;
			//	break;
			//case 3:
			//	currentXPos = PlayergridX + CellDistance;
			//	currentYPos = PlayergridY;
			//	NextXPos = PlayergridX;
			//	NextYPos = PlayergridY + CellDistance;
			//	break;
			//}

			//float playerX = playerTransform->GetTranslate().GetX();
			//float playerY = playerTransform->GetTranslate().GetY();

			////Percent
			//nodeTransform->SetTranslate(
			//	{ MathHelper::Interpolation(currentXPos, NextXPos, InterforlationBeat)
			//	, MathHelper::Interpolation(currentYPos, NextYPos, InterforlationBeat) });
		}
	}

	bool PlayerFSM::checkMinMax(int x, int y)
	{
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerId);
		GridComponent* playergrid = player->GetComponent<GridComponent>();

		if (playergrid->GetPosition().X + x > playergrid->GetMaxX())
		{
			return false;
		}
		if (playergrid->GetPosition().Y + y > playergrid->GetMaxY())
		{
			return false;
		}

		return true;
	}

	void PlayerFSM::handleState(std::queue<KeyInformation>& keyQueue)
	{
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerId);
		GameObject* playernode = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerNodeId);

		assert(player != nullptr);
		assert(playernode != nullptr);

		PlayerComponent* playecomponent = player->GetComponent<PlayerComponent>();
		GridMoveComponent* playermove = player->GetComponent<GridMoveComponent>();
		GridComponent* playergrid = player->GetComponent<GridComponent>();
		AnimationRenderer* playeranim = player->GetComponent<AnimationRenderer>();

		switch (playecomponent->GetPlayerCurrentState())
		{
		case ePlayerState::Idle:
		{
			// 노드가 세바퀴를 돌았을 때 데미지 / 세바퀴 + 0.2초
			if (mNodeAroundCount == 12 && playecomponent->GetPlayerNextState() == ePlayerState::Idle && (int)(mBeatTime * 1000) % (int)(mBeatManager->GetIntervalTime() * 1000) >= 200)
			{
				playecomponent->SetPlayerNextState(ePlayerState::BeAttacked);
			}
			if (playecomponent->GetPlayerHp() <= 0)
			{
				playecomponent->SetPlayerNextState(ePlayerState::Death);
			}
			if (keyQueue.empty())
			{
				return;
			}

			mMovePositionX = 0;
			mMovePositionY = 0;
			bool bIsWall = false;

			char key = keyQueue.front().Key;

			if (key == 'W')
			{
				const std::set<unsigned int>& WallsId = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Wall);
				for (const unsigned int wallid : WallsId)
				{
					GameObject* Wall = ObjectManager::GetInstance()->FindObjectOrNull(wallid);
					if (Wall->GetComponent<GridComponent>()->GetPosition().X == playergrid->GetPosition().X
						&& Wall->GetComponent<GridComponent>()->GetPosition().Y == playergrid->GetPosition().Y + 1)
					{
						bIsWall = true;
						return;
					}
				}
				mMovePositionY = 1;
			}
			else if (key == 'A')
			{
				const std::set<unsigned int>& WallsId = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Wall);
				for (unsigned int wallid : WallsId)
				{
					GameObject* Wall = ObjectManager::GetInstance()->FindObjectOrNull(wallid);
					if (Wall->GetComponent<GridComponent>()->GetPosition().X == playergrid->GetPosition().X - 1
						&& Wall->GetComponent<GridComponent>()->GetPosition().Y == playergrid->GetPosition().Y)
					{
						bIsWall = true;
						return;
					}
				}
				mMovePositionX = -1;
			}
			else if (key == 'D')
			{
				const std::set<unsigned int>& WallsId = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Wall);
				for (unsigned int wallid : WallsId)
				{
					GameObject* Wall = ObjectManager::GetInstance()->FindObjectOrNull(wallid);
					if (Wall->GetComponent<GridComponent>()->GetPosition().X == playergrid->GetPosition().X + 1
						&& Wall->GetComponent<GridComponent>()->GetPosition().Y == playergrid->GetPosition().Y)
					{
						bIsWall = true;
						return;
					}
				}
				mMovePositionX = 1;
			}
			else if (key == 'S')
			{
				std::set<unsigned int> WallsId = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Wall);
				for (unsigned int wallid : WallsId)
				{
					GameObject* Wall = ObjectManager::GetInstance()->FindObjectOrNull(wallid);
					GridComponent* wallgrid = Wall->GetComponent<GridComponent>();

					if (Wall->GetComponent<GridComponent>()->GetPosition().X == playergrid->GetPosition().X
						&& Wall->GetComponent<GridComponent>()->GetPosition().Y == playergrid->GetPosition().Y - 1)
					{
						bIsWall = true;
						return;
					}
				}
				mMovePositionY = -1;
			}

			if (checkBeat(keyQueue) && bIsWall == false && checkMinMax(mMovePositionX, mMovePositionY))
			{
				playecomponent->SetPlayerNextState(ePlayerState::Dash);
			}
			else
			{
				if (playecomponent->GetPlayerHp() <= 0)
				{
					playecomponent->SetPlayerNextState(ePlayerState::Death);
				}
				else
				{
					playecomponent->SetPlayerNextState(ePlayerState::BeAttacked);
				}
			}
		}
		break;
		case ePlayerState::Dash:
		{

			if (playermove->GetIsCanMove())
			{
				playecomponent->SetPlayerNextState(ePlayerState::Idle);
			}
		}
		break;
		case ePlayerState::Attack:
		{

			if (playermove->GetIsCanMove())
			{
				playecomponent->SetPlayerNextState(ePlayerState::Idle);
			}
		}
		break;
		case ePlayerState::BeAttacked:
		{

			AnimationRenderer* playerAnimation = player->GetComponent<AnimationRenderer>();
			if (playerAnimation->GetFrameindex() >= 6)
			{
				playecomponent->SetPlayerNextState(ePlayerState::Idle);
			}
		}
		break;
		case ePlayerState::Death:
			if (playeranim->GetIsAnimationEnd())
			{
				ObjectManager::GetInstance()->DeleteObject(mPlayerId);
			}
			break;
		}
	}

	void PlayerFSM::enter()
	{
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerId);
		GameObject* playernode = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerNodeId);

		PlayerComponent* playecomponent = player->GetComponent<PlayerComponent>();
		GridMoveComponent* playermove = player->GetComponent<GridMoveComponent>();
		AnimationRenderer* playerAnimation = player->GetComponent<AnimationRenderer>();
		GridComponent* playerGrid = player->GetComponent<GridComponent>();

		NodeComponent* playernoderender = playernode->GetComponent<NodeComponent>();
		GridComponent* nodegrid = playernode->GetComponent<GridComponent>();

		switch (playecomponent->GetPlayerCurrentState())
		{
		case ePlayerState::Idle:
		{
				//SoundManager::GetInstance()->Play()
			nodegrid->SetPosition({ playerGrid->GetPosition().X, playerGrid->GetPosition().Y });

			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationIndex(1);
			playerAnimation->SetIsLoop(true);
		}
		break;
		case ePlayerState::Dash:
		{			SoundManager::GetInstance()->Play(EFFECT_PLAYERMOVE_KEY, eSoundType::Effect, ePlayType::Overwrite, 0);

			if (playermove->Move(mMovePositionX, mMovePositionY))
			{
				playernoderender->SetNodeState(eNodeState::MoveSuccess);
				playernoderender->SetIsActive(false);
			}

			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationIndex(0);
			playerAnimation->SetIsLoop(false);

			if (mMovePositionX > 0)
			{
				playerAnimation->SetIsLeft(true);
			}
			else if (mMovePositionX < 0)
			{
				playerAnimation->SetIsLeft(false);
			}
		}
		break;
		case ePlayerState::Attack:
		{
			SoundManager::GetInstance()->Play(EFFECT_PLAYERATTACK_KEY, eSoundType::Effect, ePlayType::Overwrite, 1);
			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationIndex(2);
			playerAnimation->SetIsLoop(true);

		}
		break;
		case ePlayerState::BeAttacked:
		{
			SoundManager::GetInstance()->Play(EFFECT_PLAYERHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 2);
			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationIndex(3);
			playerAnimation->SetIsLoop(false);
		}
		break;
		case ePlayerState::Death:
		{
			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationIndex(4);
			playerAnimation->SetIsLoop(false);
			playernode->GetComponent<NodeComponent>()->SetIsActive(false);
		}
		break;
		}
	}

	void PlayerFSM::exit()
	{
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerId);
		GameObject* playernode = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerNodeId);

		assert(player != nullptr);
		assert(playernode != nullptr);

		PlayerComponent* playecomponent = player->GetComponent<PlayerComponent>();

		switch (playecomponent->GetPlayerCurrentState())
		{
		case ePlayerState::Idle:
			break;
		case ePlayerState::Dash:
			break;
		case ePlayerState::Attack:
			break;
		case ePlayerState::BeAttacked:
			break;
		case ePlayerState::Death:
			break;
		}
	}

	// 플레이어의 키 입력과 비트 인터벌이 오차 몇 이하인지를 체크하면되겟지? 넹
	bool PlayerFSM::checkBeat(std::queue<KeyInformation>& keyQueue)
	{
		KeyInformation& frontInput = keyQueue.front();
		double defferencTime = mBeatManager->GetIntervalDefferenceTime(frontInput.Time);

		// 비트가 맞는지 판정
		if (defferencTime <= BEAT_TOLERANCE)
		{
			return true;
		}

		while (!keyQueue.empty())
		{
			keyQueue.pop();
		}

		return false;
	}

	void PlayerFSM::Release()
	{

	}
}