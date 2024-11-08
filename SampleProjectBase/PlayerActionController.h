#pragma once
#include "StateMachine.h"
#include "PlayerActState_Base.h"
#include "ChangeAnimObserver.h"

class PlayerChangeAnimObserver;
class CP_Player;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController : public HashiTaku::StateMachine_Base<PlayerActState_Base::PlayerState>, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
private:
	/// @brief �A�j���[�V�����ύX�����Ƃ��̃I�u�U�[�o�[
	std::unique_ptr<PlayerChangeAnimObserver> pChangeAnimObserver;

	/// @brief �A�j���[�V�����R���|�[�l���g	
	CP_Animation* pAnimation;

	/// @brief �v���C���[�R���|�[�l���g
	CP_Player* pPlayer;

	/// @brief ���̓N���X
	GameInput* pGameInput;

	/// @brief �L�����Z���t���O�̃|�C���^
	const bool* pIsCanCancel;

	/// @brief �^�[�Q�b�g�����ǂ���
	bool isTargeting;
public:
	/// @brief �R���X�g���N�^
	/// @param _player �v���C���[�R���|�[�l���g
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief �J�n����
	/// @param _animationController �A�j���[�V����
	void Begin(CP_Animation& _animation);

	/// @brief ���݂̍X�V����
	void Update();

	/// @brief �^�[�Q�b�g�̍X�V����
	void UpdateTargeting();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextActionName ���̏�Ԃ̖��O
	bool ChangeNode(const PlayerActState_Base::PlayerState& _nextActionState) override;

	/// @brief �^�[�Q�b�g�����擾����
	/// @return �^�[�Q�b�g�����H
	bool GetIsTargeting() const;

	/// @brief �L�����Z���ł��邩�擾
	/// @return �L�����Z���ł��邩�H
	bool GetIsCanCancel() const;

	/// @brief �v���C���[�R���|�[�l���g�擾
	/// @return �v���C���[�R���|�[�l���g
	CP_Player& GetPlayer();

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
	/// @brief �A�j���[�V�����R���g���[�����̃v���C���[��
	static constexpr  auto STATEANIM_PARAMNAME{ "state" };

	/// @brief �^�[�Q�b�g����\���A�j���[�V�����p�����[�^
	static constexpr auto TARGET_PARAMNAME{ "targeting" };

	/// @brief �L�����Z���ł��邩��\���A�j���[�V�����p�����[�^
	static constexpr auto CANCEL_PARAMNAME = "canCancel";
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *this);

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
