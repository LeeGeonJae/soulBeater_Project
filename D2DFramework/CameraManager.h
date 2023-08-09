#pragma once

// 항상 디폴트 카메라가 있어야 게임을 볼 수 있다.
// 클리핑, 데카르트 좌표 -> 스크린 변환, 카메라 변환 기능을 제공한다.\
// 렌더링 영역에서의 검출만을 요구하므로 렌더링 로직에서 활용하도록 처리해야 된다.

#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"

#include <cassert>
#include <d2d1.h>

namespace d2dFramework
{
	class Camera;

	class CameraManager
	{
	public:
		CameraManager();
		~CameraManager();

		inline void RegisterCamera(GameObject* camera);

		inline void SetScreenSize(const Vector2& size);

		inline const Vector2& GetScrennSize() const;
		inline D2D1::Matrix3x2F GetCombineMatrix() const;

	private:
		GameObject* mCurrnetCamara;
		Vector2 mSize;
	};

	void CameraManager::RegisterCamera(GameObject* camera)
	{
		assert(camera->GetComponent<Transform>() != nullptr);
		mCurrnetCamara = camera;
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
		Transform* transform = mCurrnetCamara->GetComponent<Transform>();
		D2D1::Matrix3x2F cameraToScreen =
			D2D1::Matrix3x2F::Scale(1, -1)
			* D2D1::Matrix3x2F::Translation(mSize.GetX() * 0.5f, mSize.GetY() * 0.5f);

		return transform->GetInverseTransform() * cameraToScreen;
	}
}