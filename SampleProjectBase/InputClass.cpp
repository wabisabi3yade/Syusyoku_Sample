#include "pch.h"
#include "InputClass.h"

using namespace DirectX::SimpleMath;

InputClass::InputClass()
{
	gamePad = std::make_unique<GamePad>();
	keyboard = std::make_unique<GameKey>();

	inputValue.emplace("Left", Vector2::Zero);
	inputValue.emplace("Right", Vector2::Zero);
}

InputClass::~InputClass()
{
}

void InputClass::Init(HWND _hwnd)
{
	keyboard->Init(_hwnd);
}

void InputClass::Update()
{
	gamePad->InputUpdate();
	keyboard->InputUpdate();

	// スティック値を更新する
	Vector2 vecR;
	Vector2 vecL;
	if (gamePad->GetConnecting())	// ゲームパッドが繋がっている
	{
		vecR.x = gamePad->GetValue(GamePad::Value::StickR_X);
		vecR.y = gamePad->GetValue(GamePad::Value::StickR_Y);

		vecL.x = gamePad->GetValue(GamePad::Value::StickL_X);
		vecL.y = gamePad->GetValue(GamePad::Value::StickL_Y);
	}
	else	// キーボード入力
	{
		// Rスティックと同様
		if (keyboard->GetKey(DIK_UP))
		{
			vecR.y += 1.0f;
		}
		if ((keyboard->GetKey(DIK_DOWN)))
		{
			vecR.y += -1.0f;
		}
		if ((keyboard->GetKey(DIK_RIGHT)))
		{
			vecR.x += 1.0f;
		}
		if ((keyboard->GetKey(DIK_LEFT)))
		{
			vecR.x += -1.0f;
		}
		// Lスティックと同様
		if (keyboard->GetKey(DIK_W))
		{
			vecL.y += 1.0f;
		}
		if ((keyboard->GetKey(DIK_S)))
		{
			vecL.y += -1.0f;
		}
		if ((keyboard->GetKey(DIK_D)))
		{
			vecL.x += 1.0f;
		}
		if ((keyboard->GetKey(DIK_A)))
		{
			vecL.x += -1.0f;
		}
	}
	// 値代入
	inputValue["Right"] = vecR;
	inputValue["Left"] = vecL;
}

Vector2 InputClass::GetValue(const std::string& _getType) const
{
	if (!inputValue.contains(_getType))
	{
		HASHI_DEBUG_LOG("入力値の取得名が間違っています");
		return Vector2::Zero;
	}

	return inputValue.at(_getType);
}
