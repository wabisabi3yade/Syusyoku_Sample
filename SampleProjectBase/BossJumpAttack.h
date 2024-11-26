#pragma once
#include "BossAttackState.h"

/// @brief �{�X�̃W�����v�U��
class BossJumpAttack : public BossAttackState
{
	/// @brief �W�����v�c�̋����J�[�u
	AnimationCurve jumpHeightDisCurve;

	/// @brief  �W�����v�����x�W���̋����J�[�u
	AnimationCurve horiSpeedCofficientCurve;

	/// @brief �{�X����I�t�Z�b�g���ꂽ�Ƃ���ɍ��킹�ăv���C���[�֗�����i����ɍ��킹����j
	// �{�X�̐��ʕ������猩��
	float fallPosOffset;

	/// @brief ���݂̈ړ����x
	DirectX::SimpleMath::Vector3 curMoveSpeed;

	/// @brief ���ړ��ōő�̑��x
	float maxHoriSpeed;

	/// @brief �W�����v�̍���
	float maxJumpHeight;

	/// @brief �W�����v���x�W��
	float horiSpeedCofficient;

	/// @brief 1�t���[���O�܂ł̍���
	float prevJumpHeight;

	// �W�����v���̉�]���x
	float rotSpeedOnJump;
public:
	BossJumpAttack();
	~BossJumpAttack() {}

	/// @brief �f�o�b�O�`��
	void DebugDisplay();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void OnStartBehavior();
	void UpdateBehavior();

	/// @brief �W�����v�̓���
	void JumpMove();

	/// @brief ���̓���
	void HorizonMove();

	/// @brief �c�̓���
	void VertivalMove();

	void ImGuiDebug() override;
};

