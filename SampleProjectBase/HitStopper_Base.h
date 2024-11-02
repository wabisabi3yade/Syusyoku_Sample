#pragma once

/// @brief �q�b�g�X�g�b�v�Ŏ~�܂鑤�̊��N���X
class HitStopper_Base
{

public:
	HitStopper_Base() {}
	virtual ~HitStopper_Base();

	/// @brief �q�b�g�X�g�b�v�}�l�[�W���[�Ɏ��g��ǉ�����
	void AddToHitStopManager();

	/// @brief �q�b�g�X�g�b�v�J�n�������̏���
	virtual void OnHitStopBegin() = 0;

	/// @brief �q�b�g�X�g�b�v�I���������̏���
	virtual void OnHitStopEnd() = 0;

private:
	/// @brief �q�b�g�X�g�b�v�}�l�[�W���[���玩�g���폜����
	void RemoveFromHitStopManager();
};

