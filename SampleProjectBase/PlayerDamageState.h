#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

/// @brief �_���[�W����
class PlayerDamageState : public PlayerActState_Base
{
	/// @brief �ړ����x�̃A�j���[�V���J�[�u
	AnimationCurve knockSpeedCurve;

	/// @brief ������ԕ���(�O������Z�b�g����)
	DirectX::SimpleMath::Vector3 knockVector;

	/// @brief �_���[�W���Ɉړ����鋗��
	float maxKnockMoveSpeed;

	/// @brief �O��̃A�j���[�V��������
	float prevAnimRatio;

	/// @brief �m�b�N�ňړ����Ă��邩�H
	bool isKnockMoving;

	/// @brief ���G�ɂ��邩�H
	bool isInvicible;
public:
	PlayerDamageState();
	~PlayerDamageState() {}

	/// @brief ������ԃx�N�g��
	void SetKnockVec(const DirectX::SimpleMath::Vector3& _knockVec);

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �e��Ԃ̊J�n����
	void OnStartBehavior() override;

	/// @brief �X�V����
	void UpdateBehavior() override;

	/// @brief �I������
	void OnEndBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	/// @brief �m�b�N���̈ړ�
	void KnockMove();

	void LookEnemy();

	void ImGuiDebug() override;
};

