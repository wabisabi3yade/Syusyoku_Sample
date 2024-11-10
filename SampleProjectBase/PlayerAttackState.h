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

	/// @brief �O�ɐi�ޑ��x�J�[�u
	AnimationCurve forwardSpeedCurve;

	/// @brief �i�ދ���
	float atkMoveSpeed;

	/// @brief ��s���͎���
	float senkoInputTime;

	/// @brief �O�֐i�ނ��H
	bool isMoveForward;
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

protected:
	// State���ʏ���
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	void ImGuiDebug() override;
private:
	/// @brief �U�������X�V����
	void UpdateAttackInfo();

	/// @brief �U�����ɑO�֐i��
	void ForwardProgressMove();
	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

public:
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// �U���g���K�[
};