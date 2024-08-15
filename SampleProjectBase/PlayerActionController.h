#pragma once

#include "PlayerActState_Base.h"

class PlayerAnimController;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController
{
public:
	// �v���C���[�̍s�����
	enum class State
	{
		Move,
		Jump,
		Attack,
		MaxNum
	};

private:
	/// @brief �v���C���[�̍s����ԃ��X�g
	std::unordered_map<State, std::unique_ptr<PlayerActState_Base>> pActions;

	/// @brief ���݂̍s���N���X
	PlayerActState_Base* pCurrentState;

	/// @brief �A�j���[�V�����̃R���g���[���[
	PlayerAnimController* pAnimController;

	/// @brief �v���C���[�̃I�u�W�F�N�g
	GameObject* pPlayerObject;

	// ���݂̏��
	State nowState;
public:
	PlayerActionController(GameObject& _pPlayerObject, PlayerAnimController& _animController);
	~PlayerActionController() {}

	/// @brief ���݂̍X�V����
	void Update();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextState ���̏��
	void TransitionState(State _nextState);

	///// @brief �A�j���[�V������J�ڂ�����
	///// @param _nextAnimation ���̃A�j���[�V�����̖��O
	//void TransitionAnimation(const std::string& _nextName);

	void ImGuiSetting();

private:
	/// @brief �V����State�𐶐�
	/// @tparam T �Ή����Ă���s���N���X
	/// @param _state ����������
	template <class T> void CreateState(State _state);
};

template<class T>
inline void PlayerActionController::CreateState(State _state)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>(*this);

	createState->SetPlayerObject(*pPlayerObject);
	createState->SetAnimController(*pAnimController);

	pActions[_state] = std::move(createState);
}
