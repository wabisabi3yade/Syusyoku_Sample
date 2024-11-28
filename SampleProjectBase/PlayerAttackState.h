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

	/// @brief �J�ڂ����̍U��
	PlayerState curChangeAtkState;

	/// @brief �U����񃊃X�g(�P���Ȃ�ŏ��̏����g�p����)
	std::vector<HashiTaku::AttackInformation> attackInfos;

	/// @brief �O�ɐi�ދ����J�[�u
	AnimationCurve progressDistanceCurve;

	/// @brief ���A�^�b�N�ϐ�
	const bool* pIsReAttack;

	/// @brief ����̍U���Ői�ދ���
	float curAtkProgressDis;

	/// @brief �i�ލő勗��
	float atkMaxDistance;

	/// @brief �O�t���[���܂łɐi�񂾋���
	float prevProgressDistance;

	/// @brief �ŏ��Ɍ����]���x
	float lookRotateSpeed;

	/// @brief �U����񉽌ڂ�
	u_int curAttackTime;

	/// @brief �S�̂̍U����񉽌��邩
	u_int attackTimeCnt;

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
	void TransitionCheckUpdate() override;

	void ImGuiDebug() override;
private:
	/// @brief �p�����[�^��������
	void InitParameter();

	/// @brief �U�������X�V����
	void UpdateAttackInfo();

	/// @brief ���A�^�b�N���邩�m�F����
	void UpdateReAttack();

	/// @brief �R���r�l�[�V�����U���̓��͍X�V
	void UpdateCombInput();

	/// @brief �U���Ői�ދ��������߂�
	/// @param _atkEnemyPos �G�̍��W
	void CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos);

	/// @brief �G�ɑ΂��Č�����
	/// @param _atkEnemyPos �G�̍��W
	void LookAtEnemyInstant(DirectX::SimpleMath::Vector3 _atkEnemyPos);

	/// @brief �U�����ɑO�֐i��
	void ForwardProgressMove();

	/// @brief �U���ł��邩�擾����
	/// @return �U���ł��邩�H
	bool GetCanCombAttack();

	/// @brief �U������G�̍��W���擾����
	/// @return �G�̍��W
	DirectX::SimpleMath::Vector3 GetAtkEnemyPos();
public:
	/// @brief �U���g���K�[
	constexpr static auto ATTACKTRIGGER_PARAMNAME{ "attackTrigger" };

	/// @brief ���A�^�b�N����Ƃ��ɌĂяo���ϐ�
	static constexpr auto REATTACK_PARAMNAME{ "isReAttack" };
};