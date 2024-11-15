#pragma once
#include "CharacterActionController.h"
#include "PlayerActState_Base.h"
#include "ChangeAnimObserver.h"
#include "ITargetAccepter.h"

class PlayerChangeAnimObserver;
class CP_Player;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController : public CharacterActionController, public IObjectTargeter
{
private:
	/// @brief ���̓N���X
	GameInput* pInput;

	/// @brief �^�[�Q�b�g���Ă���I�u�W�F�N�g��
	ITargetAccepter* pTargetObject;

	/// @brief �L�����Z���t���O�̃|�C���^
	const bool* pIsCanCancel;

	/// @brief ��s���̓t���O�̃|�C���^
	const bool* pIsSenkoInput;

	/// @brief �^�[�Q�b�g�����ǂ���
	bool isTargeting;
public:
	/// @brief �R���X�g���N�^
	/// @param _player �v���C���[�R���|�[�l���g
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief �J�n����
	/// @param _animation �A�j���[�V����
	void Init(CP_Animation* _animation);

	/// @brief �^�[�Q�b�g�̍X�V����
	void UpdateTargeting();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextActionName ���̏�Ԃ̖��O
	bool ChangeState(const PlayerActState_Base::PlayerState& _nextActionState);

	/// @brief �^�[�Q�b�g�����擾����
	/// @return �^�[�Q�b�g�����H
	bool GetIsTargeting() const;

	/// @brief �L�����Z���ł��邩�擾
	/// @return �L�����Z���ł��邩�H
	bool GetIsCanCancel() const;

	/// @brief ��s���͂ł��邩���擾
	/// @return ��s���͂ł��邩�H
	bool GetCanInput() const;

	/// @brief �v���C���[�R���|�[�l���g�擾
	/// @return �v���C���[�R���|�[�l���g
	CP_Player& GetPlayer();

	/// @brief ���݂̃A�N�V�������擾
	/// @return �A�N�V�����X�e�[�g
	PlayerActState_Base* GetCurrentAction();

	/// @brief �eState�̕�������擾����
	/// @param _stateId ��Ԃ�ID
	/// @return ������
	std::string GetStateStr(int _stateId) override;

	/// @brief �eState��ID���擾����
	/// @param _stateName�@��Ԗ�
	/// @return ��Ԃ�ID
	int GetStateId(const std::string& _stateName);

	/// @brief �^�[�Q�b�g�I�u�W�F�N�g���擾����
	/// @param _targetObject �^�[�Q�b�g�I�u�W�F�N�g
	void GetTargetObject(ITargetAccepter& _targetObject);

	/// @brief �^�[�Q�b�g�����񂾂Ƃ��̏���
	void OnTargetDeath();
private:
	/// @brief ���݂̍X�V����
	void Update() override;

	/// @brief �V����State�𐶐�
	/// @tparam T �Ή����Ă���s���N���X
	/// @param _actionName �A�N�V������
	template <class T> void CreateState(PlayerActState_Base::PlayerState _actionState);

	/// @brief PlayerActState_Base�ɃL���X�g
	/// @param _stateNodeBase �X�e�[�g�m�[�h���ϐ�
	/// @return �L���X�g�����ϐ�
	PlayerActState_Base& CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase);

	void ImGuiDebug() override;
private:
	/// @brief �A�j���[�V�����R���g���[�����̃v���C���[��
	static constexpr  auto STATEANIM_PARAMNAME{ "state" };

	/// @brief �^�[�Q�b�g����\���A�j���[�V�����p�����[�^
	static constexpr auto TARGET_PARAMNAME{ "targeting" };

	/// @brief �L�����Z���ł��邩��\���A�j���[�V�����p�����[�^
	static constexpr auto CANCEL_PARAMNAME = "canCancel";

	/// @brief ��s���͂ł��邩��\���A�j���[�V�����p�����[�^
	static constexpr auto SENKOINPUT_PARAMNAME = "canInput";
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *this);

	AddNode(static_cast<int>(_actionState), std::move(createState));
}
