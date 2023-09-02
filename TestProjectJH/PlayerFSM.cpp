#include "PlayerFSM.h"

#include "InputManager.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "RenderManger.h"
#include "ObjectManager.h"
#include "CameraManager.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridPosition.h"
#include "GridMoveComponent.h"
#include "SpriteRenderer.h"
#include "AnimationRenderer.h"
#include "CircleCollider.h"
#include "Transform.h"

#include "MathHelper.h"

namespace d2dFramework
{
	PlayerFSM::PlayerFSM(unsigned int id)
		: BaseEntity::BaseEntity(id)
		, mPlayer(nullptr)
		, mPlayerNode(nullptr)
		, mOwnerMoveComponent(nullptr)
		, mBeatManager(nullptr)
		, mTimeManager(nullptr)
		, mPlayerCurrentState(ePlayerState::Idle)
		, mPlayerNextState(ePlayerState::Idle)
		, mNodeAround(false)
		, mMovePositionX(0)
		, mMovePositionY(0)
		, mGridCellSize(0)
		, mPlayerHp(100)
		, mComboCount(0)
		, mNodeAroundCount(0)
		, mDriection(0)
		, mBeatTime(0.0)
	{
	}

	PlayerFSM::~PlayerFSM()
	{
	}

	void PlayerFSM::Init(GameObject* player, GameObject* playernode, BeatManager* beatmanager, TimeManager* timemanager)
	{
		mPlayer = player;
		mPlayerNode = playernode;
		mBeatManager = beatmanager;
		mTimeManager = timemanager;
		mOwnerMoveComponent = mPlayer->GetComponent<GridMoveComponent>();
		mPlayerCurrentState = ePlayerState::Idle;
		mGridCellSize = mPlayer->GetComponent<GridComponent>()->GetCellDistance();

		nodeInit();

		auto registerMonsterCallback = [this](const std::string& data) ->void
		{
			const unsigned int id = std::stoi(data);

			GameObject* other = ObjectManager::GetInstance()->FindObjectOrNull(id);

			if (other->GetObjectType() == eObjectType::Enemy)
			{
				assert(other->GetComponent<Transform>() != nullptr);
				assert(other->GetComponent<GridComponent>() != nullptr);
				assert(other->GetComponent<GridMoveComponent>() != nullptr);

				if (mPlayerCurrentState == ePlayerState::Idle)
				{
					mPlayerHp -= 10;
					mPlayerNextState = ePlayerState::BeAttacked;
				}
			}
		};

		//
		auto MonsterColliderMonserCallback = [this](unsigned int monsterid)->void
		{
			GameObject* other = ObjectManager::GetInstance()->FindObjectOrNull(monsterid);

			if (other->GetObjectType() == eObjectType::Enemy)
			{
				assert(other->GetComponent<Transform>() != nullptr);
				assert(other->GetComponent<GridComponent>() != nullptr);
				assert(other->GetComponent<GridMoveComponent>() != nullptr);

				BeginOverlap(other);
			}
		};
		auto DaggerColliderMonserCallback = [this](unsigned int monsterid)->void
		{
			GameObject* other = ObjectManager::GetInstance()->FindObjectOrNull(monsterid);

			if (other->GetObjectType() == eObjectType::Projectile)
			{
				assert(other->GetComponent<Transform>() != nullptr);

				if (mPlayerCurrentState == ePlayerState::Idle)
				{
					BeginOverlap(other);
				}
			}
		};

		CircleCollider* playercollider = mPlayer->GetComponent<CircleCollider>();
		playercollider->RegisterOnEnterCollisionCallBack(mPlayer->GetId(), DaggerColliderMonserCallback);
		playercollider->RegisterOnEnterCollisionCallBack(mPlayer->GetId(), MonsterColliderMonserCallback);
		playercollider->SetIsTrigger(true);

		EventManager::GetInstance()->RegisterEventHandler("RegisterMonster", GetId(), registerMonsterCallback);
		GameObject* monster = ObjectManager::GetInstance()->FindObjectOrNull(mPlayer->GetId());
	}

	void PlayerFSM::nodeInit()
	{
		// 플레이어 노드 처음 위치 세팅
		Transform* playerTransform = mPlayer->GetComponent<Transform>();
		GridComponent* playerGrid = mPlayer->GetComponent<GridComponent>();
		Transform* nodeTransform = mPlayerNode->GetComponent<Transform>();
		SpriteRenderer* nodeSprite = mPlayerNode->GetComponent<SpriteRenderer>();

		nodeTransform->SetTranslate({ playerTransform->GetTranslate().GetX(), playerTransform->GetTranslate().GetY() });
		nodeSprite->SetOffSet({ 0, 0 });
		nodeSprite->SetTransformLayer(eTransformLayer::Grid);
	}

	void PlayerFSM::Update(float deltaTime, std::queue<KeyInformation>& keyQueue)
	{
		updateState(deltaTime);
		handleState(keyQueue);

		nodeUpdate(deltaTime);

		if (mPlayerCurrentState != mPlayerNextState)
		{
			exit();
			mPlayerCurrentState = mPlayerNextState;
			enter();
		}

		debugRender();
	}

	void PlayerFSM::nodeUpdate(float deltaTime)
	{
		// 현지 비트와 다음 비트가 달라지면 비트 카운트++
		static int CurrentBeat = mBeatTime / mBeatManager->GetIntervalTime();
		static int NextBeat = CurrentBeat;
		static int BeatCount = 0;

		// 유령 노드 위치
		float currentXPos = 0;
		float NextXPos = 0;
		float currentYPos = 0;
		float NextYPos = 0;

		// 노드 위치 = 플레이어 위치 + 노드 이동 위치
		Transform* nodeTransform = mPlayerNode->GetComponent<Transform>();
		Transform* playerTransform = mPlayer->GetComponent<Transform>();
		GridComponent* playerGrid = mPlayer->GetComponent<GridComponent>();

		// 지속 시간 구하기
		mBeatTime = static_cast<double>(mTimeManager->GetCurTime().QuadPart - mBeatManager->GetLastBPMChaneTime().QuadPart) / mTimeManager->GetFrequency().QuadPart;

		// 회전
		{
			//nodeTransform->SetTranslate({ playerTransform->GetTranslate().GetX(), playerTransform->GetTranslate().GetY() });
			//nodeTransform->AddRotate(180.f * NodeRotationTime);
		}

		// 직선
		{
			// 지속 시간 / 비트 시간
			// 지속 시간 / 비트 의 나머지 값 ( 보간 값 )
			NextBeat = mBeatTime / mBeatManager->GetIntervalTime();
			double InterforlationBeat = d2dFramework::MathHelper::GetDoubleRemainder(mBeatTime, mBeatManager->GetIntervalTime());
			InterforlationBeat /= mBeatManager->GetIntervalTime();

			// 비트가 달라지면 비트 카운트 ++
			if (CurrentBeat != NextBeat)
			{
				CurrentBeat = NextBeat;
				BeatCount++;

				if (mPlayerCurrentState == ePlayerState::Idle)
					mNodeAroundCount++;
			}

			// 4방향
			mDriection = BeatCount % 4;

			int CellDistance = playerGrid->GetCellDistance();
			int PlayergridX = playerTransform->GetTranslate().GetX();
			int PlayergridY = playerTransform->GetTranslate().GetY();

			switch (mDriection)
			{
			case 0:
				currentXPos = PlayergridX;
				currentYPos = PlayergridY + CellDistance;
				NextXPos = PlayergridX - CellDistance;
				NextYPos = PlayergridY;
				break;
			case 1:
				currentXPos = PlayergridX - CellDistance;
				currentYPos = PlayergridY;
				NextXPos = PlayergridX;
				NextYPos = PlayergridY - CellDistance;
				break;
			case 2:
				currentXPos = PlayergridX;
				currentYPos = PlayergridY - CellDistance;
				NextXPos = PlayergridX + CellDistance;
				NextYPos = PlayergridY;
				break;
			case 3:
				currentXPos = PlayergridX + CellDistance;
				currentYPos = PlayergridY;
				NextXPos = PlayergridX;
				NextYPos = PlayergridY + CellDistance;
				break;
			}

			float playerX = playerTransform->GetTranslate().GetX();
			float playerY = playerTransform->GetTranslate().GetY();

			//Percent
			nodeTransform->SetTranslate(
				{ MathHelper::Interpolation(currentXPos, NextXPos, InterforlationBeat)
				, MathHelper::Interpolation(currentYPos, NextYPos, InterforlationBeat) });
		}
	}

	void PlayerFSM::debugRender()
	{
		using namespace d2dFramework;


	}

	void PlayerFSM::handleState(std::queue<KeyInformation>& keyQueue)
	{
		switch (mPlayerCurrentState)
		{
		case ePlayerState::Idle:
		{
			// 노드가 세바퀴를 돌았을 때 데미지 / 세바퀴 + 0.2초
			if (mNodeAroundCount == 12 && mPlayerNextState == ePlayerState::Idle && (int)(mBeatTime * 1000) % (int)(mBeatManager->GetIntervalTime() * 1000) >= 200)
			{
				mPlayerNextState = ePlayerState::BeAttacked;
			}
			if (mPlayerHp <= 0)
			{
				mPlayerNextState = ePlayerState::Death;
			}
			if (keyQueue.empty())
			{
				return;
			}

			char key = keyQueue.front().Key;

			if (key == 'W')
			{
				mMovePositionY = 1;
			}
			else if (key == 'A')
			{
				mMovePositionX = -1;
			}
			else if (key == 'D')
			{
				mMovePositionX = 1;
			}
			else if (key == 'S')
			{
				mMovePositionY = -1;
			}

			if (checkBeat(keyQueue) && checkDirectionNode(mMovePositionX, mMovePositionY))
			{
				mPlayerNextState = ePlayerState::Dash;
			}
			else
			{
				if (checkDeath())
				{
					mPlayerNextState = ePlayerState::Death;
				}
				else
				{
					mPlayerNextState = ePlayerState::BeAttacked;
				}
			}
		}
		break;
		case d2dFramework::ePlayerState::Dash:
		{
			if (mOwnerMoveComponent->GetIsCanMove())
			{
				mPlayerNextState = ePlayerState::Idle;
			}
		}
		break;
		case d2dFramework::ePlayerState::Attack:
		{
			if (mOwnerMoveComponent->GetIsCanMove())
			{
				mPlayerNextState = ePlayerState::Idle;
			}
		}
		break;
		case d2dFramework::ePlayerState::BeAttacked:
		{
			AnimationRenderer* playerAnimation = mPlayer->GetComponent<AnimationRenderer>();
			if (playerAnimation->GetFrameindex() >= 6)
			{
				mPlayerNextState = ePlayerState::Idle;
			}
		}
		break;
		case d2dFramework::ePlayerState::Death:
			break;
		}
	}

	void PlayerFSM::updateState(float deltaTime)
	{
		switch (mPlayerCurrentState)
		{
		case d2dFramework::ePlayerState::Idle:
			break;
		case d2dFramework::ePlayerState::Dash:
			break;
		case d2dFramework::ePlayerState::Attack:
			break;
		case d2dFramework::ePlayerState::BeAttacked:
			break;
		case d2dFramework::ePlayerState::Death:
			break;
		}
	}

	void PlayerFSM::enter()
	{
		AnimationRenderer* playerAnimation = mPlayer->GetComponent<AnimationRenderer>();

		switch (mPlayerCurrentState)
		{
		case d2dFramework::ePlayerState::Idle:
		{
			mMovePositionX = 0;
			mMovePositionY = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationAsset(mPlayerAnimation.find(L"PlayerIdle")->second);
			playerAnimation->SetIsLoop(true);
		}
		break;
		case d2dFramework::ePlayerState::Dash:
		{
			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationAsset(mPlayerAnimation.find(L"PlayerMove")->second);
			playerAnimation->SetIsLoop(false);
			mOwnerMoveComponent->Move(mMovePositionX, mMovePositionY);
		}
		break;
		case d2dFramework::ePlayerState::Attack:
		{
			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationAsset(mPlayerAnimation.find(L"PlayerAttack")->second);
			playerAnimation->SetIsLoop(true);
		}
		break;
		case d2dFramework::ePlayerState::BeAttacked:
		{
			mNodeAroundCount = 0;
			mPlayerHp -= 5;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationAsset(mPlayerAnimation.find(L"PlayerBeAttacked")->second);
			playerAnimation->SetIsLoop(false);
		}
		break;
		case d2dFramework::ePlayerState::Death:
		{
			mNodeAroundCount = 0;
			playerAnimation->SetFrameindex(0);
			playerAnimation->SetAnimationAsset(mPlayerAnimation.find(L"PlayerDeath")->second);
			playerAnimation->SetIsLoop(false);
			mPlayerNode->GetComponent<SpriteRenderer>()->SetIsActive(false);
		}
		break;
		}
	}

	void PlayerFSM::exit()
	{
		switch (mPlayerCurrentState)
		{
		case d2dFramework::ePlayerState::Idle:
			break;
		case d2dFramework::ePlayerState::Dash:
			break;
		case d2dFramework::ePlayerState::Attack:
			break;
		case d2dFramework::ePlayerState::BeAttacked:
			break;
		case d2dFramework::ePlayerState::Death:
			break;
		}
	}

	void PlayerFSM::BeginOverlap(GameObject* monster)
	{
		CircleCollider* monstercollider = monster->GetComponent<CircleCollider>();
		SpriteRenderer* monstersprite = monster->GetComponent<SpriteRenderer>();

		if (mPlayerCurrentState == ePlayerState::Idle)
		{
			mPlayerNextState = ePlayerState::BeAttacked;
		}
		if (mPlayerCurrentState == ePlayerState::Dash)
		{
			mPlayerNextState = ePlayerState::Attack;
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

	bool PlayerFSM::checkDirectionNode(int x, int y)
	{
		// 예외 처리는 이후에 하고 일단 이동하는지 확인 코드만 작성

		Transform* nodetransform = mPlayerNode->GetComponent<Transform>();
		Transform* playertransform = mPlayer->GetComponent<Transform>();
		AnimationRenderer* playerAnimation = mPlayer->GetComponent<AnimationRenderer>();

		float nodePositionX = nodetransform->GetTransform().dx - playertransform->GetTransform().dx;
		float nodePositionY = nodetransform->GetTransform().dy - playertransform->GetTransform().dy;

		if (y == 1)
		{
			if (nodePositionY >= 70.f)
			{
				return true;
			}
		}
		if (x == -1)
		{
			if (nodePositionX <= -70)
			{
				playerAnimation->SetIsLeft(false);
				return true;
			}
		}
		if (y == -1)
		{
			if (nodePositionY <= -70)
			{
				return true;
			}
		}
		if (x == 1)
		{
			if (nodePositionX >= 70)
			{
				playerAnimation->SetIsLeft(true);
				return true;
			}
		}

		return false;
	}

	bool PlayerFSM::checkMonster(int movePositionX, int movePositionY)
	{
		// 이동 방향에 몬스터가 있는지 판정
		std::set<unsigned int> Monsters = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Enemy);

		for (unsigned int monsterid : Monsters)
		{
			auto monster = ObjectManager::GetInstance()->FindObjectOrNull(monsterid);
			GridComponent* monstergird = monster->GetComponent<GridComponent>();
			GridComponent* playergrid = mPlayer->GetComponent<GridComponent>();

			if (monstergird->GetPosition().X == playergrid->GetPosition().X + movePositionX
				&& monstergird->GetPosition().Y == playergrid->GetPosition().Y + movePositionY)
			{
				return true;
			}
		}

		return false;
	}

	bool PlayerFSM::checkDeath(int damage)
	{
		// 데미지를 받아서 체력이 0보다 작아지는 경우
		mPlayerHp -= damage;

		if (mPlayerHp <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void PlayerFSM::Release()
	{
		mBeatManager = nullptr;
		mPlayerNode = nullptr;
		mOwnerMoveComponent = nullptr;

		mPlayer->GetComponent<CircleCollider>()->UnregisterOnCollisionCallBack(GetId());
	}
}