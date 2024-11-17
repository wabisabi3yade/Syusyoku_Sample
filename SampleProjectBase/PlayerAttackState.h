#pragma once
#include "PlayerActState_Base.h"
#include "AttackInformation.h"
#include "AnimationCurve.h"

/// @brief �v���C���[�U���̃Q�[��������
class PlayerAttackState : public PlayerActState_Base
{
private:
	/// @brief �R���r�l�[�V�����U���̐�
	PlayerState nextCombAtkState;

	/// @brief �U�����
	std::unique_ptr<HashiTaku::AttackInformation> pAttackInfo;

	/// @brief �O�ɐi�ދ����J�[�u
	AnimationCurve progressDistanceCurve;

	/// @brief �i�ދ���
	float atkMoveSpeed;

	/// @brief �ŏ��Ɍ����]���x
	float lookRotateSpeed;

	/// @brief �O�֐i�ނ��H
	bool isMoveForward;

	/// @brief �U������o��O���H
	bool isAttackCollisionBefore; 
	
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
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

	/// @brief �G�ɑ΂��Č�����
	void LookAtEnemyInstant();

	/// @brief �U�����ɑO�֐i��
	void ForwardProgressMove();
public:
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// �U���g���K�[
};