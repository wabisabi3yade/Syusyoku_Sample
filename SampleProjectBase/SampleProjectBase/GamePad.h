#pragma once
#include <xinput.h>
#pragma comment (lib, "xinput.lib")

// ゲームパッドクラス
class GamePad	
{

public:
	enum Button	// ボタン
	{
		// 十字
		Arrow_Up = 0,
		Arrow_Down,
		Arrow_Right,
		Arrow_Left,

		// 記号
		Sankaku,
		Batsu,
		Maru,
		Shikaku,

		// RL
		R1,
		L1,
		R3,
		L3,

		// オプション
		Option,
		Share,

		Button_Num
	};

	enum Value	// 傾きをとる
	{
		// スティック
		StickR_X = 0,
		StickR_Y,
		StickL_X,
		StickL_Y,

		// トリガー
		Trigger_R2,
		Trigger_L2,

		Value_Num
	};

private:
	DWORD isResult;	// 今の状態

	bool buttonState[static_cast<int>(Button::Button_Num)];	// 現在のボタン状態
	bool o_buttonState[static_cast<int>(Button::Button_Num)];	// 1つ前のボタン状態

	float padValue[static_cast<int>(Value::Value_Num)];
	float o_padValue[static_cast<int>(Value::Value_Num)];
public:
	GamePad();
	~GamePad();

	void InputUpdate();	// 入力の更新

	bool ButtonPress(const Button& _getButton)const;	// 押されているか(継続)
	bool ButtonDown(const Button& _getButton)const;	// 押した瞬間(瞬間)
	bool ButtonUp(const Button& _getButton)const;	// 離れた瞬間(瞬間)

	float GetValue(const Value& _getValue)const;

	void DebugInput();	// デバッグ確認

	bool GetConnecting() { return (isResult == ERROR_SUCCESS); }	// 繋がってるか確認する
};

