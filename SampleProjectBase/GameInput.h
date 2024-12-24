#pragma once
#include "Singleton_Base.h"

namespace HashiTaku
{
	// �Q�[�������̓N���X
	class GameInput
	{
	public:
		// bool�^�ň������
		enum class ButtonType
		{
			Player_Attack,	// �U��
			Player_ChargeAttack,	// �U��
			Player_Jump, // �W�����v
			Player_Rolling,	// ���[�����O
			Player_RockOn,	// ���b�N�I��
			Player_Guard,	// �K�[�h
			Max_BoolNum
		};
		/// @brief �{�^���̎�ސ�
		static constexpr u_int BUTTON_TYPE_CNT{ static_cast<u_int>(ButtonType::Max_BoolNum) };

		// ���l�ň������
		enum class ValueType
		{
			Player_Move,
			Camera_Move,
			Max_ValueNum
		};

	private:
		// �Ή��{�^��
		struct ButtonLink
		{
			GamePad::PadFlag padType;
			int keyType;
		};

		// �Ή��X�e�B�b�N
		struct ValueLink
		{
			GamePad::Value padValue_x;	// X
			GamePad::Value padValue_y;	// Y
			int keyType_x_positive;	// ��
			int keyType_x_negative;	// ��
			int keyType_y_positive;
			int keyType_y_negative;
		};

		/// @brief �Ή��������
		std::array<ButtonLink, BUTTON_TYPE_CNT> buttonLinks;
		std::array<ValueLink, BUTTON_TYPE_CNT> valueLinks;

		/// @brief �{�^�����̗͂�������(�g���K�[���͂̂ݑΉ�)
		std::array<float, BUTTON_TYPE_CNT> buttonTimeLine;

		// ���
		bool c_buttonState[BUTTON_TYPE_CNT];
		bool p_buttonState[BUTTON_TYPE_CNT];

		DirectX::SimpleMath::Vector2 c_valueState[static_cast<u_int>(ValueType::Max_ValueNum)];

	public:
		GameInput();
		~GameInput() {}

		/// @brief �X�V����
		void Update();

		/// @brief ������Ă��邩�擾
		/// @param _buttonType �Q�[�����{�^���̎��
		/// @return ������Ă��邩�H
		bool GetButton(ButtonType _buttonType);

		/// @brief �������u�Ԃ��擾
		/// @param _buttonType �Q�[�����{�^���̎��
		/// @return �������u�Ԃ��H
		bool GetButtonDown(ButtonType _buttonType);

		/// @brief �����ꂽ�u�Ԃ��擾
		/// @param _buttonType �Q�[�����{�^���̎��
		/// @return �����ꂽ�u�Ԃ��H
		bool GetButtonUp(ButtonType _buttonType);

		/// @brief �w��b�ȓ��Ƀ{�^���������ꂽ���擾
		/// @param _buttonType �Q�[�����{�^���̎��
		/// @param _withinTime �w��b�ȓ��ɉ�����Ă���
		/// @return �w��b�ȓ��Ƀ{�^���������ꂽ���H
		bool GetButtonDown(ButtonType _buttonType, float _withinTime);

		/// @brief ���̓X�e�B�b�N�̐��l���擾����
		/// @param _valueType �Q�[�����X�e�B�b�N�̎��
		/// @return ���͐��l
		const DirectX::SimpleMath::Vector2& GetValue(ValueType _valueType);

	private:
		/// @brief �f�t�H���g�̐ݒ�ɂ���
		void DefaultSetting();

		/// @brief �X�e�B�b�N���͏����ݒ�
		void InitValueSettings();

		// �Q�[���p�b�h���Ȃ����Ă��邩�擾
		bool IsGamePadConnect();

		/// @brief �{�^�����͍X�V
		void ButtonUpdate();

		/// @brief ���l���͍X�V
		void ValueUpdate();

		/// @brief �����̍X�V����
		void TimeLineUpdate();
	};
}