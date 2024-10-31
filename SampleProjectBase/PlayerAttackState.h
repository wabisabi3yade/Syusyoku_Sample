#pragma once
#include "PlayerActState_Base.h"
#include "AttackInformation.h"

/// @brief �v���C���[�U���̃Q�[��������
class PlayerAttackState : public PlayerActState_Base
{
private:
	/// @brief �R���r�l�[�V�����U���̐�
	StateType nextCombAtkState;

	/// @brief �U�����
	std::unique_ptr<HashiTaku::AttackInformation> pAttackInfo;
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

private:
	// State���ʏ���
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void OnTransitionCheck();

	/// @brief �U�������X�V����
	void UpdateAttackInfo();

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};