#pragma once
#include "PlayerActState_Base.h"
class PlayerIdleState : public PlayerActState_Base
{
	/// @brief ���u�o�ߎ���
	float leaveElapsedTime;

	/// @brief ����ҋ@���[�V�����Ɉڍs����܂ł̎���
	float specialIdleTransTiime;
public:
	PlayerIdleState();
	~PlayerIdleState() {}

private:
	/// @brief �e��Ԃ̊J�n����
	void OnStart() override;
	/// @brief �X�V����
	void Update() override;
	/// @brief �e��Ԃ̏I������
	void OnEnd() override;

	/// @brief ���u�o�ߎ��Ԃ�i�߂�
	void ProgressLeaveElapse();

	/// @brief �ړ�State�ɑJ�ڂł��邩�m�F
	/// @return �J�ڂł��邩�H
	bool IsCanMoveTransition();
};

