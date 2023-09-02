#include "CameraManager.h"

#include "eFrameworkID.h"
#include "ObjectManager.h"

namespace d2dFramework
{
	CameraManager::CameraManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::CameraManager))
		, mCurrnetCamara(nullptr)
		, mSize()
		, mCameraMaps()
	{
	}

	CameraManager::~CameraManager()
	{
		Release();
	}

	void CameraManager::Release()
	{
		mCurrnetCamara = nullptr;
		mCameraMaps.clear();
	}
}