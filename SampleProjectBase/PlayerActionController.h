#pragma once
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"

class CP_Animation;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController : HashiTaku::IImGuiUser, HashiTaku::ISaveLoad
{
private:
	/// @brief �v���C���[�̍s����ԃ��X�g
	std::unordered_map<PlayerActState_Base::StateType, std::unique_ptr<PlayerActState_Base>> actionList;

	/// @brief �X�e�[�g����ύX�������ɒʒm���󂯎��I�u�U�[�o�[
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief ���݂̍s���N���X
	PlayerActState_Base* pCurrentState;

	/// @brief �A�j���[�V�����̃R���g���[���[
	CP_Animation* pAnimation;

	/// @brief �v���C���[�̃I�u�W�F�N�g
	GameObject* pPlayerObject;
public:
	PlayerActionController(GameObject& _pPlayerObject);
	~PlayerActionController() {}

	/// @brief �J�n����
	/// @param _animationController �A�j���[�V����
	void Begin(CP_Animation& _animation);

	/// @brief ���݂̍X�V����
	void Update();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextState ���̏��
	void ChangeState(PlayerActState_Base::StateType _nextState);

	/// @brief �����̏�Ԃ��Z�b�g
	/// @param _defaultState ������Ԃ̃X�e�[�g
	void DefaultState(PlayerActState_Base::StateType _defaultState);

	void ImGuiSetting() override;
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �V����State�𐶐�
	/// @tparam T �Ή����Ă���s���N���X
	template <class T> void CreateState();

private:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto STATE_PARAMNAME = "state";	// �A�j���[�V�����̏�ԕϐ�
};

template<class T>
inline void PlayerActionController::CreateState()
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(*pPlayerObject, *pStateChangeObserver);

	PlayerActState_Base::StateType state = createState->GetActStateType();
	actionList[state] = std::move(createState);
}
