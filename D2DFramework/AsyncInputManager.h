#pragma once

#include "eKeyState.h"

#include <queue>
#include <Windows.h>
#include <map>
#include <mutex>

namespace d2dFramework
{
	struct KeyInformation
	{
		char Key;
		LARGE_INTEGER Time;
	};

	class AsyncInputManager
	{
	public:
		AsyncInputManager();
		~AsyncInputManager() = default;

		void Update();
		std::queue<KeyInformation> Flush();

		inline bool GetIsEnd();

		inline void AddHandleKey(char vKeyCode);
		inline void SetIsEnd(bool bIsEnd);

	private:
		std::map<char, eKeyState> mHandleKeys;
		std::queue<KeyInformation> mKeyQueue;
		std::mutex mMutex;
		bool mbIsEnd;
	};

	bool AsyncInputManager::GetIsEnd()
	{
		mMutex.lock();
		bool result = mbIsEnd;
		mMutex.unlock();

		return result;
	}

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
}