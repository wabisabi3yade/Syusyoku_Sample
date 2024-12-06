#pragma once
#include "PlayerActionController_Base.h"
#include "ITargetAccepter.h"

/// @brief �v���C���[�A�N�V�����N���X
class PlayerAction : 
	public IObjectTargeter,
	public HashiTaku::IImGuiUser,
	public HashiTaku::ISaveLoad
{
public:
	/// @brief �v���C���[���猩�����͂̕���
	enum class InputVector
	{
		Forward,	// �O����
		Back	// ���
	};

private:
	/// @brief �n��s���̃A�N�V�����R���g���[���[
	std::unique_ptr<PlayerActionController>
		pGroundContoroller;

	/// @brief �󒆍s���̃A�N�V�����R���g���[���[
	std::unique_ptr<PlayerActionController>
		pAirContoroller;

	/// @brief ���̓N���X
	GameInput* pInput;

	/// @brief �o�g���}�l�W���[
	CP_BattleManager* pBattleManager;

	/// @brief �^�[�Q�b�g���Ă���I�u�W�F�N�g��
	ITargetAccepter* pTargetObject;

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

	/// @brief �^�[�Q�b�g�����ǂ���
	bool isTargeting;

	/// @brief 1�t���[���O�̃^�[�Q�b�g���
	bool prevIsTargeting;
public:
	PlayerAction();
	~PlayerAction();

	void Init(CP_Animation* _pAnimation,
		CP_RigidBody* _pRigidBody);

	/// @brief �X�V����
	void Update();

	/// @brief �J�����擾
	/// @return �J����
	CP_Camera& GetCamera();

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
	bool GetIsCanCancel() const;

	/// @brief �R���r�l�[�V�����U���ł��邩���擾
	/// @return �R���r�l�[�V�����U���ł��邩�H
	bool GetCanAttack() const;

	/// @brief �ړ��ł��邩���擾
	/// @return �ړ��ł��邩�H
	bool GetCanMove() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief  �^�[�Q�b�g�J�n���̂�����
	void OnBeginTargeting();

	/// @brief �^�[�Q�b�g�I�����̏���
	void OnEndTargeting();

	/// @brief �^�[�Q�b�g�̍X�V����
	void UpdateTargeting();

	void ImGuiDebug();
};

