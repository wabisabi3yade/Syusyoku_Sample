#pragma once
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"

class AnimationController;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController
{
private:
	/// @brief �v���C���[�̍s����ԃ��X�g
	std::unordered_map<PlayerActState_Base::StateType, std::unique_ptr<PlayerActState_Base>> pActions;

	/// @brief �X�e�[�g����ύX�������ɒʒm���󂯎��I�u�U�[�o�[
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief ���݂̍s���N���X
	PlayerActState_Base* pCurrentState;

	/// @brief �A�j���[�V�����̃R���g���[���[
	AnimationController* pAnimController;

	/// @brief �v���C���[�̃I�u�W�F�N�g
	GameObject* pPlayerObject;
public:
	PlayerActionController(GameObject& _pPlayerObject);
	~PlayerActionController() {}

	/// @brief �J�n����
	/// @param _animationController �A�j���[�V�����R���g���[���[
	void Begin(AnimationController& _animationController);

	/// @brief ���݂̍X�V����
	void Update();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextState ���̏��
	void ChangeState(PlayerActState_Base::StateType _nextState);

	/// @brief �����̏�Ԃ��Z�b�g
	/// @param _defaultState ������Ԃ̃X�e�[�g
	void DefaultState(PlayerActState_Base::StateType _defaultState);

	void ImGuiSetting();

private:
	/// @brief �V����State�𐶐�
	/// @tparam T �Ή����Ă���s���N���X
	/// @param _state ����������
	template <class T> void CreateState(PlayerActState_Base::StateType _state);
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::StateType _state)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(*pPlayerObject, *pStateChangeObserver);

	pActions[_state] = std::move(createState);
}
