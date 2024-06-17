#pragma once
#include "GamePad.h"

// ゲームの入力全てをラップするクラス
class InputClass
{
	std::unique_ptr<GamePad> gamePad;	// ゲームパッド

public:
	InputClass();	
	~InputClass();

	void Update();	// 更新

	GamePad* GetGamePad()const { return gamePad.get(); }
};

