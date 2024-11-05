#pragma once
#include "StateMachine.h"
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"
#include "ChangeAnimObserver.h"

class CP_Animation;
class PlayerChangeAnimObserver;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController : public HashiTaku::StateMachine_Base<PlayerActState_Base::PlayerState>, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
private:
	/// @brief �X�e�[�g����ύX�������ɒʒm���󂯎��I�u�U�[�o�[
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief �A�j���[�V�����ύX�����Ƃ��̃I�u�U�[�o�[
	std::unique_ptr<PlayerChangeAnimObserver> pChangeAnimObserver;

	/// @brief �A�j���[�V�����̃R���g���[���[
	CP_Animation* pAnimation;

	/// @brief �v���C���[�R���|�[�l���g
	CP_Player* pPlayer;

	/// @brief �v���C���[�̃I�u�W�F�N�g
	GameObject* pPlayerObject;
public:
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief �J�n����
	/// @param _animationController �A�j���[�V����
	void Begin(CP_Animation& _animation);

	/// @brief ���݂̍X�V����
	void Update();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextActionName ���̏�Ԃ̖��O
	bool ChangeNode(const PlayerActState_Base::PlayerState& _nextActionState) override;

	/// @brief ���݂̃A�N�V�������擾
	/// @return �A�N�V�����X�e�[�g
	PlayerActState_Base* GetCurrentAction();

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[���擾
	/// @return �A�j���[�V�����ύX�I�u�U�[�o�[
	PlayerChangeAnimObserver& GetChangeAnimObserver();
	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �V����State�𐶐�
	/// @tparam T �Ή����Ă���s���N���X
	/// @param _actionName �A�N�V������
	template <class T> void CreateState(PlayerActState_Base::PlayerState _actionState);

	/// @brief PlayerActState_Base�ɃL���X�g
	/// @param _stateNodeBase �X�e�[�g�m�[�h���ϐ�
	/// @return �L���X�g�����ϐ�
	PlayerActState_Base& CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase);

	void ImGuiSetting() override;
private:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto STATEANIM_PARAMNAME = "state";	// �A�j���[�V�����̏�ԕϐ�
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *pPlayer, *pStateChangeObserver);

	AddNode(_actionState, std::move(createState));
}

/// @brief �v���C���[�A�N�V�����ŃA�j���[�V�����J�ڂ����Ƃ��̃I�u�U�[�o�[
class PlayerChangeAnimObserver : public HashiTaku::ChangeAnimObserver
{
	PlayerActionController* pActionController;

public:
	PlayerChangeAnimObserver(PlayerActionController& _playerActCon);
	~PlayerChangeAnimObserver() {}

	/// @brief �ʒm�������Ƃ��̏���
	void ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value);
};
