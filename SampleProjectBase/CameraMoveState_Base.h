#pragma once
#include "StateMachine.h"
#include "CP_Camera.h"
#include "GameInput.h"

namespace HashiTaku
{
	class CameraMoveController;

	/// @brief �J�����̍s�����N���X
	class CameraMoveState_Base : public StateNode_Base,
		public ISaveLoad, public IImGuiUser
	{
	public:
		enum  class CameraState
		{
			Follow,	// �ړ���
			Charge,	// �`���[�W��
			Win,	// �v���C���[����
			Lose,	// �v���C���[����
			None
		};		
	protected:
		/// @brief �J�����̍s���R���g���[��
		CameraMoveController* pCamController;

		/// @brief ����
		GameInput* pInput;

	public:
		CameraMoveState_Base();
		virtual ~CameraMoveState_Base() {}

		/// @brief ����������
		/// @param _cameraController �J�����R���g���[�� 
		void Init(CameraMoveController& _cameraController);

		/// @brief ��Ԑ؂�ւ��J�n����
		void OnStart() override;

		/// @brief �X�V����
		void Update() override;

		/// @brief ��Ԑ؂�ւ��I������
		void OnEnd() override;

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief �J�n�����̐U�镑��
		virtual void OnStartBehavior() {}

		/// @brief �X�V�����̐U�镑��
		virtual void UpdateBehavior() {}

		/// @brief �I�������̐U�镑��
		virtual void OnEndBehavior() {}

		/// @brief ���̃X�e�[�g�ɑJ�ڂ��邩�m�F
		virtual void CheckTransitionUpdate();

		/// @brief �J�����̃X�e�[�g��J��
		/// @param _camState �J�ڐ�̃J����
		void ChangeState(CameraState _camState, bool _isForce = false);

		/// @brief �x�[�X�ƂȂ�J�����̍��W���Z�b�g
		/// @param _basePos �x�[�X�ƂȂ���W
		void SetBasePosition(const DXSimp::Vector3& _basePos);

		/// @brief �J�����R���|�[�l���g���擾
		/// @return �J����
		CP_Camera& GetCamera();

		/// @brief �Ǐ]��̍��W���擾����
		/// @return  �Ǐ]��̍��W
		const DXSimp::Vector3& GetFollowPosition() const;

		/// @brief �x�[�X�ƂȂ���W���擾����
		/// @return	�J�����̃x�[�X�ƂȂ郏�[���h���W
		const DXSimp::Vector3& GetBasePosition() const;

		/// @brief �I�u�W�F�N�g�̌o�ߎ��Ԃ��擾
		/// @return �o�ߎ���
		float DeltaTime() const;

		void ImGuiDebug() override;
	};
}