#include "pch.h"
#include "InputClass.h"

namespace HashiTaku
{
	InputClass::InputClass()
	{
		gamePad = std::make_unique<GamePad>();
		keyboard = std::make_unique<GameKey>();
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
	}

	bool InputClass::GetInputTriggerDir(InputDirection _getDir) const
	{
		// コントローラの入力
		if (gamePad->GetConnecting())
		{
			using enum GamePad::Value;
			using enum GamePad::PadFlag;

			switch (_getDir)
			{
			case InputClass::InputDirection::Up:
				return gamePad->InputTrigger(StickL_Up) ||
					gamePad->InputTrigger(Arrow_Up);

			case InputClass::InputDirection::Down:
				return  gamePad->InputTrigger(StickL_Down) ||
					gamePad->InputTrigger(Arrow_Down);

			case InputClass::InputDirection::Right:
				return gamePad->InputTrigger(StickL_Right) ||
					gamePad->InputTrigger(Arrow_Right);

			case InputClass::InputDirection::Left:
				return gamePad->InputTrigger(StickL_Left) ||
					gamePad->InputTrigger(Arrow_Left);

			default:
				break;
			}
		}
		else	// キーボード入力
		{
			switch (_getDir)
			{
			case InputClass::InputDirection::Up:
				return keyboard->GetKeyDown(DIK_UP);

			case InputClass::InputDirection::Down:
				return keyboard->GetKeyDown(DIK_DOWN);

			case InputClass::InputDirection::Right:
				return keyboard->GetKeyDown(DIK_RIGHT);

			case InputClass::InputDirection::Left:
				return keyboard->GetKeyDown(DIK_LEFT);

			default:
				break;
			}
		}

		return false;
	}
}