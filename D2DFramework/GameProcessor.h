#pragma once

#include <cassert>
#include <string>
#include <windows.h>

namespace d2dFramework
{
	class TimeManager;
	class RenderManager;
	class InputManager;
	class EventManager;
	class SceneManager;
	class CollisionManager;
	class CameraManager;
	class SoundManager;
	class GameObject;

	class GameProcessor
	{
	public:
		GameProcessor(UINT width, UINT height, std::wstring name);
		virtual ~GameProcessor();

		virtual void Init(HWND hwnd);
		virtual void Update();
		virtual void Release();

		inline void SetIsUpdateable(bool bIsUpdateable);

		inline UINT GetWidth() const;
		inline UINT GetHeight() const;
		inline const WCHAR* GetTitle() const;
		inline bool GetIsUpdateable();

	protected:
		inline TimeManager* getTimeManager() const;
		inline SceneManager* getSceneManager() const;
		inline CollisionManager* getCollisionManager() const;
		inline CameraManager* getCameraManager() const;

	private:
		UINT mWidth;
		UINT mHeight;
		std::wstring mTitle;

		HWND mHwnd;

		TimeManager* mTimeManager;
		SceneManager* mSceneManager;
		CollisionManager* mCollisionManager;
		CameraManager* mCameraManager;

		bool mbIsUpdate;
	};

	void GameProcessor::SetIsUpdateable(bool bIsUpdateable)
	{
		mbIsUpdate = bIsUpdateable;
	}

	UINT GameProcessor::GetWidth() const
	{
		return mWidth;
	}
	UINT GameProcessor::GetHeight() const
	{
		return mHeight;
	}
	const WCHAR* GameProcessor::GetTitle() const
	{
		return mTitle.c_str();
	}
	bool GameProcessor::GetIsUpdateable()
	{
		return mbIsUpdate;
	}

	TimeManager* GameProcessor::getTimeManager() const
	{
		assert(mTimeManager != nullptr);
		return mTimeManager;
	}
	SceneManager* GameProcessor::getSceneManager() const
	{
		assert(mSceneManager != nullptr);
		return mSceneManager;
	}
	CollisionManager* GameProcessor::getCollisionManager() const
	{
		assert(mCollisionManager != nullptr);
		return mCollisionManager;
	}
	CameraManager* GameProcessor::getCameraManager() const
	{
		assert(mCameraManager != nullptr);
		return mCameraManager;
	}
}
