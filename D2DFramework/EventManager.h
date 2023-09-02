#pragma once

#include "BaseEntity.h"

#include <cassert>
#include <map>
#include <unordered_map>
#include <queue>
#include <functional>
#include <string>

namespace d2dFramework
{
	enum class eDefaultEvent
	{
		ChangeScene,
	};

	class EventManager final : public BaseEntity
	{
		friend class GameProcessor;

	public:
		static EventManager* GetInstance();

		void ExcuteBroadcastEvent(const std::string& event, const std::string& data);
		inline void RegisterBroadcastEvent(const std::string& event, const std::string& data);

		inline void RegisterEventHandler(const std::string& event, unsigned int id, std::function<void(const std::string&)> callback);
		inline void UnregisterEventHandler(const std::string& event, unsigned int id);

		inline const std::unordered_map<std::string, std::unordered_map<unsigned int, std::function<void(const std::string& data)>>>& GetEventCallbackMap() const;
		inline const std::queue<std::pair<std::string, std::string>>& GetBraodcastEventQueue() const;

	private:
		EventManager();
		~EventManager();

		void handleEvent();
		void release();

	private:
		enum { RESERVE_SIZE = 1024 };

		static EventManager* mInstance;

		std::unordered_map<std::string, std::unordered_map<unsigned int, std::function<void(const std::string& data)>>> mEventCallbackMap;
		std::queue<std::pair<std::string, std::string>> mBroadcastEventQueue;
	};

	void EventManager::RegisterBroadcastEvent(const std::string& event, const std::string& data)
	{
		mBroadcastEventQueue.push({ event, data });
	}
	void EventManager::RegisterEventHandler(const std::string& event, unsigned int id, std::function<void(const std::string&)> callback)
	{
		mEventCallbackMap.insert({ event, std::unordered_map<unsigned int, std::function<void(const std::string& data)>>() });
		auto& callbakcMap = mEventCallbackMap.find(event)->second;
		callbakcMap.insert({ id, callback });
	}
	void EventManager::UnregisterEventHandler(const std::string& event, unsigned int id)
	{
		auto iter = mEventCallbackMap.find(event);
		if (iter == mEventCallbackMap.end())
		{
			return;
		}

		iter->second.erase(id);
	}

	const std::unordered_map<std::string, std::unordered_map<unsigned int, std::function<void(const std::string& data)>>>& EventManager::GetEventCallbackMap() const
	{
		return mEventCallbackMap;
	}
	const std::queue<std::pair<std::string, std::string>>& EventManager::GetBraodcastEventQueue() const
	{
		return mBroadcastEventQueue;
	}
}