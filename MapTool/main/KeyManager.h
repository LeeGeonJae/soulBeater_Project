#pragma once
#include "defalut.h"

enum KEY_STATE
{
	NONE,	// ������ �ʾҰ�, �������� ������ ���� ����
	TAP,	// �� ���� ����
	HOLD,	// ������ �ִ�
	AWAY,	// �� �� ����
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	N_1,
	N_2,
	N_3,
	N_4,
	N_5,
	N_6,
	N_7,
	N_8,
	N_9,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,
	LBTN,
	RBTN,

	LAST,
};

struct tKeyInfo
{
	KEY_STATE	eState;			// Ű�� ���°�
	bool		bPrevPush;		// ���� �����ӿ��� ���ȴ��� ����
};

class KeyManager
{
public:
	static KeyManager* m_pInstance;

public:
	KeyManager();
	~KeyManager();

public:
	void Initialize();
	void Update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }

private:
	vector<tKeyInfo> m_vecKey;
};