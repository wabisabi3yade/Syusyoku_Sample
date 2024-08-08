#pragma once

#include "PlayerState_Base.h"

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController
{
public:
	enum class State
	{
		Move,
		Attack,
		MaxNum
	};

private:
	/// @brief �v���C���[�̍s����ԃ��X�g
	std::unordered_map<State, std::unique_ptr<PlayerState_Base>> stateList;

	/// @brief ���݂̍s���N���X
	PlayerState_Base* pCurrentState;

	/// @brief �v���C���[�̃I�u�W�F�N�g
	GameObject* pPlayerObject;

	// ���݂̏��
	State nowState;
public:
	PlayerActionController(GameObject& _pPlayerObject);
	~PlayerActionController() {}

	/// @brief ���݂̍X�V����
	void Update();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextState ���̏��
	void TransitionState(State _nextState);

	void ImGuiSetting();
};

