#include "IFixedUpdateable.h"

#include "SceneManager.h"

namespace d2dFramework
{
	SceneManager* IFixedUpdateable::mSceneManager = nullptr;

	void IFixedUpdateable::SetSceneManager(SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
	}

	void IFixedUpdateable::Init()
	{
		mSceneManager->RegisterFixedUpdateable(this);
	}

	void IFixedUpdateable::Release()
	{
		mSceneManager->UnregisterFixedUpdateable(this);
	}
}