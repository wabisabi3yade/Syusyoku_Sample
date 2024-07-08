#include "pch.h"
#include "GameKey.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

GameKey::GameKey()
{
	// 初期化
	memset(&keyState, false, sizeof(keyState));
	memset(&o_keyState, false, sizeof(o_keyState));
}

GameKey::~GameKey()
{
	// 解放処理
	inputDevice->Unacquire();
	inputDevice->Release();
}

bool GameKey::Init(HWND _hwnd)
{
	// インプットの作成
	HRESULT hr = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)(&input), NULL);
	if (FAILED(hr))
		return false;

	// キーデバイスの作成
	hr = input->CreateDevice(GUID_SysKeyboard, &inputDevice, NULL);
	if (FAILED(hr))
		return false;

	// キーフォーマットのセット
	hr = inputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false;

	// キーの協調レベルをセット
	hr = inputDevice->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false;

	// 入力デバイスへのアクセス権利を取得
	hr = inputDevice->Acquire();
	if (FAILED(hr))
		return false;

	return true;
}

void GameKey::InputUpdate()
{
	// １フレーム前のデータを保存
	memcpy_s(o_keyState, sizeof(o_keyState), keyState, sizeof(keyState));

	// キーボードデバイスの取得
	HRESULT hr = inputDevice->GetDeviceState(KEY_MAX, keyState);

	if (FAILED(hr))	// デバイスロスト時は再度制御開始する
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
