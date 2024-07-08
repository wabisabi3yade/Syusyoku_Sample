#pragma once
#include "GamePad.h"
#include "GameKey.h"

// ゲームの入力全てをラップするクラス
class InputClass
{
	std::unique_ptr<GamePad> gamePad;	// ゲームパッド
	std::unique_ptr<GameKey> keyboard;	// キーボード

	// パッドとキーボード入力の両対応させた入力値を格納する
	std::unordered_map<std::string, DirectX::SimpleMath::Vector2> inputValue;
public:
	InputClass();	
	~InputClass();

	void Init(HWND _hwnd);

	void Update();	// 更新

	const GamePad& GetGamePad()const { return *gamePad.get(); }	// コントローラ入力

	const GameKey& GetKeyboard()const { return *keyboard.get(); }	// キーボード入力
	
	DirectX::SimpleMath::Vector2 GetValue(const std::string& _getType)const;
};

