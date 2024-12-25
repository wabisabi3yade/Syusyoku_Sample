#pragma once
#include <xinput.h>
#pragma comment (lib, "xinput.lib")


namespace HashiTaku
{
	/// @brief �Q�[���p�b�h�N���X
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
		/// @brief �X�e�B�b�N�Ȃǂ̓��͒l
		float padValue[static_cast<int>(Value::Value_Num)];

		/// @brief �U������
		float vibeTime;

		/// @brief �U���̌o�ߎ���
		float elapsedViveTime;

		/// @brief �R���g���[���[���Ȃ����Ă��邩���
		DWORD isConnectResult;

		/// @brief ���݂̃{�^�����
		bool buttonState[static_cast<int>(PadFlag::Button_Num)];

		/// @brief 1�t���[���O�̃{�^�����
		bool o_buttonState[static_cast<int>(PadFlag::Button_Num)];

		/// @brief �R���g���[���[�U�������H
		bool isVibration;
	public:
		GamePad();
		~GamePad();

		void InputUpdate();	// ���͂̍X�V

		/// @brief �R���g���[���[�U���̍X�V����
		void VibrationUpdate();

		bool Input(const PadFlag& _getButton)const;	// ������Ă��邩(�p��)
		bool InputTrigger(const PadFlag& _getButton)const;	// �������u��(�u��)
		bool InputRelease(const PadFlag& _getButton)const;	// ���ꂽ�u��(�u��)]

		/// @brief �R���g���[���[�U��
		/// @param _power �k����p���[(0.0�`1.0)
		/// @param _time �U������
		void SetVibration(float _power, float _time);

		/// @brief �R���g���[���[�U�����I������
		void EndVibration();

		/// @brief �X�e�B�b�N��LR�g���K�[�̓��͗ʂ��擾����
		/// @param _getValue �擾����ꏊ
		/// @return ���͗�
		float GetValue(const Value& _getValue) const;

		bool GetConnecting() const { return (isConnectResult == ERROR_SUCCESS); }	// �q�����Ă邩�m�F����
	};


}
