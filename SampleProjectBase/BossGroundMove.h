#pragma once
#include "BossGroundState.h"

/// @brief �{�X�n��ړ�
class BossGroundMove : public BossGroundState
{
	/// @brief  �ő�ړ����x
	float maxSpeed;

	/// @brief �����x
	float acceleration;

	/// @brief ���ݑ��x
	float currentSpeed;

	/// @brief ��]���x
	float rotateSpeed;
	
	/// @brief �ߋ����U���Ɉڍs���鋗��
	float transNearDistance;

public:
	BossGroundMove();
	~BossGroundMove() {}

	void DebugDisplay() override;

	nlohmann::json Save();
	void Load(const nlohmann::json& _data);
private:
	/// @brief �X�V����
	void UpdateBehavior() override;

	/// @brief ��Ԑ؂�ւ��I������
	void OnEndBehavior() override;

	/// @brief �J�ڐ؂�ւ�
	void TransitionCheckUpdate() override;

	/// @brief ���g����]����
	void Rotation();

	/// @brief �ړ�����
	void Move();

	/// @brief �ߐڂ̋����ɂȂ��Ă��邩�m�F
	/// @return �J�ڂ��邩�H
	bool CheckNearTransition();

	void ImGuiDebug() override;
};

