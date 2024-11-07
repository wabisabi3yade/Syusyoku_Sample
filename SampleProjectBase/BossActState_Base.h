#pragma once
#include "EnemyActState_Base.h"

class BossActionController;

/// @brief �{�X�̃X�e�[�g�m�[�h���
class BossActState_Base : public EnemyActState_Base
{
public:
	// �{�X�̃X�e�[�g�^�C�v
	enum class BossState
	{
		// �ړ�
		Idle,
		Walk,
		Move,

		// �U��
		Attack1,

		// �Ō�
		None
	};

private:
	/// @brief �{�X�̃X�e�[�g
	BossState stateType;

	/// @brief �A�N�V�����R���g���[���[
	BossActionController* pActionController;

public:
	BossActState_Base();
	virtual ~BossActState_Base() {}

	/// @brief ����������
	/// @param _stateType�@���
	/// @param _actController�@�A�N�V�����R���g���[���[
	void Init(BossState _stateType, BossActionController& _actController);

	/// @brief ��Ԑ؂�ւ��J�n����
	void OnStart() override;

	/// @brief �X�V����
	void Update() override;

	/// @brief ��Ԑ؂�ւ��I������
	void OnEnd() override;
};

