#pragma once
#include <dinput.h>

// �L�[�{�[�h���̓N���X
class GameKey
{
	static const u_int KEY_MAX = 256;	//�L�[�ő吔

	bool keyState[KEY_MAX];	// �L�[���͏��
	bool o_keyState[KEY_MAX];	// 1�t���[���O�̏��

	LPDIRECTINPUT8 input;	// �C���v�b�g
	LPDIRECTINPUTDEVICE8 key;	// �C���v�b�g�f�o�C�X

public:
	GameKey();
	~GameKey();

	bool Init(HWND _hwnd);	// ����������	

	void InputUpdate();

};

