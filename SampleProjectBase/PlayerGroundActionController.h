#pragma once
#include "PlayerActionController_Base.h"
#include "PlayerGroundState.h"

namespace HashiTaku
{
	/// @brief �v���C���[�̒n��s�����p�����Ă��邩�R���Z�v�g
	template<typename T>
	concept PlayerGroundConcept =
		std::is_base_of_v<PlayerGroundState, T>;

	/// @brief �v���C���[�̒n��s���R���g���[���[
	class PlayerGroundActionController : public PlayerActionController_Base
	{
	public:
		using GroundState = PlayerGroundState::PlayerState;

		/// @brief �R���X�g���N�^
		/// @param _player �v���C���[�R���|�[�l���g
		PlayerGroundActionController(PlayerAction& _pAction, CP_Player& _player);
		~PlayerGroundActionController();

		/// @brief �J�n����
		/// @param _animation �A�j���[�V����
		/// @param _pRigidBody ���W�b�h�{�f�B
		/// @param �^�[�Q�b�g���Ɉꏏ�Ɍ���J����
		void Init(CP_Animation* _animation, CP_RigidBody* _pRigidBody) override;

		/// @brief ���݂̍X�V����
		void Update() override;

		/// @brief �s����Ԃ�؂�ւ���
		/// @param _nextActionName ���̏�Ԃ̖��O
		/// @param _isForce �����Ő؂�ւ����邩�H
		bool ChangeGroundState(const GroundState& _nextActionState, bool _isForce = false);

		/// @brief �_���[�W�󂯂��Ƃ��̃A�N�V��������
		/// @param _atkInfo �U�����
		/// @param _pAcceptDamage �_���[�W�󂯂����H
		void OnDamage(AttackInformation& _atkInfo,
			bool* _pAcceptDamage = nullptr) override;

		/// @brief �^�[�Q�b�g�����擾����
		/// @return �^�[�Q�b�g�����H
		bool GetIsTargeting() const;

		/// @brief �J�����擾
		/// @return �J����
		CP_Camera& GetCamera();

		/// @brief ���݂̒n��s���̃X�e�[�g���擾����
		/// @return �n��s���̃X�e�[�g
		PlayerGroundState* GetCurrentGroundState();


		/// @brief ���݂̃v���C���[�̏�ԃ^�C�v���擾����
		/// @return ���݂̃v���C���[�̏�ԃ^�C�v
		PlayerGroundState::PlayerState GetCurrentStateType() const;

		/// @brief �eState�̕�������擾����
		/// @param _stateId ��Ԃ�ID
		/// @return ������
		std::string GetStateStr(int _stateId) override;

		/// @brief �eState��ID���擾����
		/// @param _stateName�@��Ԗ�
		/// @return ��Ԃ�ID
		int GetStateId(const std::string& _stateName);
	private:
		/// @brief �V����State�𐶐�
		/// @tparam T �Ή����Ă���s���N���X
		/// @param _actionState �A�N�V������
		/// @param _cancelType �L�����Z���^�C�v
		template <PlayerGroundConcept T> void CreateState(GroundState _actionState,
			CancelType _cancelType);

		/// @brief �X�V�ł��邩�擾
		/// @return �X�V�ł��邩�H
		bool GetCanUpdate();

		/// @brief �m�b�N��ԂɑJ��
		/// @param _atkInfo �U�����
		/// @param _attackerPos �U���G�̍��W
		void ChangeKnockState(const AttackInformation& _atkInfo);

		void ImGuiDebug() override;
	private:
		/// @brief �A�j���[�V�����R���g���[�����̃v���C���[��
		static constexpr  auto STATEANIM_PARAMNAME{ "state" };
	};

	template<PlayerGroundConcept T>
	inline void PlayerGroundActionController::CreateState(GroundState _actionState,
		CancelType _cancelType)
	{
		PlayerActionController_Base::CreateState<T>(static_cast<int>(_actionState),
			_cancelType);
	}
}