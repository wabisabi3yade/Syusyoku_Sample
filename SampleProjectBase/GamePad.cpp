#include "pch.h"
#include "GamePad.h"

constexpr int STICK_FLAG_THRESHOLD(10000);	// �X�e�B�b�N�̃t���O���|�ꂽ�Ƃ���臒l

GamePad::GamePad() :isResult(ERROR_SUCCESS)
{
	for (int idx = 0; idx < PadFlag::Button_Num; idx++)
	{
		buttonState[idx] = false;
		o_buttonState[idx] = false;
	}

	for (int idx = 0; idx < Value::Value_Num; idx++)
	{
		padValue[idx] = 0.0f;
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
	// ��Ԃ����Z�b�g
	for (int idx = 0; idx < PadFlag::Button_Num; idx++)
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
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) buttonState[PadFlag::Arrow_Up] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) buttonState[PadFlag::Arrow_Down] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) buttonState[PadFlag::Arrow_Right] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) buttonState[PadFlag::Arrow_Left] = true;

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) buttonState[PadFlag::Sankaku] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) buttonState[PadFlag::Batsu] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) buttonState[PadFlag::Maru] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) buttonState[PadFlag::Shikaku] = true;


	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) buttonState[PadFlag::R1] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) buttonState[PadFlag::L1] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) buttonState[PadFlag::R3] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) buttonState[PadFlag::L3] = true;

	if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	// �����т𒴂�����
	{
		padValue[Value::Trigger_R2] = static_cast<float>(state.Gamepad.bRightTrigger) / MAX_TRIGGER_VALUE;	// 0.0 �`1.0�͈̔͂ɂ����߂�
		buttonState[PadFlag::R2] = true;
	}
	if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		padValue[Value::Trigger_L2] = static_cast<float>(state.Gamepad.bLeftTrigger) / MAX_TRIGGER_VALUE;
		buttonState[PadFlag::L2] = true;
	}

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)	buttonState[PadFlag::Option] = true;
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) buttonState[PadFlag::Share] = true;

	// �X�e�B�b�N�̃f�b�h�]�[���𒴂�����
	if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		// �����͕͂��ɔ�ׂ�1�������l�ɂȂ�̂�1����
		long int valX = state.Gamepad.sThumbRX;	// �I�[�o�[�t���[�΍�
		long int valY = state.Gamepad.sThumbRY;
		// ���̕�����1�������̂�
		if (state.Gamepad.sThumbRX > 0)
		{
			valX += 1;
			if (state.Gamepad.sThumbRX > STICK_FLAG_THRESHOLD)
				buttonState[PadFlag::StickR_Right] = true;
		}
		else if (state.Gamepad.sThumbRX < -1 * STICK_FLAG_THRESHOLD)
		{
			buttonState[PadFlag::StickR_Left] = true;
		}

		if (state.Gamepad.sThumbRY > 0)
		{
			valY += 1;
			if (state.Gamepad.sThumbRY > STICK_FLAG_THRESHOLD)
				buttonState[PadFlag::StickR_Up] = true;
		}
		else if (state.Gamepad.sThumbRY < -1 * STICK_FLAG_THRESHOLD)
		{
			buttonState[PadFlag::StickR_Down] = true;
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
			if (state.Gamepad.sThumbLX > STICK_FLAG_THRESHOLD)
				buttonState[PadFlag::StickL_Right] = true;
		}
		else if (state.Gamepad.sThumbLX < -1 * STICK_FLAG_THRESHOLD)
		{
			buttonState[PadFlag::StickL_Left] = true;
		}


		if (state.Gamepad.sThumbLY > 0)
		{
			valY += 1;
			if (state.Gamepad.sThumbLY > STICK_FLAG_THRESHOLD)
				buttonState[PadFlag::StickL_Up] = true;
		}
		else if (state.Gamepad.sThumbLY < -1 * STICK_FLAG_THRESHOLD)
		{
			buttonState[PadFlag::StickL_Down] = true;
		}

		// -1.0�`1,0�͈̔͂ɂ����߂�
		padValue[Value::StickL_X] = static_cast<float>(valX) / MAX_STICK_VALUE;
		padValue[Value::StickL_Y] = static_cast<float>(valY) / MAX_STICK_VALUE;
	}

}

bool GamePad::Input(const PadFlag& _getButton) const
{
	if (_getButton < 0 || _getButton > PadFlag::Button_Num)
	{
		HASHI_DEBUG_LOG("ButtonPress�F�w�肵���l���Ⴂ�܂�");
		return false;
	}

	return buttonState[_getButton];
}

bool GamePad::InputTrigger(const PadFlag& _getButton) const
{
	if (_getButton < 0 || _getButton >  PadFlag::Button_Num)
	{
		HASHI_DEBUG_LOG("ButtonDown�F�w�肵���l���Ⴂ�܂�");
		return false;
	}
	return (buttonState[_getButton] && !o_buttonState[_getButton]);
}

bool GamePad::InputRelease(const PadFlag& _getButton) const
{
	if (_getButton < 0 || _getButton >  PadFlag::Button_Num)
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
