#pragma once
#include "PlayerActionController_Base.h"
#include "ITargetAccepter.h"

/// @brief �v���C���[�A�N�V�����N���X
class PlayerAction
{
	/// @brief ���̓N���X
	GameInput* pInput;

	/// @brief �o�g���}�l�W���[
	CP_BattleManager* pBattleManager;

	/// @brief �^�[�Q�b�g����J�����ɓ`����
	IObjectTargeter* pTargetCamera;

	/// @brief �J����
	CP_Camera* pCamera;

	/// @brief �L�����Z���t���O�̃|�C���^
	const bool* pIsCanCancel;

	/// @brief ���͉\�t���O�̃|�C���^
	const bool* pIsCanInput;

	/// @brief �U���\�̃|�C���^
	const bool* pIsCanAttack;

	/// @brief �ړ��\�̃|�C���^
	const bool* pIsCanMove;

	/// @brief �^�[�Q�b�g�����ǂ���
	bool isTargeting;

	/// @brief 1�t���[���O�̃^�[�Q�b�g���
	bool prevIsTargeting;
public:
	PlayerAction();
	~PlayerAction() {}
};

