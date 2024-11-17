#pragma once
#include "CharacterActionController.h"
#include "PlayerActState_Base.h"
#include "ChangeAnimObserver.h"
#include "ITargetAccepter.h"

class PlayerChangeAnimObserver;
class CP_Player;
class CP_BattleManager;

/// @brief �v���C���[�̓����R���g���[���[
class PlayerActionController : public CharacterActionController, public IObjectTargeter
{
private:
	/// @brief ���̓N���X
	GameInput* pInput;

	/// @brief �o�g���}�l�W���[
	CP_BattleManager* pBattleManager;

	/// @brief �^�[�Q�b�g���Ă���I�u�W�F�N�g��
	ITargetAccepter* pTargetObject;

	/// @brief �L�����Z���t���O�̃|�C���^
	const bool* pIsCanCancel;

	/// @brief ���͉\�t���O�̃|�C���^
	const bool* pIsCanInput;

	/// @brief �R���r�l�[�V�����U���\�̃|�C���^
	const bool* pIsCanCombAtk;

	/// @brief �^�[�Q�b�g�����ǂ���
	bool isTargeting;

	/// @brief 1�t���[���O�̃^�[�Q�b�g���
	bool prevIsTargeting;
public:
	/// @brief �R���X�g���N�^
	/// @param _player �v���C���[�R���|�[�l���g
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief �J�n����
	/// @param _animation �A�j���[�V����
	/// @param _pRigidBody ���W�b�h�{�f�B
	void Init(CP_Animation* _animation, CP_RigidBody* _pRigidBody);

	/// @brief �^�[�Q�b�g�̍X�V����
	void UpdateTargeting();

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextActionName ���̏�Ԃ̖��O
	bool ChangeState(const PlayerActState_Base::PlayerState& _nextActionState);

	/// @brief �^�[�Q�b�g�����擾����
	/// @return �^�[�Q�b�g�����H
	bool GetIsTargeting() const;

	/// @brief 1�t���[���O�̃^�[�Q�b�g�����擾����
	/// @return �^�[�Q�b�g�����H
	bool GetIsPrevTargeting() const;

	/// @brief �L�����Z���ł��邩�擾
	/// @return �L�����Z���ł��邩�H
	bool GetIsCanCancel() const;

	/// @brief ��s���͂ł��邩���擾
	/// @return ��s���͂ł��邩�H
	bool GetCanInput() const;

	/// @brief �R���r�l�[�V�����U���ł��邩���擾
	/// @return �R���r�l�[�V�����U���ł��邩�H
	bool GetCanCombAtk() const;

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

	/// @brief �^�[�Q�b�g�ɂ��Ă���I�u�W�F�N�g���擾
	/// @return �^�[�Q�b�g�ɂ��Ă���I�u�W�F�N�g
	ITargetAccepter* GetTargetObject();

	/// @brief �^�[�Q�b�g�I�u�W�F�N�g���擾����
	/// @param _targetObject �^�[�Q�b�g�I�u�W�F�N�g
	void SetTargetObject(ITargetAccepter& _targetObject) override;

	/// @brief �^�[�Q�b�g�I�u�W�F�N�g�����񂾂Ƃ��̍X�V����
	/// @param _deathTargetObj ���񂾃^�[�Q�b�g�I�u�W�F�N�g
	void UpdateDeathNotify(const ITargetAccepter& _deathTargetObj) override;
private:
	/// @brief �X�V�ł��邩�擾
	/// @return �X�V�ł��邩�H
	bool GetCanUpdate();

	/// @brief ���݂̍X�V����
	void Update() override;

	/// @brief  �^�[�Q�b�g�J�n���̂�����
	void OnBeginTargeting();

	/// @brief �^�[�Q�b�g�I�����̏���
	void OnEndTargeting();

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
	static constexpr auto CANCEL_PARAMNAME{ "canCancel" };

	/// @brief ��s���͂ł��邩��\���A�j���[�V�����p�����[�^
	static constexpr auto INPUT_PARAMNAME{ "canInput" };

	/// @brief �R���r�l�[�V�����U���ł��邩��\���A�j���[�V�����p�����[�^
	static constexpr auto COMBATK_PARAMNAME{ "canCombAttack" };
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *this);

	AddNode(static_cast<int>(_actionState), std::move(createState));
}
