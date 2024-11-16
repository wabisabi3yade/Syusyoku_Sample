#pragma once
#include "PlayerGroundAttack.h"

/// @brief �O�ːi�U��
class PlayerRushAttack : public PlayerGroundAttack
{
	AnimationCurve speedVerCurve;
	AnimationCurve speedHoriCurve;

	/// @brief �ړ����x
	float moveSpeed;

	/// @brief �ړ�����
	float moveTime;

	/// @brief �ړ��o�ߎ���
	float elapsedMoveTime;
public:
	PlayerRushAttack();
	~PlayerRushAttack() {}

private:
	// State���ʏ���
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void TransitionCheckUpdate();

	void ImGuiDebug() override;
};

