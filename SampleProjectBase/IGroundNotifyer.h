#pragma once

/// @brief �n�ʔ���ʒm���Ă��炤���C���^�[�t�F�[�X
class IGroundNotifyer
{

public:
	IGroundNotifyer() = default;
	~IGroundNotifyer() = default;

	/// @brief �n�ʂɕt�����u�Ԃ̏���
	virtual void OnGroundEnter() = 0;

	/// @brief �n�ʂ��痣�ꂽ�u�Ԃ̏���
	virtual void OnGroundExit() = 0;
};