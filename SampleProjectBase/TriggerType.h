#pragma once

/// @brief �C�x���g���N���������ǂ�����Trigger�^�ϐ�
class TriggerType
{
	/// @brief �C�x���g���N��������true�ɂȂ�
	bool isTrigger;

public:
	TriggerType();
	~TriggerType() {}

	/// @brief �g���K�[��true�ɂ���
	void SetIsTrigger();

	/// @brief isTrigger�����Z�b�g����
	void ResetTrigger();

	/// @brief �g���K�[�̏�Ԃ��擾
	/// @return �g���K�[�̏��
	bool GetIsTrigger() const;
};

