#pragma once
#include "PlayerAirState.h"
#include "PlayerAttackInformation.h"

/// @brief �v���C���[�̋󒆍U��
class PlayerAirAttack : public PlayerAirState
{
	/// @brief �R���r�l�[�V�����U���̐�
	PlayerState nextCombAtkState;

	/// @brief �U����񃊃X�g(�P���Ȃ�ŏ��̏����g�p����)
	std::vector<HashiTaku::PlayerAttackInformation> attackInfos;

	/// @brief �S�̂̍U����񉽌��邩
	u_int attackTimeCnt;
public:
	PlayerAirAttack();
	~PlayerAirAttack() {}

private:
	/// @brief �J�n
	void OnStartBehavior() override;

	/// @brief �X�V����
	void UpdateBehavior() override;

	/// @brief �I��
	void OnEndBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName,
		const std::string& _toAnimNodeName) override;

	/// @brief �R���r�l�[�V�����U���̓��͍X�V
	void UpdateCombInput();

	/// @brief �G�̕���������
	void LookAtEnemy();

	/// @brief �U�������X�V����
	void UpdateAttackInfo();

	/// @brief Y���x��0�ɂ��ė����Ȃ��悤�ɂ���
	void ClearVelocityY();
};

