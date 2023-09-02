#pragma once

#include "eKeyState.h"
#include "BaseEntity.h"
#include "KeyInformation.h"

#include <queue>
#include <Windows.h>
#include <map>
#include <mutex>

namespace d2dFramework
{
	

	class AsyncInputManager final : public BaseEntity
	{
	public:
		AsyncInputManager();
		~AsyncInputManager();

		void Release();

		void Update();
		std::queue<KeyInformation> Flush();

		inline void AddHandleKey(char vKeyCode);

		inline bool GetIsEnd();

		inline void SetIsEnd(bool bIsEnd);

	private:
		std::map<char, eKeyState> mHandleKeys;
		std::queue<KeyInformation> mKeyQueue;
		std::mutex mMutex;
		bool mbIsEnd;
	};

	void AsyncInputManager::AddHandleKey(char vKeyCode)
	{
		mHandleKeys.insert({ vKeyCode, eKeyState::None });
	}

	void AsyncInputManager::SetIsEnd(bool bIsEnd)
	{
		mMutex.lock();
		mbIsEnd = bIsEnd;
		mMutex.unlock();
	}

	bool AsyncInputManager::GetIsEnd()
	{
		mMutex.lock();
		bool result = mbIsEnd;
		mMutex.unlock();

		return result;
	}
}