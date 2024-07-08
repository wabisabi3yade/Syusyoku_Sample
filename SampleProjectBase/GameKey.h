#pragma once
#include <dinput.h>

// �L�[�{�[�h���̓N���X
class GameKey
{
	static const u_int KEY_MAX = 256;	//�L�[�ő吔

	unsigned char keyState[KEY_MAX];	// �L�[���͏��
	unsigned char o_keyState[KEY_MAX];	// 1�t���[���O�̏��

	LPDIRECTINPUT8 input;	// �C���v�b�g
	LPDIRECTINPUTDEVICE8 inputDevice;	// �C���v�b�g�f�o�C�X

public:
	GameKey();
	~GameKey();

	bool Init(HWND _hwnd);	// ����������	

	void InputUpdate();

	bool GetKey(int _key)const;	// ������Ă��邩
	bool GetKeyDown(int _key)const;	// �������u��
	bool GetKeyUp(int _key)const;	// �����ꂽ�u��
};

