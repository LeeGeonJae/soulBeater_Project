#pragma once

// �׻� ����Ʈ ī�޶� �־�� ������ �� �� �ִ�.
// Ŭ����, ��ī��Ʈ ��ǥ -> ��ũ�� ��ȯ, ī�޶� ��ȯ ����� �����Ѵ�.\
// ������ ���������� ���⸸�� �䱸�ϹǷ� ������ �������� Ȱ���ϵ��� ó���ؾ� �ȴ�.

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