#include "pch.h"
#include "GameInput.h"

using namespace DirectX::SimpleMath;

/// @brief この時間を超えると入力履歴を削除する
constexpr float DELETE_TIMELINE_SEC(2.0f);

/// @brief 数えていない時はこの値
constexpr float ON_NOTCOUNT_TIME(100.0f);


GameInput::GameInput()
{
	// ボタンデフォルト設定
	DefaultSetting();

	// スティック数値初期設定
	InitValueSettings();

	// 初期時間をセット
	for (auto& time : buttonTimeLine)
		time = ON_NOTCOUNT_TIME;
}

void GameInput::Update()
{
	// ボタン入力の更新
	ButtonUpdate();

	// スティック入力の更新
	ValueUpdate();

	// 入力履歴更新
	TimeLineUpdate();
}

bool GameInput::GetButton(ButtonType _buttonType)
{
	u_int buttonNum = static_cast<u_int>(_buttonType);
	return c_buttonState[buttonNum];
}

bool GameInput::GetButtonDown(ButtonType _buttonType)
{
	u_int buttonNum = static_cast<u_int>(_buttonType);

	return (c_buttonState[buttonNum] && !p_buttonState[buttonNum]);
}

bool GameInput::GetButtonUp(ButtonType _buttonType)
{
	u_int buttonNum = static_cast<u_int>(_buttonType);

	return (!c_buttonState[buttonNum] && p_buttonState[buttonNum]);
}

bool GameInput::GetButtonDown(ButtonType _buttonType, float _withinTime)
{
	return buttonTimeLine[static_cast<u_int>(_buttonType)] < _withinTime;
}

DirectX::SimpleMath::Vector2 GameInput::GetValue(ValueType _valueType)
{
	return c_valueState[static_cast<u_int>(_valueType)];
}

void GameInput::DefaultSetting()
{
	using enum ButtonType;
	using enum GamePad::Button;
	buttonLinks[static_cast<u_int>(Player_Attack)] = { Sankaku, DIK_RETURN };
	buttonLinks[static_cast<u_int>(Player_Jump)] = { Batsu, DIK_SPACE };
	buttonLinks[static_cast<u_int>(Player_Rolling)] = { Shikaku, DIK_LSHIFT };
	buttonLinks[static_cast<u_int>(Player_RockOn)] = { R1, DIK_LSHIFT };
}

void GameInput::InitValueSettings()
{
	using enum GamePad::Value;
	// スティック入力は基本変更しない
	valueLinks[static_cast<u_int>(ValueType::Player_Move)] =
	{
		StickL_X, StickL_Y,
		DIK_D, DIK_A, DIK_W, DIK_S
	};

	// スティック入力は基本変更しない
	valueLinks[static_cast<u_int>(ValueType::Camera_Move)] =
	{
		StickR_X, StickR_Y,
		DIK_RIGHT, DIK_LEFT, DIK_UP, DIK_DOWN
	};
}

bool GameInput::IsGamePadConnect()
{
	return MainApplication::GetInput().GetGamePad().GetConnecting();
}

void GameInput::ButtonUpdate()
{
	// 前フレームの状態を更新する
	memcpy_s(p_buttonState, sizeof(p_buttonState), c_buttonState, sizeof(c_buttonState));

	InputClass& input = MainApplication::GetInput();

	if (IsGamePadConnect()) // ゲームパッドがつながっているなら
	{
		const GamePad& pad = input.GetGamePad();
		for (u_int b_i = 0; b_i < BUTTON_TYPE_CNT; b_i++)
		{
			c_buttonState[b_i] = pad.ButtonPress(buttonLinks[b_i].padType);
		}
	}
	else	// キーボード
	{
		const GameKey& keyboard = input.GetKeyboard();
		for (u_int b_i = 0; b_i < BUTTON_TYPE_CNT; b_i++)
		{
			c_buttonState[b_i] = keyboard.GetKey(buttonLinks[b_i].keyType);
		}
	}
}

void GameInput::ValueUpdate()
{
	InputClass& input = MainApplication::GetInput();
	u_int maxNum = static_cast<u_int>(ValueType::Max_ValueNum);

	if (IsGamePadConnect()) // ゲームパッドがつながっているなら
	{
		const GamePad& pad = input.GetGamePad();
		for (u_int v_i = 0; v_i < maxNum; v_i++)
		{
			c_valueState[v_i].x = pad.GetValue(valueLinks[v_i].padValue_x);
			c_valueState[v_i].y = pad.GetValue(valueLinks[v_i].padValue_y);
		}
	}
	else	// キーボード
	{
		const GameKey& keyboard = input.GetKeyboard();

		for (u_int v_i = 0; v_i < maxNum; v_i++)
		{
			c_valueState[v_i] = Vector2::Zero;

			// →
			if (keyboard.GetKey(valueLinks[v_i].keyType_x_positive))
				c_valueState[v_i].x += 1.0f;

			// ←
			if (keyboard.GetKey(valueLinks[v_i].keyType_x_negative))
				c_valueState[v_i].x += -1.0f;

			// ↑
			if (keyboard.GetKey(valueLinks[v_i].keyType_y_positive))
				c_valueState[v_i].y += 1.0f;

			// ↓
			if (keyboard.GetKey(valueLinks[v_i].keyType_y_negative))
				c_valueState[v_i].y += -1.0f;
		}
	}
}

void GameInput::TimeLineUpdate()
{
	float deltaTime = MainApplication::DeltaTime();

	// 削除確認と時間経過
	for (u_int b_i = 0; b_i < BUTTON_TYPE_CNT; b_i++)
	{
		buttonTimeLine[b_i] += deltaTime;
		// 時間が超えていたら数えないように
		if (buttonTimeLine[b_i] > DELETE_TIMELINE_SEC)
			buttonTimeLine[b_i] = ON_NOTCOUNT_TIME;

		// トリガー入力がされたら
		if (c_buttonState[b_i] && !p_buttonState[b_i])
			buttonTimeLine[b_i] = 0.0f;
	}
}
