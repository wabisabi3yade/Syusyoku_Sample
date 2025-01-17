#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief �������̃J�������o(Devil May Cry5����)
	class CameraWinState : public CameraMoveState_Base
	{
		enum class DirectionStep
		{
			Move, // �ړ�
			Zoom, // �Y�[��
			Wait, // �ҋ@
			End	// �I��
		};

		/// @brief �J�����ɉf���Ώۂ̃g�����X�t�H�[��
		const Transform* pTargetTransform;

		/// @brief �f���Ώۂ̍��W���璆�S���W�܂ł̂̃I�t�Z�b�g
		DXSimp::Vector3 centerPosOffset;

		/// @brief �J������������Ώۂ̉�]��
		DXSimp::Quaternion lookTargetRotation;

		/// @brief ���o�J�n���̃J�����̉�]��
		DXSimp::Quaternion beginCamRotation;

		/// @brief �ړ���̍��W
		DXSimp::Vector3 moveTargetPos;

		/// @brief ���o�J�n���̃J�����̍��W
		DXSimp::Vector3 beginCamPos;

		/// @brief �����_���ŋ��߂�^�[�Q�b�g�܂ł̃x�N�g���͈̔́i�ő�j
		DXSimp::Vector3 randTargetVecMax;

		/// @brief �����_���ŋ��߂�^�[�Q�b�g�܂ł̃x�N�g���͈̔́i�ŏ��j
		DXSimp::Vector3 randTargetVecMin;

		/// @brief ���݂̃X�e�b�v
		DirectionStep curStep;

		/// @brief ���ɍs���X�e�b�v
		DirectionStep nextStep;

		/// @brief �ړ����̃C�[�W���O
		EaseKind moveEaseKind;

		/// @brief ���_�ړ����̃C�[�W���O
		EaseKind lookEaseKind;

		/// @brief ���_�ړ����̎���p�C�[�W���O
		EaseKind fovEaseKindOnMove;

		/// @brief �Y�[�����̎���p�C�[�W���O
		EaseKind fovEaseKindOnZoom;

		/// @brief �^�[�Q�b�g�܂ł̋���
		float targetToDistance;

		/// @brief �ړ����o����
		float moveTime;

		/// @brief �ҋ@���o����(1���)
		float firstWaitTime;

		/// @brief �Y�[�����̉��o����
		float zoomTime;

		/// @brief �ҋ@���o����(2���)
		float secondWaitTime;

		/// @brief ���ݐݒ肳��Ă���ҋ@����
		float curTargetWaitTime;

		/// @brief ���݂̌o�ߎ���
		float elapsedTime;

		/// @brief �ړ����̃^�[�Q�b�g����p
		float targetFovOnMove;

		/// @brief �Y�[�����̃^�[�Q�b�g����p
		float targetFovOnZoom;

		/// @brief �J�n���̎���p
		float beginFov;
	public:
		CameraWinState();
		~CameraWinState() {}

		/// @brief �^�[�Q�b�g�̃g�����X�t�H�[�����Z�b�g
		/// @param _targetTransform �^�[�Q�b�g�̃g�����X�t�H�[��
		void SetTargetTransform(const Transform& _targetTransform);

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �J�n�����̐U�镑��
		void OnStartBehavior() override;

		/// @brief �X�V�����̐U�镑��
		void UpdateBehavior() override;

		/// @brief �e�X�e�b�v�̊J�n�������s��
		/// @param _nextStep ���̃X�e�b�v
		void BeginDirection(DirectionStep _nextStep);

		/// @brief �^�[�Q�b�g�܂ł̃x�N�g���𗐐��Ō��߂�
		/// @return �����ŋ��߂��x�N�g��
		DXSimp::Vector3 GetRandomTargetVec();

		/// @brief �ړ����̊J�n����
		void BeginMove();

		/// @brief �߂Â��Ƃ��̏���
		void MoveUpdate();

		/// @brief �ҋ@�̊J�n����
		/// @param _waitTime �ҋ@����
		/// @param _nextStep �ҋ@���I�������̎��̃X�e�b�v
		void BeginWait(float _waitTime, DirectionStep _nextStep);

		/// @brief �ҋ@�X�V����
		void WaitUpdate();

		/// @brief �Y�[���J�n����
		void BeginZoom();

		/// @brief �Y�[������
		void ZoomUpdate();

		/// @brief �I�����̏���
		void EndProcess();

		// �p�����[�^�ҏW
		void ImGuiDebug() override;
		void ImGuiMove();
		void ImGuiZoom();
		void ImGuiWait();
	};
}

