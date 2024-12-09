#pragma once
#include "PlayerGroundActionController.h"
#include "PlayerAirActionController.h"
#include "ITargetAccepter.h"
#include "IGroundNotifyer.h"

class CP_Player;
class CP_GroundCheck;

/// @brief �v���C���[�A�N�V�����N���X
class PlayerAction : 
	public IObjectTargeter,
	public HashiTaku::IImGuiUser,
	public HashiTaku::ISaveLoad,
	public IGroundNotifyer
{
	using ActionPlace = PlayerActionController_Base::ActionPlace;
public:
	/// @brief �v���C���[���猩�����͂̕���
	enum class InputVector
	{
		Forward,	// �O����
		Back	// ���
	};

	// �󒆁@���@�n��
	enum class AirToGround
	{
		AirToLanding,	// ���n
		AirToGroundKnock,	// �n��m�b�N��
	};

	// �n��@���@��
	enum class GroundToAir
	{
		ToAirMove,	// �󒆈ړ���	
	};

private:
	/// @brief �ڒn�`�F�b�J�[�I�u�W�F�N�g��
	std::string groundCheckerName;

	/// @brief �n�ʔ�����m�F����R���|�[�l���g
	CP_GroundCheck* pGroundChecker;

	/// @brief �n��s���̃A�N�V�����R���g���[���[
	std::unique_ptr<PlayerGroundActionController>
		pGroundController;

	/// @brief �󒆍s���̃A�N�V�����R���g���[���[
	std::unique_ptr<PlayerAirActionController>
		pAirController;

	/// @brief ���ݎg�p���Ă���R���g���[���[�N���X
	PlayerActionController_Base* pCurrentController;

	/// @brief ���̓N���X
	GameInput* pInput;

	/// @brief �o�g���}�l�W���[
	CP_BattleManager* pBattleManager;

	/// @brief �^�[�Q�b�g���Ă���I�u�W�F�N�g��
	ITargetAccepter* pTargetAccepter;

	/// @brief �^�[�Q�b�g����J�����ɓ`����
	IObjectTargeter* pTargetCamera;

	/// @brief �v���C���[
	CP_Player* pPlayer;

	/// @brief �J����
	CP_Camera* pCamera;

	/// @brief �A�j���[�V����
	CP_Animation* pAnimation;

	/// @brief �L�����Z���t���O�̃|�C���^
	const bool* pIsCanCancel;

	/// @brief ���͉\�t���O�̃|�C���^
	const bool* pIsCanInput;

	/// @brief �U���\�̃|�C���^
	const bool* pIsCanAttack;

	/// @brief �ړ��\�̃|�C���^
	const bool* pIsCanMove;

	/// @brief ���݂̏ꏊ����
	ActionPlace currentPlace;

	/// @brief �󒆂ɕύX�����Ƃ��ɂǂ̏�Ԃ֌q���邩�ϐ�
	GroundToAir reserveAirTransition;

	/// @brief �n��ɕύX�����Ƃ��ɂǂ̏�Ԃ֌q���邩�ϐ�
	AirToGround reserveGroundTransition;

	/// @brief �n�ォ�ǂ����H
	bool isGround;

	/// @brief 1�t���[���O�̐ڒn���
	bool prevIsGround;

	/// @brief �^�[�Q�b�g�����ǂ���
	bool isTargeting;

	/// @brief 1�t���[���O�̃^�[�Q�b�g���
	bool prevIsTargeting;
public:
	/// @brief �R���X�g���N�^
	/// @param _player �v���C���[
	PlayerAction(CP_Player& _player);
	~PlayerAction();

	void Init(CP_Animation* _pAnimation,
		CP_RigidBody* _pRigidBody);

	/// @brief �X�V����
	void Update();

	/// @brief �A�N�V�����̏ꏊ��ύX����
	/// @param _setPlaceTransition �ǂ��ύX���邩�w�肷��
	void OnChangePlace();

	/// @brief �J�����擾
	/// @return �J����
	CP_Camera& GetCamera();

	/// @brief �^�[�Q�b�g��̃I�u�W�F�N�g���擾
	/// @return �^�[�Q�b�g��̃I�u�W�F�N�g(nullptr)
	const ITargetAccepter* GetTargetAccepter() const;

	/// @brief �R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	/// @return ���X�e�B�b�N�̓���
	const DirectX::SimpleMath::Vector2& GetInputLeftStick() const;

	/// @brief ���̕����ɓ��͂ł��Ă��邩�m�F����
	/// @param _checkVector �m�F����������
	/// @return �ł��Ă��邩�H
	bool IsInputVector(InputVector _checkVector);

	/// @brief �^�[�Q�b�g�����擾����
	/// @return �^�[�Q�b�g�����H
	bool GetIsTargeting() const;

	/// @brief ��s���͂ł��邩���擾
	/// @return ��s���͂ł��邩�H
	bool GetCanInput() const;

	/// @brief �L�����Z���ł��邩�擾
	/// @return �L�����Z���ł��邩�H
	bool GetCanCancel() const;

	/// @brief �R���r�l�[�V�����U���ł��邩���擾
	/// @return �R���r�l�[�V�����U���ł��邩�H
	bool GetCanAttack() const;

	/// @brief �ړ��ł��邩���擾
	/// @return �ړ��ł��邩�H
	bool GetCanMove() const;

	/// @brief �ڒn�t���O���擾
	/// @return �ڒn�t���O
	bool GetIsGround() const;

	/// @brief �^�[�Q�b�g�I�u�W�F�N�g���擾����
	/// @param _targetObject �^�[�Q�b�g�I�u�W�F�N�g
	void SetTargetObject(ITargetAccepter& _targetObject) override;

	/// @brief �^�[�Q�b�g�I�u�W�F�N�g�����񂾂Ƃ��̍X�V����
	/// @param _deathTargetObj ���񂾃^�[�Q�b�g�I�u�W�F�N�g
	void RemoveNotify(const ITargetAccepter& _deathTargetObj) override;

	/// @brief �_���[�W�󂯂��Ƃ��̃A�N�V��������
	/// @param _atkInfo �U�����
	/// @param _attackerPos �U���ϓG�̍��W
	/// @param _pAcceptDamage �_���[�W�󂯂����H
	void OnDamage(const HashiTaku::AttackInformation& _atkInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos,
		bool* _pAcceptDamage = nullptr);

	/// @brief �n�ʂɕt�����u�Ԃ̏���
	void OnGroundEnter() override;

	/// @brief �n�ʂ��痣�ꂽ�u�Ԃ̏���
	void OnGroundExit()	override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �ڒn�`�F�b�J�[��T���Ď擾
	void FindGroundChecker();

	/// @brief �X�V�ł��邩�擾
	/// @return �X�V�ł��邩�H
	bool GetCanUpdate();

	/// @brief �A�N�V�����X�V
	void ActionUpdate();

	/// @brief  �^�[�Q�b�g�J�n���̂�����
	void OnBeginTargeting();

	/// @brief �^�[�Q�b�g�I�����̏���
	void OnEndTargeting();

	/// @brief �^�[�Q�b�g�̍X�V����
	void UpdateTargeting();

	/// @brief �ڒn��Ԃ̍X�V����
	void UpdateGround();

	/// @brief �n�ォ��󒆂ւ̏���
	void OnGroundToAir();

	/// @brief �󒆂���n��ւ̏���
	void OnAirToGround();

	void ImGuiDebug() override;

	// �ڒn�`�F�b�J�[
	void ImGuiGroundChecker();
};

