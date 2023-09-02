#include "InputManager.h"

#include "eFrameworkID.h"

#include <cassert>
#include <Windows.h>

namespace d2dFramework
{
	InputManager* InputManager::mInstance = nullptr;

	InputManager* InputManager::GetInstance()
	{
		assert(mInstance != nullptr);
		return mInstance;
	}

	InputManager::InputManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::InputManager))
		, mKeyStates{ eKeyState::None, }
		, mMousePos{}
		, mHwnd(NULL)
	{
	}

	void InputManager::init(HWND hwnd)
	{
		mHwnd = hwnd;
		memset(&mMousePos, 0, sizeof(mMousePos));
		memset(mKeyStates, static_cast<int>(eKeyState::None), KEY_STATES_SIZE * sizeof(eKeyState));
	}

	void InputManager::release()
	{
		mHwnd = NULL;
	}

	void InputManager::update()
	{
		POINT curMousePos;
		GetCursorPos(&curMousePos);

		if (ScreenToClient(mHwnd, &curMousePos) != false)
		{
			mMousePos = curMousePos;
		}

		for (int i = 0; i < KEY_STATES_SIZE; ++i)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				switch (mKeyStates[i])
				{
				case eKeyState::None:
					/* intentional fall-through */
				case eKeyState::Pop:
					mKeyStates[i] = eKeyState::Push;
					break;
				case eKeyState::Push:
					/* intentional fall-through */
				case eKeyState::Hold:
					mKeyStates[i] = eKeyState::Hold;
					break;
				default:
					assert(false);
					break;
				}
			}
			else
			{
				switch (mKeyStates[i])
				{
				case eKeyState::None:
					/* intentional fall-through */
				case eKeyState::Pop:
					mKeyStates[i] = eKeyState::None;
					break;
				case eKeyState::Push:
					/* intentional fall-through */
				case eKeyState::Hold:
					mKeyStates[i] = eKeyState::Pop;
					break;
				default:
					assert(false);
					break;
				}
			}
		}
	}
}