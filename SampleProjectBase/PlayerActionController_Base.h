#pragma once
#include "CharacterActionController.h"
#include "PlayerActState_Base.h"

class CP_Player;
class PlayerAction;
class CP_BattleManager;

/// @brief �v���C���[�̋��ʃA�N�V�����R���g���[���[
class PlayerActionController_Base : public CharacterActionController
{
public:
	/// @brief �v���C���[���猩�����͂̕���
	enum class ActionPlace
	{
		Ground,	// �n��
		Air	// ��
	};

protected:
	/// @brief �ǂ̏ꏊ�̃R���g���[���[��
	ActionPlace place;

	/// @brief �v���C���[�A�N�V�����̃O���[�v�ϐ�
	PlayerAction* pAction;

	/// @brief ���͕ϐ�
	const GameInput* pInput;
public:
	PlayerActionController_Base(PlayerAction& _pAction, CP_Player& _player, const std::string& _controllerName);
	virtual ~PlayerActionController_Base() {}

	/// @brief ����������
	/// @param _pAnimation �A�j���[�V����
	/// @param _pRigidBody Rb
	void Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody) override;

	/// @brief  �I������
	void OnEnd();

	const ITargetAccepter* GetTargetAccepter() const;

	/// @brief �v���C���[�ɃL���X�g���Ď擾
	/// @return �v���C���[ 
	CP_Player& GetPlayer();

	/// @brief �J�������擾����
	/// @return �J����
	CP_Camera& GetCamera();

	/// @brief �^�[�Q�b�g��Ԃ��擾
	/// @return �^�[�Q�b�g��Ԃ��H
	bool GetIsTargeting() const;

	/// @brief ���͂ł��邩�擾
	/// @return ���͂ł��邩�H
	bool GetCanInput() const;

	/// @brief �U���ł��邩�擾
	/// @return �U���ł��邩�H
	bool GetCanAttack() const;

	/// @brief �A�N�V�����ł��邩�擾
	/// @return �A�N�V�����ł��邩�H
	bool GetCanAction() const;

	/// @brief �ړ��ł��邩�擾
	/// @return �ړ��ł��邩�H
	bool GetCanMove() const;
protected:
	/// @brief �V����State�𐶐�
	/// @tparam T �Ή����Ă���s���N���X
	/// @param _actionName �A�N�V������
	template <class T> void CreateState(int _actionState);

	/// @brief ��Ԃ�J��
	/// @param _nextActionState �A�N�V������ID
	/// @param _isForce �������ĕύX���邩�H
	bool ChangeState(int _nextActionState, bool _isForce);
private:
	/// @brief �X�V�ł��邩�擾
	/// @return �X�V�ł��邩�H
	bool GetCanUpdate();

	/// @brief �eState�̕�������擾����
	/// @param _stateId ��Ԃ�ID
	/// @return ������
	virtual std::string GetStateStr(int _stateId) = 0;

	/// @brief �eState��ID���擾����
	/// @param _stateName�@��Ԗ�
	/// @return ��Ԃ�ID
	virtual int GetStateId(const std::string& _stateName) = 0;
};

template<class T>
inline void PlayerActionController_Base::CreateState(int _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(*this);

	AddNode(static_cast<int>(_actionState), std::move(createState));
}
