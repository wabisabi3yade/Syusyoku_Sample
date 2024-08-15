#pragma once
#include "Singleton_Base.h"

// �Q�[�������̓N���X
class GameInput : public Singleton_Base<GameInput>
{
public:
	// bool�^�ň������
	enum class ButtonType
	{
		Player_Attack,
		Player_Jump,
		Max_BoolNum
	};

	// ���l�ň������
	enum class ValueType
	{
		Player_Move,
		Camera_Move,
		Max_ValueNum
	};

private:

	friend class Singleton_Base<GameInput>;

	// �Ή��{�^��
	struct ButtonLink
	{
		GamePad::Button padType;
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
	ButtonLink buttonLinks[static_cast<u_int>(ButtonType::Max_BoolNum)];
	ValueLink valueLinks[static_cast<u_int>(ValueType::Max_ValueNum)];

	// ���
	bool c_buttonState[static_cast<u_int>(ButtonType::Max_BoolNum)];
	bool o_buttonState[static_cast<u_int>(ButtonType::Max_BoolNum)];

	DirectX::SimpleMath::Vector2 c_valueState[static_cast<u_int>(ButtonType::Max_BoolNum)];


	GameInput();
	~GameInput() {}
public:
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

	/// @brief ���̓X�e�B�b�N�̐��l���擾����
	/// @param _valueType �Q�[�����X�e�B�b�N�̎��
	/// @return ���͐��l
	DirectX::SimpleMath::Vector2 GetValue(ValueType _valueType);

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
};

