#include "pch.h"
#include "InputClass.h"

using namespace DirectX::SimpleMath;

constexpr float STICK_DEADZONE(0.2f);	// �ړ�����ƂȂ�X�e�B�b�N�̃f�b�h�]�[��

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

	if (gamePad->GetConnecting())
	{
		using enum GamePad::Value;
		using enum GamePad::Button;

		// ��
		u_int id = static_cast<u_int>(Move_Value::Up);
		moveFlags[id] =
			gamePad->GetValue(StickL_Y) > STICK_DEADZONE || gamePad->ButtonDown(Arrow_Up);

		// ��
		id = static_cast<u_int>(Move_Value::Down);
		moveFlags[id] =
			gamePad->GetValue(StickL_Y) < -STICK_DEADZONE || gamePad->ButtonDown(Arrow_Down);

		// �E
		id = static_cast<u_int>(Move_Value::Right);
		moveFlags[id] =
			gamePad->GetValue(StickL_X) > STICK_DEADZONE || gamePad->ButtonDown(Arrow_Right);

		// ��
		id = static_cast<u_int>(Move_Value::Left);
		moveFlags[id] =
			gamePad->GetValue(StickL_X) < -STICK_DEADZONE || gamePad->ButtonDown(Arrow_Left);
	}
	else
	{
		u_int id = static_cast<u_int>(Move_Value::Up);
		moveFlags[id] = keyboard->GetKeyDown(DIK_UPARROW);
	}
}
