#pragma once

#include "GameObject.h"
#include "CameraComponent.h" // 강한 규칙 마련
#include "Transform.h"
#include "Vector2.h"
#include "BaseEntity.h"

#include <cassert>
#include <d2d1.h>

namespace d2dFramework
{
	class Camera;

	class CameraManager final : public BaseEntity
	{
	public:
		CameraManager();
		~CameraManager();
		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;

		void Release();

		inline void RegisterCamera(GameObject* camera);
		inline void UnregisterCamera(unsigned int id);

		inline void SetCurrentCamera(unsigned int cameraId);
		inline void SetScreenSize(const Vector2& size);

		inline const Vector2& GetScrennSize() const;
		inline D2D1::Matrix3x2F GetCombineMatrix() const;

	private:
		GameObject* mCurrnetCamara;
		Vector2 mSize;

		std::map<unsigned int, GameObject*> mCameraMaps;
	};

	void CameraManager::RegisterCamera(GameObject* camera)
	{
		mCameraMaps.insert({ camera->GetId(), camera });
	}

	void CameraManager::UnregisterCamera(unsigned int id)
	{
		mCameraMaps.erase(id);
	}

	void CameraManager::SetCurrentCamera(unsigned int cameraID)
	{
		auto find = mCameraMaps.find(cameraID);
		assert(find != mCameraMaps.end());

		mCurrnetCamara = find->second;
	}
	void CameraManager::SetScreenSize(const Vector2& size)
	{
		mSize = size;
	}

	const Vector2& CameraManager::GetScrennSize() const
	{
		return mSize;
	}
	D2D1::Matrix3x2F CameraManager::GetCombineMatrix() const
	{
		D2D1::Matrix3x2F cameraToScreen =
			D2D1::Matrix3x2F::Scale(1, -1)
			* D2D1::Matrix3x2F::Translation(mSize.GetX() * 0.5f, mSize.GetY() * 0.5f);

		if (mCurrnetCamara == nullptr)
		{
			return cameraToScreen;
		}

		Transform* transform = mCurrnetCamara->GetComponent<Transform>();

		return transform->GetInverseTransform() * cameraToScreen;
	}
}