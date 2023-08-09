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

		virtual void Init();
		virtual void Update();
		virtual void Release();

		inline UINT GetWidth() const;
		inline UINT GetHeight() const;
		inline const WCHAR* GetTitle() const;

		inline void SetHwnd(HWND hwnd);

	protected:
		inline TimeManager* getTimeManager() const;
		inline RenderManager* getRenderManager() const;
		inline SceneManager* getSceneManager() const;
		inline CollisionManager* getCollisionManager() const;
		inline CameraManager* getCameraManager() const;
		inline SoundManager* getSoundManager() const;

	private:
		UINT mWidth;
		UINT mHeight;
		std::wstring mTitle;
		HWND mHwnd;

		TimeManager* mTimeManager;
		RenderManager* mRenderManager;
		SceneManager* mSceneManager;
		CollisionManager* mCollisionManager;
		CameraManager* mCameraManager;
		SoundManager* mSoundManager;

		GameObject* mDefaultCamera;
	};

	void GameProcessor::SetHwnd(HWND hwnd)
	{
		mHwnd = hwnd;
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
	TimeManager* GameProcessor::getTimeManager() const
	{
		assert(mTimeManager != nullptr);
		return mTimeManager;
	}
	RenderManager* GameProcessor::getRenderManager() const
	{
		assert(mRenderManager != nullptr);
		return mRenderManager;
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
	SoundManager* GameProcessor::getSoundManager() const
	{
		assert(mSoundManager != nullptr);
		return mSoundManager;
	}
}
