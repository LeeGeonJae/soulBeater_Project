#include "KeyManager.h"

KeyManager* KeyManager::m_pInstance = nullptr;

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',

	VK_MENU,	// ALT
	VK_CONTROL,	// CTRL
	VK_LSHIFT,	// LSHIFT
	VK_SPACE,	// SPACE BAR
	VK_RETURN,	// ENTER
	VK_ESCAPE,	// ESC

	VK_LBUTTON,
	VK_RBUTTON,
};

KeyManager::KeyManager()
	:m_vecKey{}
{
	m_pInstance = this;
}

KeyManager::~KeyManager()
{

}

void KeyManager::Initialize()
{
	// 키의 모든 상태를 false로 세팅
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void KeyManager::Update()
{
	// 윈도우 포커싱 알아내기
	//HWND hMainWnd = CCore::GetInstance()->GetMainHWnd();

	// 현재 포커싱 되어 있는 윈도우의 핸들값을 알려준다
	// 현재 우리가 가지고 있는 원도우들의 핸들을 가지고 있지 않으면 nullptr이 반환된다
	HWND hWnd = GetFocus();

	// 윈도우 포커싱 중일 때 키 이벤트 동적
	// 윈도우 창이 포커싱 중이지 않을 때 동작하지 않는다
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌려있다
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에도 눌려 있었다.
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다.
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				m_vecKey[i].bPrevPush = true;
			}

			// 키가 안눌려있다
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에 눌려 있었다.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 눌려있지 않았다.
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPush = false;
			}
		}
	}

	// 윈도우 포커싱 해제상태
	// 이것을 해주지 않으면 다른 창이 포커싱된 상태에서도 키가 입력되게 된다
	else
	{
		// 만약에 윈도우 포커싱이 해제된 상태라면 모든 키의 상태를 AWAY와 NONE 상태로 전환하고 bPrevPush를 false로 세팅
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;

			if (m_vecKey[i].eState == KEY_STATE::TAP || m_vecKey[i].eState == KEY_STATE::HOLD)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (m_vecKey[i].eState == KEY_STATE::AWAY)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}
}
