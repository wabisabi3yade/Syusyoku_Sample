#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief �v���C���[�ړ����̃J�����̍s��
	class CameraOnMoveState : public CameraMoveState_Base
	{
		/// @brief �J�����|�W�V����
		DirectX::SimpleMath::Vector3 cameraPos;

		/// @brief �Ǐ]�̃��[���h���W
		DirectX::SimpleMath::Vector3 followPos;

		/// @brief ���͐��l
		DirectX::SimpleMath::Vector2 inputVal;

		/// @brief �X�e�B�b�N��|�����Ƃ��̗V�т̑傫��
		DirectX::SimpleMath::Vector2 inpDeadZone;

		/// @brief �������͂Ȃ��̂Ƃ��̍���
		float idleHeight;

		/// @brief ���݂�y����
		float currentHeight;

		/// @brief �Ώۂ̃I�u�W�F�N�g�Ƃ̍ő�y����
		float cameraHeightMax;

		/// @brief �Ώۂ̃I�u�W�F�N�g�Ƃ̍ŏ�y����
		float cameraHeightMin;

		/// @brief �c�����̑��x
		float verticalSpeed;

		/// @brief �ҋ@�̍����ɖ߂�Ƃ��̑��x����
		float returnVertRatio;

		/// @brief ��]���x
		float rotateSpeed;

		/// @brief �����̖ڕW�ւ̈ړ����x
		float horiSpeedToTarget;

		/// @brief ���S�Ƃ̊p�x(��Ԃ�ς��Ă��ێ�����ׂ�static)
		static float centerAngle;

		/// @brief �����̑ΏۂƂ̃J��������
		float distanceHorizon;

		/// @brief �^�[�Q�b�g����Y���W�̒����_
		float lookTargetOffsetY;

		/// @brief �^�[�Q�b�g�������邩�H
		bool isTargeting;
	public:
		CameraOnMoveState();
		virtual ~CameraOnMoveState() {}

		/// @brief �������W��n��
		void InitCameraTransform();

		json Save() override;
		void Load(const json& _data) override;

	protected:
		/// @brief �J�n����
		void OnStartBehavior() override;

		/// @brief �X�V����
		void UpdateBehavior() override;

		/// @brief �I������
		void OnEndBehavior() override;

		void ImGuiDebug() override;
	private:
		/// @brief �ʏ펞�̍X�V����
		void NormalUpdate();

		/// @brief �^�[�Q�b�g���̍X�V����
		void TargetUpdate();

		/// @brief �J�ڊm�F
		void CheckTransitionUpdate() override;

		/// @brief ���͍X�V
		void InputUpdate();

		/// @brief �c���̈ړ�
		void VerticalMove();

		/// @brief �ړ�����
		void RotationMove();

		/// @brief �����_���X�V
		void LookUpdate();
	};
}