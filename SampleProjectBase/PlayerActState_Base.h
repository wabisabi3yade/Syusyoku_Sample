#pragma once

// �Q�[��������
#include "GameInput.h"

class PlayerActionController;
class PlayerAnimController;
class GameObject;

/// @brief �v���C���[�̍s��State���N���X
class PlayerActState_Base
{
protected:
	// �A�N�V�����Ǘ�
	PlayerActionController* pActionController;

	// �A�j���[�V�����Ǘ�
	PlayerAnimController* pAnimController;

	/// @brief  �v���C���[�I�u�W�F�N�g
	GameObject* pPlayerObject;

protected:
	/// @brief �X�V����
	virtual void Update() = 0;

	/// @brief �J�ڏ������m�F����
	virtual void TransitionCheck() = 0;

	/// @brief �{�^���������ꂽ�玟�̏�ԂɑJ�ڂ�����
	/// @param _buttonType �{�^���̎��
	/// @param _nextState ���̏��
	void ButtonChangeState(GameInput::ButtonType _buttonType, 
		u_int _nextState);

	/// @brief �A�j���[�V������ύX
	/// @param _animState �A�j���[�V�������̃X�e�[�g
	void ChangeAnimation(u_int _animState);
public:
	PlayerActState_Base(PlayerActionController& _pController);
	~PlayerActState_Base() {}

	/// @brief ��Ԃ��ς�����Ƃ��̏�������
	virtual void Init() = 0;

	/// @brief �O������Ăяo���p�X�V����
	void UpdateBase();

	/// @brief  �I������
	virtual void Terminal() = 0;

	/// @brief ImGui����
	virtual void ImGuiSetting() {}

	// �v���C���[�I�u�W�F�N�g���Z�b�g
	void SetPlayerObject(GameObject& _playerObj);

	// �A�j���[�V�����Ǘ����Z�b�g����
	void SetAnimController(PlayerAnimController& _pController);
};

