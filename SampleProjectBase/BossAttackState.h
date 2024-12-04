#pragma once
#include "BossActState_Base.h"
#include "AttackInformation.h"

/// @brief �{�X��s�U���X�e�[�g
class BossAttackState : public BossActState_Base
{
	/// @brief ��]���x�J�[�u
	AnimationCurve rotSpeedCurve;

	/// @brief ��]���x�{��
	float rotSpeedTimes;

	/// @brief �J�[�u�ŉ�]�ړ������邩�H
	bool isUseRotateCurve;

protected :
	/// @brief �U����񃊃X�g(�P���Ȃ�ŏ��̏����g�p����)
	std::vector<HashiTaku::AttackInformation> attackInfos;

public:
	BossAttackState();
	virtual ~BossAttackState() {}

	/// @brief �J�n
	void OnStartBehavior() override;

	/// @brief �X�V����
	void UpdateBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	void RotateUpdate();

	void ImGuiDebug() override;
protected:
	/// @brief ���A�^�b�N����Ƃ��ɌĂяo���ϐ�
	static constexpr auto REATTACK_PARAMNAME{ "isReAttack" };
};

