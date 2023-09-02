#include "EventManager.h"

#include "eFrameworkID.h"

namespace d2dFramework
{
	EventManager* EventManager::mInstance = nullptr;

	EventManager* EventManager::GetInstance()
	{
		assert(mInstance != nullptr);
		return mInstance;
	}

	EventManager::EventManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::EventManager))
		, mEventCallbackMap(RESERVE_SIZE)
		, mBroadcastEventQueue()
	{
	}

	EventManager::~EventManager()
	{
		release();
	}

	void EventManager::release()
	{
		while (!mBroadcastEventQueue.empty())
		{
			mBroadcastEventQueue.pop();
		}

		mEventCallbackMap.clear();
	}


	void EventManager::ExcuteBroadcastEvent(const std::string& event, const std::string& data)
	{
		auto keyCallback = mEventCallbackMap.find(event);

		if (keyCallback == mEventCallbackMap.end())
		{
			return;
		}

		std::unordered_map<unsigned int, std::function<void(const std::string& data)>> listener = keyCallback->second;

		for (auto iter = listener.begin(); iter != listener.end(); ++iter)
		{
			iter->second(data);
		}
	}

	void EventManager::handleEvent()
	{
		while (!mBroadcastEventQueue.empty())
		{
			auto keyData = mBroadcastEventQueue.front();
			mBroadcastEventQueue.pop();

			ExcuteBroadcastEvent(keyData.first, keyData.second);
		}
	}

}