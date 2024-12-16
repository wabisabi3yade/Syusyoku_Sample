#pragma once
#include <xinput.h>
#pragma comment (lib, "xinput.lib")

// �Q�[���p�b�h�N���X
class GamePad	
{
public:
	enum PadFlag	// �p�b�h�̓��̓t���O
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
		R2,
		L2,
		R3,
		L3,

		// �X�e�B�b�N
		StickR_Up,
		StickR_Down,
		StickR_Right,
		StickR_Left,

		StickL_Up,
		StickL_Down,
		StickL_Right,
		StickL_Left,


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

	bool buttonState[static_cast<int>(PadFlag::Button_Num)];	// ���݂̃{�^�����
	bool o_buttonState[static_cast<int>(PadFlag::Button_Num)];	// 1�O�̃{�^�����

	float padValue[static_cast<int>(Value::Value_Num)];

	/// @brief �U������
	float vibeTime;
public:
	GamePad();
	~GamePad();

	void InputUpdate();	// ���͂̍X�V

	bool Input(const PadFlag& _getButton)const;	// ������Ă��邩(�p��)
	bool InputTrigger(const PadFlag& _getButton)const;	// �������u��(�u��)
	bool InputRelease(const PadFlag& _getButton)const;	// ���ꂽ�u��(�u��)]


	void SetVibration(float _power, float _time);

	/// @brief �X�e�B�b�N��LR�g���K�[�̓��͗ʂ��擾����
	/// @param _getValue �擾����ꏊ
	/// @return ���͗�
	float GetValue(const Value& _getValue) const;

	bool GetConnecting() const { return (isResult == ERROR_SUCCESS); }	// �q�����Ă邩�m�F����
};

