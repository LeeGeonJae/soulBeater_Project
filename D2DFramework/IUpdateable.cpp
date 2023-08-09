#include "IUpdateable.h"

#include "SceneManager.h"

namespace d2dFramework
{
	SceneManager* IUpdateable::mSceneManager = nullptr;

	void IUpdateable::SetSceneManager(SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
	}

	void IUpdateable::Init()
	{
		mSceneManager->RegisterUpdateable(this);
	}

	void IUpdateable::Release()
	{
		mSceneManager->UnregisterUpdateable(this);
	}
}