#pragma once
#include "BossDamageState.h"

/// @brief �{�X�u���C�N�I���̃m�b�N
class BossBreakEndKnock : public BossDamageState
{
	/// @brief �u���C�N�I���ł���u���C�N�l
	float canBreakValue;

	/// @brief �m�b�N�̋���
	float knockDistance;
public:
	BossBreakEndKnock();
	~BossBreakEndKnock() {}

	/// @brief �u���C�N�l���擾����
	float GetCanBreakValue() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �J�n
	void OnStartBehavior() override;

	/// @brief �m�b�N����^�[�Q�b�g�̍��W�����߂�
	void CalcKnockTargetPos();

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName,
		const std::string& _toAnimNodeName) override;

	void ImGuiDebug() override;
};

