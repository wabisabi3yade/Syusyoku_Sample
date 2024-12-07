#pragma once
#include"PlayerGroundState.h"

/// @brief �v���C���[�W�����v�X�e�[�g
class PlayerJumpState : public PlayerGroundState
{
	/// @brief �W�����v�̗�
	float jumpPower;

	/// @brief �W�����v�J�n�܂ł̂����ގ���
	float crouchTime;

	/// @brief �X�e�[�g�J�n����̌o�ߎ���
	float stateElapsedTime;

	/// @brief  �W�����v�������ǂ����H
	bool isAlreadyJump;
public:
	PlayerJumpState();
	~PlayerJumpState();
	
private:
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;

	/// @brief �W�����v�J�n����Ƃ��̏���
	void OnBeginJump();
};

