#pragma once
#include "PlayerActState_Base.h"

class PlayerAirActionController;

/// @brief �v���C���[�󒆍s���̃x�[�X
class PlayerAirState : public PlayerActState_Base
{
public:
	/// @brief  �v���C���[�̍s�����
	enum class PlayerState
	{
		// ����
		None = 99,

		// �ҋ@
		Idle = 100,

		// �ړ�
		Move,
		Damage,	// �_���[�W��̂�����
		Guard,	// �K�[�h�J�n

		// �R���r�l�[�V�����U��
		Attack11 = 120,
		Attack12,
		Attack13,
		Attack14,

		// �K�E�U��
		HelmBreaker = 140,	// ������
		SpecialAtkGuard,	// �K�[�h�p���B���̍U��
	};

protected:
	/// @brief �e��Ԃ̊J�n����
	virtual void OnStartBehavior() {};

	/// @brief �X�V����
	virtual void UpdateBehavior() {};

	/// @brief �e��Ԃ̏I������
	virtual void OnEndBehavior() {};

	/// @brief �X�e�[�g�J�ڏ����̃`�F�b�N����
	virtual void TransitionCheckUpdate();

	/// @brief �󒆍s���̃R���g���[���[�ɃL���X�g
	/// @return �󒆍s���R���g���[���[
	PlayerAirActionController& CastAirController();

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	/// @param _isForce) �����I��
	void ChangeState(PlayerState _nextState, bool _isForce = false);

	void ImGuiDebug() override;
};

