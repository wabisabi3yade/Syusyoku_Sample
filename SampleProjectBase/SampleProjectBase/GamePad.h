#pragma once
#include <xinput.h>
#pragma comment (lib, "xinput.lib")

// �Q�[���p�b�h�N���X
class GamePad	
{

public:
	enum Button	// �{�^��
	{
		// �\��
		Arrow_Up = 0,
		Arrow_Down,
		Arrow_Right,
		Arrow_Left,

		// �L��
		Sankaku,
		Batsu,
		Maru,
		Shikaku,

		// RL
		R1,
		L1,
		R3,
		L3,

		// �I�v�V����
		Option,
		Share,

		Button_Num
	};

	enum Value	// �X�����Ƃ�
	{
		// �X�e�B�b�N
		StickR_X = 0,
		StickR_Y,
		StickL_X,
		StickL_Y,

		// �g���K�[
		Trigger_R2,
		Trigger_L2,

		Value_Num
	};

private:
	DWORD isResult;	// ���̏��

	bool buttonState[static_cast<int>(Button::Button_Num)];	// ���݂̃{�^�����
	bool o_buttonState[static_cast<int>(Button::Button_Num)];	// 1�O�̃{�^�����

	float padValue[static_cast<int>(Value::Value_Num)];
	float o_padValue[static_cast<int>(Value::Value_Num)];
public:
	GamePad();
	~GamePad();

	void InputUpdate();	// ���͂̍X�V

	bool ButtonPress(const Button& _getButton)const;	// ������Ă��邩(�p��)
	bool ButtonDown(const Button& _getButton)const;	// �������u��(�u��)
	bool ButtonUp(const Button& _getButton)const;	// ���ꂽ�u��(�u��)

	float GetValue(const Value& _getValue)const;

	void DebugInput();	// �f�o�b�O�m�F

	bool GetConnecting() { return (isResult == ERROR_SUCCESS); }	// �q�����Ă邩�m�F����
};

