#pragma once
#include "PlayerActState_Base.h"

class PlayerGroundActionController;

/// @brief �n��s���̊��N���X
class PlayerGroundState : public PlayerActState_Base
{
public:
	/// @brief  �v���C���[�̍s�����
	enum class PlayerState
	{
		// �ҋ@
		Idle,

		// �ړ�
		Move,
		TargetMove,
		BeginJump,
		Rolling,
		Damage_S,	// �_���[�W���̂�����
		Damage_L,	// �_���[�W��̂�����
		Guard,	// �K�[�h�J�n
		Guard_Parry,	// �p���B����
		Landing,	// ���n

		// �n��R���r�l�[�V�����U��
		Attack11 = 20,
		Attack12,
		Attack13,
		Attack14,

		// �n��K�E�U��
		SpecialAtkHi = 30,
		SpecialAtkLow,
		SpecialAtkGuard,	// �K�[�h�p���B���̍U��

		// �Ō�
		None = 99
	};
public:
	PlayerGroundState();
	virtual  ~PlayerGroundState() {}

	/// @brief �n��s���̏���������
	/// @param _stateType �X�e�[�g
	/// @param _actController �A�N�V�����R���g���[���[
	void Init(PlayerActionController_Base& _actController);

	/// @brief �J�n�����Ăяo��
	void OnStart() override;

	/// @brief �X�V�����Ăяo��
	void Update() override;

	/// @brief  �I�������Ăяo��
	void OnEnd() override;

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �e��Ԃ̊J�n����
	virtual void OnStartBehavior() {};

	/// @brief �X�V����
	virtual void UpdateBehavior() {};

	/// @brief �e��Ԃ̏I������
	virtual void OnEndBehavior() {};

	/// @brief �X�e�[�g�J�ڏ����̃`�F�b�N����
	virtual void TransitionCheckUpdate();

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	/// @param _isForce) �����I��
	void ChangeState(PlayerState _nextState, bool _isForce = false);

	/// @brief �n��s���R���g���[���[�ɕϊ�
	PlayerGroundActionController& CastGroundController();

private:
	/// @brief ���͍X�V
	void InputUpdate();
};

