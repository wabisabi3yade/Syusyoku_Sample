#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief  �^�[�Q�b�g���̃J����
	class CameraTargetMove : public CameraMoveState_Base
	{
		/// @brief ��������Ƃ��̉�]���x
		float targetLookRotSpeed;

		/// @brief �v���C���[�Ƃ̋���
		float horiDisLength;

		/// @brief �Ǐ]�J�������x
		float followMoveSpeed;

		/// @brief �Ǐ]��̃I�t�Z�b�gY���W
		float followOffsetY;

		/// @brief �����X���^�[�Q�b�g�̍��W����y���W�����炷
		float targetLookOffsetY;

		/// @brief �Ǐ]�悩��̍ő�̍���
		float maxHeight;

		/// @brief �Ǐ]�悩��̍Œ�̍���
		float minHeight;
	public:
		CameraTargetMove();
		~CameraTargetMove() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �J�n�����̐U�镑��
		void OnStartBehavior() override;

		/// @brief �X�V�����̐U�镑��
		void UpdateBehavior()  override;

		/// @brief �I�������̐U�镑��
		void OnEndBehavior()  override;

		/// @brief �J�ڈړ�
		void CheckTransitionUpdate() override;

		/// @brief �X�V�ł��邩�擾
		/// @return �X�V�ł��邩
		bool CanUpdate();

		/// @brief �v���C���[�Ǐ]����
		void FollowMove();

		/// @brief ���_�̓���
		void LookMove();

		void ImGuiDebug() override;
	};
}