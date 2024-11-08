#pragma once
#include "BossGroundState.h"

/// @brief �{�X�n��ړ�
class BossGroundMove : public BossGroundState
{
	/// @brief  �ő�ړ����x
	float maxSpeed;

	/// @brief �����x
	float acceralation;

	/// @brief ���ݑ��x
	float currentSpeed;
public:
	BossGroundMove();
	~BossGroundMove() {}

private:
	/// @brief �X�V����
	void UpdateBehavior() override;

	/// @brief �ړ�����
	void Move();
};

