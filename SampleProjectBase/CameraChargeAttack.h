#pragma once
#include "CameraFollowMove.h"
#include "CalcPerlinShakeVector.h"

namespace HashiTaku
{
	/// @brief �v���C���[�̃`���[�W�U�����̃J�����X�e�[�g
	class CameraChargeAttack : public CameraFollowMove
	{
		/// @brief �`���[�W���̃V�F�C�N���v�Z����
		CalcPerlinShakeVector calcChargeingShake;

		/// @brief �`���[�W���̃V�F�C�N���
		PerlinShakeParameter chargeingShakeParam;

		/// @brief �`���[�W���̃^�[�Q�b�g��Fov
		float chargeTargetFov;

		/// @brief ����p�̈ړ�����
		float moveTargetFovTime;

		/// @brief ����p�ړ��o�ߎ���
		float elapsedMoveFovTime;

		/// @brief ����p�ړ��J�n���̎���p
		float fovOnBeginMove;

		/// @brief ����p�̈ړ��C�[�W���O
		EaseKind moveFovEase;
	public:
		CameraChargeAttack();
		~CameraChargeAttack() {}

		/// @brief ����p�ړ����J�n����
		/// @param _targetFov �^�[�Q�b�g�̎���p
		/// @param _moveTime �ړ�����
		void BeginMoveFov(float _targetFov, float _moveTime);

		/// @brief �`���[�W���̐U�����~�߂�
		void EndChargingShake();

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �J�n����
		void OnStartBehavior() override;

		/// @brief �X�V����
		void UpdateBehavior() override;

		/// @brief �I��
		void OnEndBehavior() override;

		/// @brief ����p�ړ��X�V
		void UpdateFovMove();

		/// @brief �`���[�W���̃V�F�C�N�X�V
		void ChargeShakeUpdate();

		void ImGuiDebug() override;
	};

}

