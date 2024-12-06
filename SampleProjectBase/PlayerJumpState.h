#pragma once
#include"PlayerActState_Base.h"

/// @brief �v���C���[�W�����v�X�e�[�g
class PlayerJumpState : public PlayerActState_Base
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

