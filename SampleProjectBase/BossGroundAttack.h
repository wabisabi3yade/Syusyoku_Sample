#pragma once
#include "BossGroundState.h"

/// @brief �n��ł̍U��
class BossGroundAttack : public BossGroundState
{

public:
	BossGroundAttack();
	virtual ~BossGroundAttack() {}

	/// @brief �J�n
	void OnStartBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;
};

