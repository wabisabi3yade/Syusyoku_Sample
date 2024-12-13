#pragma once
#include "PlayerGroundState.h"
#include "PlayerAttackInformation.h"
#include "AnimationCurve.h"

/// @brief �v���C���[�U���̃Q�[��������
class PlayerAttackState : public PlayerGroundState
{
private:
	/// @brief �R���r�l�[�V�����U���̐�
	PlayerState nextCombAtkState;

	/// @brief �U����񃊃X�g(�P���Ȃ�ŏ��̏����g�p����)
	std::vector<HashiTaku::PlayerAttackInformation> attackInfos;

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

	/// @brief �U����񉽌ڂ�
	u_int curAttackTime;

	/// @brief �S�̂̍U����񉽌��邩
	u_int attackTimeCnt;

	/// @brief �O�֐i�ނ��H
	bool isMoveForward;
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

	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

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

	void ImGuiCombAttack();
private:
#ifdef EDIT
	// �R���r�l�[�V�����U�����ł���X�e�[�g
	inline static std::vector<std::string> combAtkState
	{
		"Attack11",
		"Attack12",
		"Attack13",
		"Attack14"
	};
#endif // EDIT
};