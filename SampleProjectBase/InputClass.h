#pragma once
#include "GamePad.h"
#include "GameKey.h"

namespace HashiTaku
{
	// ゲームの入力全てをラップするクラス
	class InputClass
	{
	public:
		/// @brief 入力方向
		enum class InputDirection
		{
			Up,
			Down,
			Right,
			Left,
			MaxNum
		};

	private:
		std::unique_ptr<GamePad> gamePad;	// ゲームパッド
		std::unique_ptr<GameKey> keyboard;	// キーボード

	public:
		InputClass();
		~InputClass();

		void Init(HWND _hwnd);

		void Update();	// 更新

		GamePad& GetGamePad()const { return *gamePad.get(); }	// コントローラ入力

		const GameKey& GetKeyboard()const { return *keyboard.get(); }	// キーボード入力

		/// @brief 入力で方向のボタンが押されたか取得する
		/// @param _getDir 取得したい方向
		/// @return 押した瞬間か？
		bool GetInputTriggerDir(InputDirection _getDir) const;
	};
}
