#pragma once
#include "EnemyActionController.h"
#include "BossActState_Base.h"

/// @brief �{�X�̃A�N�V�����R���g���[���[
class BossActionController : public EnemyActionController
{


public:
	BossActionController(CP_Character& _boss);
	~BossActionController() {}

	/// @brief ����������
	/// @param _animationController 
	void Init(CP_Animation& _animationController) override;

	/// @brief �X�V����
	void Update() override;

	/// @brief �X�e�[�g��ύX
	/// @param _nextState ���̃X�e�[�g
	/// @return �����������H
	bool ChangeState(BossActState_Base::BossState _nextState);
};

