#pragma once

#include "BaseEntity.h"

#include <Windows.h>

namespace d2dFramework
{
	enum class eKeyState
	{
		None,
		Push,
		Hold,
		Pop,
	};

	class InputManager final : public BaseEntity
	{
		friend class GameProcessor;

	public:
		static InputManager* GetInstance();

		inline eKeyState GetKeyState(WORD keyCode) const;
		inline const POINT& GetMousePos() const;
		inline void SetHwnd(HWND Hwnd);

	private:
		InputManager();
		~InputManager() = default;
		InputManager(const InputManager& other) = delete;
		InputManager operator=(const InputManager& other) = delete;

		void Init();
		void Update();

	private:
		enum { KEY_STATES_SIZE = 256 };

		static InputManager* mInstance;
		eKeyState mKeyStates[KEY_STATES_SIZE];
		POINT mMousePos;

		// 추가 : 이건재
		HWND mHwnd;
	};

	eKeyState InputManager::GetKeyState(WORD keyCode) const
	{
		return mKeyStates[keyCode];
	}

	const POINT& InputManager::GetMousePos() const
	{
		return mMousePos;
	}

	void InputManager::SetHwnd(HWND Hwnd)
	{
		mHwnd = Hwnd;
	}
}