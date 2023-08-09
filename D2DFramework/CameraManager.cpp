#include "CameraManager.h"
#include "ObjectManager.h"

namespace d2dFramework
{
	CameraManager::CameraManager()
		: mCurrnetCamara(nullptr)
	{
	}

	CameraManager::~CameraManager()
	{
		ObjectManager::GetInstance()->DeletObject(mCurrnetCamara->GetId());
	}
}