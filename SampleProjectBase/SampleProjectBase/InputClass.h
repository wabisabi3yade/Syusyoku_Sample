#pragma once
#include "GamePad.h"
#include "GameKey.h"

// ゲームの入力全てをラップするクラス
class InputClass
{
	std::unique_ptr<GamePad> gamePad;	// ゲームパッド
	std::unique_ptr<GameKey> keyboard;	// キーボード
public:
	InputClass();	
	~InputClass();

	void Update();	// 更新

	GamePad* GetGamePad()const { return gamePad.get(); }
};

