#pragma once
#include "StateMachine.h"
#include "ChangeAnimObserver.h"
#include "CP_Character.h"
#include "CP_Animation.h"
#include "CP_RigidBody.h"
//#include "AttackInformation.h"

class CharacterChangeAnimObserver;

/// @brief �L�����N�^�[�i�v���C���[�A�G�Ȃ�)�̃A�N�V�����R���g���[���[
class CharacterActionController :
	public HashiTaku::StateMachine_Base<int>, public HashiTaku::IImGuiUser,
	public HashiTaku::ISaveLoad
{
	/// @brief �A�j���[�V�����ύX�����Ƃ��̃I�u�U�[�o�[
	std::unique_ptr<CharacterChangeAnimObserver> pChangeAnimObserver;

protected:
	/// @brief �L�����N�^�[�R���|�[�l���g
	CP_Character* pCharacter;

	/// @brief �A�j���[�V�����R���|�[�l���g	
	CP_Animation* pAnimation;

	/// @brief �v���C���[�̃��W�b�h�{�f�B
	CP_RigidBody* pRigidBody;
#ifdef EDIT
	// �O��̏�ԃL�[
	int prevStateKey{ 0 };

	// �A�N�V��������Ńf�o�b�O�`�悷�邩�H
	bool isDebugDisplay{ false };
#endif // EDIT

public:
	CharacterActionController(CP_Character& _character, const std::string& _stateMachineName);
	~CharacterActionController() {}

	/// @brief ����������
	/// @param _pAnimation �A�j���[�V�����R���|�[�l���g
	/// @param _pRigidBody ���W�b�h�{�f�B
	virtual void Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody);

	/// @brief �_���[�W�󂯂��Ƃ��̃A�N�V��������
	/// @param _atkInfo �U�����
	virtual void OnDamage(const HashiTaku::AttackInformation& _atkInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos,
		bool* _pAcceptDamage = nullptr) {};

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[���擾
	/// @return �A�j���[�V�����ύX�I�u�U�[�o�[
	CharacterChangeAnimObserver& GetChangeAnimObserver();

	/// @brief RigidBody���擾
	/// @return RigidBody
	CP_RigidBody* GetRB();

	/// @brief �A�j���[�V�������擾
	/// @return �A�j���[�V�����R���|�[�l���g
	CP_Animation* GetAnimation();

	/// @brief �f�o�b�O�`����s��
	virtual void DebugDisplay();

	/// @brief �w�肵��bool�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _isBool �Z�b�g����l
	void SetAnimationBool(const std::string& _paramName, bool _isBool);

	/// @brief �w�肵��int�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _intVall �Z�b�g����l
	void SetAnimationInt(const std::string& _paramName, int _intVal);

	/// @brief �w�肵��float�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal �Z�b�g����l
	void SetAnimationFloat(const std::string& _paramName, float _floatVal);

	/// @brief �w�肵���g���K�[�ϐ���true�ɂ���
	/// @param _paramName �p�����[�^�[��
	void SetAnimationTrigger(const std::string& _paramName);

	/// @brief �w�肵��bool�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _isBool �擾����l
	bool GetAnimationBool(const std::string& _paramName);

	/// @brief �w�肵��int�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _intVall �擾����l
	int GetAnimationInt(const std::string& _paramName);

	/// @brief �w�肵��float�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal �擾����l
	float GetAnimationFloat(const std::string& _paramName);

	/// @brief �L�����N�^�[�R���|�[�l���g���擾
	/// @return �L�����N�^�[�R���|�[�l���g
	CP_Character& GetCharacter();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �A�j���[�V�����������Ă��邩�m�F
	/// @return �A�j���[�V���������Ă��邩�H
	bool CheckHaveAnimation();

	/// @brief �eState�̕�������擾����
	/// @param _stateId ��Ԃ�ID
	/// @return ������
	virtual std::string GetStateStr(int _stateId) = 0;

	/// @brief �eState��ID���擾����
	/// @param _stateName�@��Ԗ�
	/// @return ��Ԃ�ID
	virtual int GetStateId(const std::string& _stateName) = 0;

	virtual void ImGuiDebug();
};

/// @brief �L�����N�^�[���A�j���[�V�����J�ڂ����Ƃ��̃I�u�U�[�o�[
class CharacterChangeAnimObserver : public HashiTaku::ChangeAnimObserver
{
	/// @brief �A�N�V�����R���g���[���[
	CharacterActionController* pActionController;

public:
	/// @brief �R���X�g���N�^
	/// @param _playerActCon �A�N�V�����R���g���[���[
	/// @param _observerName �I�u�U�[�o�[��
	CharacterChangeAnimObserver(CharacterActionController& _playerActCon,
		const std::string& _observerName);
	~CharacterChangeAnimObserver() {}

	/// @brief �ʒm�������Ƃ��̏���
	void ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value);
};