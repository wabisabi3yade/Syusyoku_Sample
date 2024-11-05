#pragma once
#include "PlayerActState_Base.h"
#include "AttackInformation.h"

/// @brief �v���C���[�U���̃Q�[��������
class PlayerAttackState : public PlayerActState_Base
{
private:
	/// @brief �R���r�l�[�V�����U���̐�
	PlayerState nextCombAtkState;

	/// @brief �U�����
	std::unique_ptr<HashiTaku::AttackInformation> pAttackInfo;

	/// @brief ��s���͎���
	float senkoInputTime;
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

protected:
	// State���ʏ���
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate();

	void ImGuiSetting() override;
private:
	/// @brief �U�������X�V����
	void UpdateAttackInfo();

	/// @brief �L�����Z���ł��邩�m�F����
	/// @return �L�����Z���ł��邩�H
	bool CheckCanCancel() const;
	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

public:
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// �U���g���K�[
};