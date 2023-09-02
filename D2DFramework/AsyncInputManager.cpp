#include "AsyncInputManager.h"

#include "eFrameworkID.h"

namespace d2dFramework
{
	AsyncInputManager::AsyncInputManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::AsyncInputManager))
		, mHandleKeys()
		, mKeyQueue()
		, mMutex()
		, mbIsEnd(false)
	{
	}

	AsyncInputManager::~AsyncInputManager()
	{
		Release();
	}

	void AsyncInputManager::Release()
	{
		mMutex.lock();

		mHandleKeys.clear();

		while (!mKeyQueue.empty())
		{
			mKeyQueue.pop();
		}

		mbIsEnd = true;

		mMutex.unlock();
	}

	void AsyncInputManager::Update()
	{
		for (auto iter = mHandleKeys.begin(); iter != mHandleKeys.end(); ++iter)
		{
			if (GetAsyncKeyState(iter->first) & 0x8000)
			{
				switch (iter->second)
				{
				case eKeyState::None:
					/* intentional fall-through */
				case eKeyState::Pop:
					iter->second = eKeyState::Push;
					mMutex.lock();
					LARGE_INTEGER time;
					QueryPerformanceCounter(&time);
					mKeyQueue.push({ iter->first, time });
					mMutex.unlock();
					break;
				case eKeyState::Push:
					/* intentional fall-through */
				case eKeyState::Hold:
					iter->second = eKeyState::Hold;
					break;
				default:
					break;
				}
			}
			else
			{
				switch (iter->second)
				{
				case eKeyState::None:
					/* intentional fall-through */
				case eKeyState::Pop:
					iter->second = eKeyState::None;
					break;
				case eKeyState::Push:
					/* intentional fall-through */
				case eKeyState::Hold:
					iter->second = eKeyState::Pop;
					break;
				default:
					break;
				}
			}
		}
	}

	std::queue<KeyInformation> AsyncInputManager::Flush()
	{
		mMutex.lock();
		std::queue<KeyInformation> result(std::move(mKeyQueue));
		mMutex.unlock();

		return result;
	}
}