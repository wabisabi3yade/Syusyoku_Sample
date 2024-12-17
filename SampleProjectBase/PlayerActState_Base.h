#pragma once
#include "CharacterActState_Base.h"
#include "GameInput.h"
#include "CP_Camera.h"
#include "CP_RigidBody.h"
#include "CP_Animation.h"
#include "ITargetAccepter.h"
#include "PlayerAnimReference.h"

namespace HashiTaku
{
	class CP_Player;
	class PlayerActionController_Base;

	/// @brief �v���C���[�̍s���X�e�[�g���N���X
	class PlayerActState_Base : public CharacterActState_Base
	{
	public:
		/// @brief �v���C���[���猩�����͂̕���
		enum class InputVector
		{
			Forward,	// �O����
			Back	// ���
		};

		/// @brief �L�����Z���̎��
		enum class CancelType
		{
			Action,	// �A�N�V����(���[�����O�Ȃ�)
			Attack,	// �U��
			Move,	// �ړ�
			MaxNum,
			None	// �L�����Z�����Ȃ�
		};
	private:
		/// @brief �L�����Z���^�C�v
		CancelType cancelType;

		/// @brief �^�[�Q�b�g������Ƃ��̉�]���x
		float targetLookRotateSpeed;

		/// @brief �s�����N����D�揇��(�傫�������D��)
		int statePriority;

		/// @brief �^�[�Q�b�g���ɓG������s���ɂ��邩�H
		bool isTargetLookAtEnemy;
	protected:
		/// @brief �v���C���[�A�N�V�����R���g���[���[
		PlayerActionController_Base* pActionController;

		/// @brief �Q�[�����̓N���X
		static GameInput* pPlayerInput;
	public:
		PlayerActState_Base();
		virtual ~PlayerActState_Base() {}

		/// @brief ����������
		/// @param _actController�@�v���C���[�R���|�[�l���g
		/// @param _cancelType�@�L�����Z���^�C�v
		/// @param _priority�@�D��x
		void Init(PlayerActionController_Base& _actController, 
			CancelType _cancelType, 
			int _priority);

		/// @brief �X�V�����Ăяo��
		void Update() override;

		/// @brief �L�����Z���^�C�v���擾
		/// @return �L�����Z���^�C�v
		CancelType GetCancelType() const;

		/// @brief �D��x���擾����
		/// @return �D��x
		int GetPriority() const;

		/// @brief �Z�[�u����
		/// @return �Z�[�u�f�[�^
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		void Load(const nlohmann::json& _data) override;
	protected:
		/// @brief ���x���N���A����
		/// @param _applyY Y���ɂ����f�����邩
		void ClearVelocity(bool _applyY = true);

		/// @brief ���̍s���̓^�[�Q�b�g������̂��Z�b�g
		/// @param _isLook ����̂��H
		void SetTargetAtEnemy(bool _isLook);

		/// @brief ���G�ɂ��邩�H
		/// @param _isInvicible ���G
		void SetInvicible(bool _isInvicible);

		/// @brief �v���C���[���擾����
		/// @return �v���C���[
		CP_Player& GetPlayer();

		/// @brief RigidBody���擾
		/// @return RigidBody
		CP_RigidBody& GetRB();

		/// @brief �v���C���[�̃g�����X�t�H�[�����擾
		/// @return �g�����X�t�H�[��
		Transform& GetMyTransform();

		/// @brief �A�j���[�V�������擾����
		/// @return �A�j���[�V����
		CP_Animation* GetAnimation();

		/// @brief �^�[�Q�b�g��̃|�C���^���擾
		/// @return �^�[�Q�b�g��̃|�C���^
		const ITargetAccepter* GetTargetAccepter();

		/// @brief ��t���擾
		/// @return ��t
		float DeltaTime() const;

		/// @brief ��t��i�߂鑬�x���擾
		/// @return ��t�i�߂鑬�x
		float GetDeltaSpeed() const;

		/// @brief �R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		/// @return ���X�e�B�b�N�̓���
		DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

		/// @brief �J�������猩���R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		/// @return �J�������猩�����X�e�B�b�N�̓���(y��Z������)
		DirectX::SimpleMath::Vector2 GetInputLeftStickFromCam() const;

		/// @brief �U������G�̍��W���擾����
		/// @return �G�̍��W
		DirectX::SimpleMath::Vector3 GetAtkEnemyPos();

		/// @brief ���̕����ɓ��͂ł��Ă��邩�m�F����
		/// @param _checkVector �m�F����������
		/// @return �ł��Ă��邩�H
		bool IsInputVector(InputVector _checkVector);

		/// @brief ���[�����O���͂ł��Ă��邩�H
		/// @return ���[�����O�ł��邩�H
		bool IsRollingInput() const;

		/// @brief �K�E�Z���͂ł��Ă��邩�H
		/// @param _inputVecter �����̓���
		/// @return  �K�E�Z�ł��邩�H
		bool IsSpecialAtkInput(InputVector _inputVecter);

		void ImGuiDebug() override {};
	private:
		/// @brief �^�[�Q�b�g�̕���������
		void UpdateTargetLook();
	protected:
		// �A�j���[�V�����R���g���[�����̃v���C���[��
		constexpr static auto SPEEDRATIO_PARAMNAME{ "speed" };
		constexpr static auto MOVEAXIS_X_PARAMNAME{ "axisX" };	// X�ړ�
		constexpr static auto MOVEAXIS_Y_PARAMNAME{ "axisY" };	// Y�ړ�
		constexpr static auto DAMAGETRIGGER_PARAMNAME{ "damageTrigger" };	// �󂯃_���[�W�g���K�[
		constexpr static auto ATTACKTRIGGER_PARAMNAME{ "attackTrigger" };	// �U���g���K�[
		constexpr static auto REATTACK_PARAMNAME{ "isReAttack" };	// ���A�^�b�N��bool
		// �L�����Z���ł��邩��\���A�j���[�V�����p�����[�^
		constexpr static auto CANCEL_PARAMNAME{ "canCancel" };

		// �A�j���[�V������
		constexpr static auto IDLE_ANIM_NAME{ "Idle" };	// �ҋ@���
	};
}