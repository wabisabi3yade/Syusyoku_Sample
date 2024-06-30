#pragma once
#include <dinput.h>

// キーボード入力クラス
class GameKey
{
	static const u_int KEY_MAX = 256;	//キー最大数

	unsigned char keyState[KEY_MAX];	// キー入力状態
	unsigned char o_keyState[KEY_MAX];	// 1フレーム前の状態

	LPDIRECTINPUT8 input;	// インプット
	LPDIRECTINPUTDEVICE8 inputDevice;	// インプットデバイス

public:
	GameKey();
	~GameKey();

	bool Init(HWND _hwnd);	// 初期化処理	

	void InputUpdate();

	bool GetKey(int _key)const;	// 押されているか
	bool GetKeyDown(int _key)const;	// 押した瞬間
	bool GetKeyUp(int _key)const;	// 離された瞬間
};

