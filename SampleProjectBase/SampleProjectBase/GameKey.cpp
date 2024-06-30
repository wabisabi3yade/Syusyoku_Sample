#include "pch.h"
#include "GameKey.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

GameKey::GameKey()
{
	// ������
	memset(&keyState, false, sizeof(keyState));
	memset(&o_keyState, false, sizeof(o_keyState));
}

GameKey::~GameKey()
{
	// �������
	inputDevice->Unacquire();
	inputDevice->Release();
}

bool GameKey::Init(HWND _hwnd)
{
	// �C���v�b�g�̍쐬
	HRESULT hr = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)(&input), NULL);
	if (FAILED(hr))
		return false;

	// �L�[�f�o�C�X�̍쐬
	hr = input->CreateDevice(GUID_SysKeyboard, &inputDevice, NULL);
	if (FAILED(hr))
		return false;

	// �L�[�t�H�[�}�b�g�̃Z�b�g
	hr = inputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false;

	// �L�[�̋������x�����Z�b�g
	hr = inputDevice->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false;

	// ���̓f�o�C�X�ւ̃A�N�Z�X�������擾
	hr = inputDevice->Acquire();
	if (FAILED(hr))
		return false;

	return true;
}

void GameKey::InputUpdate()
{
	// �P�t���[���O�̃f�[�^��ۑ�
	memcpy_s(o_keyState, sizeof(o_keyState), keyState, sizeof(keyState));

	// �L�[�{�[�h�f�o�C�X�̎擾
	HRESULT hr = inputDevice->GetDeviceState(KEY_MAX, keyState);

	if (FAILED(hr))	// �f�o�C�X���X�g���͍ēx����J�n����
	{
		inputDevice->Acquire();
	}
}

bool GameKey::GetKey(int _key)const
{
	return (keyState[_key] & 0x80);
}

bool GameKey::GetKeyDown(int _key)const
{
	return ((keyState[_key] & 0x80) && !(o_keyState[_key] & 0x80));
}

bool GameKey::GetKeyUp(int _key)const
{
	return (!(keyState[_key] & 0x80) && (o_keyState[_key] & 0x80));
}
