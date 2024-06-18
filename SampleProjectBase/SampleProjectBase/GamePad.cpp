#include "pch.h"
#include "GamePad.h"

GamePad::GamePad() :isResult(ERROR_SUCCESS)
{
}

GamePad::~GamePad()
{
}

void GamePad::InputUpdate()
{
	// プレステがなじみ深いのでプレステの名称でいく

	// ゲームパッドの入力状態を取得
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	isResult = XInputGetState(0, &state);

#ifdef _DEBUG
	static DWORD o_IsResult = 1;	//1フレーム前の状態
	if (isResult != o_IsResult)	// デバッグログ
	{
		if (isResult) ImGuiDebugLog::Add("ゲームパッドが接続されました！");
		else ImGuiDebugLog::Add("ゲームパッドと接続が切れました");
	}
	o_IsResult = isResult; // 更新
#endif // _DEBUG
	if (isResult != ERROR_SUCCESS) return;	// コントローラーにつながっていないなら

	// 前フレームの状態を更新する
	memcpy_s(o_buttonState, sizeof(o_buttonState), buttonState, sizeof(buttonState));
	memcpy_s(o_padValue, sizeof(o_padValue), padValue, sizeof(padValue));
	// 状態をリセット
	for (int idx = 0; idx < Button::Button_Num; idx++)
	{
		buttonState[idx] = false;
	}
	for (int idx = 0; idx < Value::Value_Num; idx++)
	{
		padValue[idx] = 0.0f;
	}

	constexpr int MAX_TRIGGER_VALUE = 255;	// トリガーの最大
	constexpr int MAX_STICK_VALUE = 32768;	// スティックの最大
	// 入力受けつけ
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) buttonState[Button::Arrow_Up] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) buttonState[Button::Arrow_Down] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) buttonState[Button::Arrow_Right] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) buttonState[Button::Arrow_Left] = true;

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) buttonState[Button::Sankaku] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) buttonState[Button::Batsu] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) buttonState[Button::Maru] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) buttonState[Button::Shikaku] = true;


	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)buttonState[Button::R1] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) buttonState[Button::L1] = true;
	if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	// あそびを超えたら
	{
		padValue[Value::Trigger_R2] = state.Gamepad.bRightTrigger / MAX_TRIGGER_VALUE;	// 0.0 ～1.0の範囲におさめる
		buttonState[Button::R2] = true;
	}
	if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	
	{
		padValue[Value::Trigger_L2] = state.Gamepad.bLeftTrigger / MAX_TRIGGER_VALUE;
		buttonState[Button::L2] = true;
	}

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)	buttonState[Button::Option] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) buttonState[Button::Share] = true;

	// スティックのデッドゾーンを超えたら
	if ((state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ||
		(state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		// -1.0～1,0の範囲におさめる
		padValue[Value::StickR_X] = state.Gamepad.sThumbLX / MAX_STICK_VALUE; 
		padValue[Value::StickR_Y] = state.Gamepad.sThumbLY / MAX_STICK_VALUE;
	}
	if ((state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ||
		(state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		// -1.0～1,0の範囲におさめる
		padValue[Value::StickL_X] = state.Gamepad.sThumbLX / MAX_STICK_VALUE; 
		padValue[Value::StickL_Y] = state.Gamepad.sThumbLY / MAX_STICK_VALUE;
	}

}

bool GamePad::ButtonPress(const Button& _getButton) const
{
	if (_getButton < 0 || _getButton > Button::Button_Num)
	{
		ImGuiDebugLog::Add("ButtonPress：指定した値が違います");
		return false;
	}

	return padValue[_getButton];
}

bool GamePad::ButtonDown(const Button& _getButton) const
{
	if (_getButton < 0 || _getButton >  Button::Button_Num)
	{
		ImGuiDebugLog::Add("ButtonDown：指定した値が違います");
		return false;
	}
	return (padValue[_getButton] && !o_padValue[_getButton]);
}

bool GamePad::ButtonUp(const Button& _getButton) const
{
	if(_getButton < 0 || _getButton >  Button::Button_Num)
	{
		ImGuiDebugLog::Add("ButtonUp：指定した値が違います");
		return false;
	}
	return padValue[_getButton];
}

float GamePad::GetValue(const Value& _getValue) const
{
	if (_getValue < 0 || _getValue > Value::Value_Num)
	{
		ImGuiDebugLog::Add("GetValue：指定した値が違います");
		return 0.0f;
	}
	return padValue[_getValue];
}
