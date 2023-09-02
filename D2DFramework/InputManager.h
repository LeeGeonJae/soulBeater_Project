#pragma once

#include "BaseEntity.h"
#include "eKeyState.h"

#include <Windows.h>

namespace d2dFramework
{
	class InputManager final : public BaseEntity
	{
		friend class GameProcessor;

	public:
		static InputManager* GetInstance();

		inline eKeyState GetKeyState(WORD keyCode) const;
		inline const POINT& GetMousePos() const;

	private:
		InputManager();
		~InputManager() = default;
		InputManager(const InputManager& other) = delete;
		InputManager operator=(const InputManager& other) = delete;

		void init(HWND hwnd);
		void release();
		void update();

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
}