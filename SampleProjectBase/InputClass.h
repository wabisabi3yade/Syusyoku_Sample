#pragma once
#include "GamePad.h"
#include "GameKey.h"

// ゲームの入力全てをラップするクラス
class InputClass
{
	enum class Move_Value
	{
		Up,
		Down,
		Right,
		Left,
		MaxNum
	};

	std::unique_ptr<GamePad> gamePad;	// ゲームパッド
	std::unique_ptr<GameKey> keyboard;	// キーボード
	
	/// @brief 移動関連はこ
	std::array<bool, static_cast<u_int>(Move_Value::MaxNum)> moveFlags;
public:
	InputClass();	
	~InputClass();

	void Init(HWND _hwnd);

	void Update();	// 更新

	const GamePad& GetGamePad()const { return *gamePad.get(); }	// コントローラ入力

	const GameKey& GetKeyboard()const { return *keyboard.get(); }	// キーボード入力
};

