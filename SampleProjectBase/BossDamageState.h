#pragma once
#include "BossActState_Base.h"

/// @brief �{�X�_���[�W���炢State
class BossDamageState : public BossActState_Base
{

public:
	BossDamageState();
	~BossDamageState() {}

private:
	/// @brief �J�n����
	void OnStartBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;
	
	/// @brief �_���[�W���v���C���[�Ɍ�����
	void LookPlayer();
};

