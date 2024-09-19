#include "pch.h"
#include "GamePad.h"

void GamePad::DebugInput()
{
	std::string inputName;
	if (ImGuiMethod::TreeNode(TO_UTF8("�R���g���[���[����")))
	{
		for (int idx = 0; idx < Button::Button_Num; idx++)
		{
			if (!buttonState[idx]) continue;

			switch (idx)
			{
			case Button::Arrow_Up: inputName = ("Up");	break;

			case Button::Arrow_Down: inputName = ("Down"); break;

			case Button::Arrow_Right: inputName = ("Right"); break;

			case Button::Arrow_Left: inputName = ("Left"); break;

			case Button::Sankaku: inputName = ("�T���J�N"); break;

			case Button::Batsu: inputName = ("�o�c"); break;

			case Button::Maru: inputName = ("�}��"); break;

			case Button::Shikaku: inputName = ("�V�J�N"); break;

			case Button::R1: inputName = ("R1"); break;

			case Button::L1: inputName = ("R2"); break;

			case Button::R3: inputName = ("R3"); break;

			case Button::L3: inputName = ("L3"); break;

			case Button::Option: inputName = ("�I�v�V����"); break;

			case Button::Share: inputName = ("�V�F�A"); break;

			default: break;
			}

			ImGui::Text(ShiftJisToUtf8(inputName).c_str());
		}

		std::string inputVal;
		for (int idx = 0; idx < Value::Value_Num; idx++)
		{
			if (padValue[idx] == 0.0f) continue;

			switch (idx)
			{
			case Value::StickR_X:
			case Value::StickR_Y:
				inputName = "R�X�e�B�b�N"; 
				inputVal = " X�F " + std::to_string(padValue[Value::StickR_X]) + " Y�F " + std::to_string(padValue[Value::StickR_Y]);
				break;

			case Value::StickL_X:
			case Value::StickL_Y:
				inputName = "L�X�e�B�b�N�F";
				inputVal = " X�F " + std::to_string(padValue[Value::StickL_X]) + " Y�F " + std::to_string(padValue[Value::StickL_Y]);
				break;

			case Value::Trigger_R2: 
				inputName = "R2�F ";
				inputVal = std::to_string(padValue[idx]);
				break;

			case Value::Trigger_L2: 
				inputName = "L2�F";
				inputVal = std::to_string(padValue[idx]);
				break;

			default: break;
			}

			ImGui::Text((ShiftJisToUtf8(inputName + inputVal)).c_str());
		}

		ImGui::TreePop();
	}
}

GamePad::GamePad() :isResult(ERROR_SUCCESS)
{
	for (int idx = 0; idx < Button::Button_Num; idx++)
	{
		buttonState[idx] = false;
		o_buttonState[idx] = false;
	}

	for (int idx = 0; idx < Value::Value_Num; idx++)
	{
		padValue[idx] = 0.0f;
		o_padValue[idx] = 0.0f;
	}
}

GamePad::~GamePad()
{
}

void GamePad::InputUpdate()
{
	// �v���X�e���Ȃ��ݐ[���̂Ńv���X�e�̖��̂ł���

	// �Q�[���p�b�h�̓��͏�Ԃ��擾
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	isResult = XInputGetState(0, &state);

#ifdef EDIT
	static DWORD o_IsResult = 1;	//1�t���[���O�̏��
	if (isResult != o_IsResult)	// �f�o�b�O���O
	{
		if (isResult == ERROR_SUCCESS) HASHI_DEBUG_LOG("�Q�[���p�b�h���ڑ�����܂����I");
		else HASHI_DEBUG_LOG("�Q�[���p�b�h�Ɛڑ����؂�܂���");
	}
	o_IsResult = isResult; // �X�V
#endif // EDIT
	if (isResult != ERROR_SUCCESS) return;	// �R���g���[���[�ɂȂ����Ă��Ȃ��Ȃ�

	// �O�t���[���̏�Ԃ��X�V����
	memcpy_s(o_buttonState, sizeof(o_buttonState), buttonState, sizeof(buttonState));
	memcpy_s(o_padValue, sizeof(o_padValue), padValue, sizeof(padValue));
	// ��Ԃ����Z�b�g
	for (int idx = 0; idx < Button::Button_Num; idx++)
	{
		buttonState[idx] = false;
	}
	for (int idx = 0; idx < Value::Value_Num; idx++)
	{
		padValue[idx] = 0.0f;
	}

	constexpr int MAX_TRIGGER_VALUE = 255;	// �g���K�[�̍ő�
	constexpr int MAX_STICK_VALUE = 32768;	// �X�e�B�b�N�̍ő�
	// ���͎󂯂�
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) buttonState[Button::Arrow_Up] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) buttonState[Button::Arrow_Down] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) buttonState[Button::Arrow_Right] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) buttonState[Button::Arrow_Left] = true;

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) buttonState[Button::Sankaku] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) buttonState[Button::Batsu] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) buttonState[Button::Maru] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) buttonState[Button::Shikaku] = true;


	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) buttonState[Button::R1] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) buttonState[Button::L1] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) buttonState[Button::R3] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) buttonState[Button::L3] = true;

	if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	// �����т𒴂�����
	{
		padValue[Value::Trigger_R2] = static_cast<float>(state.Gamepad.bRightTrigger) / MAX_TRIGGER_VALUE;	// 0.0 �`1.0�͈̔͂ɂ����߂�
		buttonState[Button::R2] = true;
	}
	if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		padValue[Value::Trigger_L2] = static_cast<float>(state.Gamepad.bLeftTrigger) / MAX_TRIGGER_VALUE;
		buttonState[Button::L2] = true;
	}

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)	buttonState[Button::Option] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) buttonState[Button::Share] = true;

	// �X�e�B�b�N�̃f�b�h�]�[���𒴂�����
	if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		// �����͕͂��ɔ�ׂ�1�������l�ɂȂ�̂�1����
		long int valX = state.Gamepad.sThumbRX;	// �I�[�o�[�t���[�΍�
		long int valY = state.Gamepad.sThumbRY;
		// ���̕�����1�������̂�
		if (state.Gamepad.sThumbRX > 0)
		{
			valX += 1;
		}
		if (state.Gamepad.sThumbRY > 0)
		{
			valY += 1;
		}

		// -1.0�`1,0�͈̔͂ɂ����߂�
		padValue[Value::StickR_X] = static_cast<float>(valX) / MAX_STICK_VALUE;
		padValue[Value::StickR_Y] = static_cast<float>(valY) / MAX_STICK_VALUE;
	}

	if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		// �����͕͂��ɔ�ׂ�1�������l�ɂȂ�̂�1����
		long int valX = state.Gamepad.sThumbLX;	// �I�[�o�[�t���[�΍�
		long int valY = state.Gamepad.sThumbLY;
		if (state.Gamepad.sThumbLX > 0)
		{
			valX += 1;
		}
		if (state.Gamepad.sThumbLY > 0)
		{
			valY += 1;
		}

		// -1.0�`1,0�͈̔͂ɂ����߂�
		padValue[Value::StickL_X] = static_cast<float>(valX) / MAX_STICK_VALUE;
		padValue[Value::StickL_Y] = static_cast<float>(valY) / MAX_STICK_VALUE;
	}

}

bool GamePad::ButtonPress(const Button& _getButton) const
{
	if (_getButton < 0 || _getButton > Button::Button_Num)
	{
		HASHI_DEBUG_LOG("ButtonPress�F�w�肵���l���Ⴂ�܂�");
		return false;
	}

	return buttonState[_getButton];
}

bool GamePad::ButtonDown(const Button& _getButton) const
{
	if (_getButton < 0 || _getButton >  Button::Button_Num)
	{
		HASHI_DEBUG_LOG("ButtonDown�F�w�肵���l���Ⴂ�܂�");
		return false;
	}
	return (buttonState[_getButton] && !o_buttonState[_getButton]);
}

bool GamePad::ButtonUp(const Button& _getButton) const
{
	if (_getButton < 0 || _getButton >  Button::Button_Num)
	{
		HASHI_DEBUG_LOG("ButtonUp�F�w�肵���l���Ⴂ�܂�");
		return false;
	}
	return (!buttonState[_getButton] && o_buttonState[_getButton]);
}

float GamePad::GetValue(const Value& _getValue) const
{
	if (_getValue < 0 || _getValue > Value::Value_Num)
	{
		HASHI_DEBUG_LOG("GetValue�F�w�肵���l���Ⴂ�܂�");
		return 0.0f;
	}
	return padValue[_getValue];
}
