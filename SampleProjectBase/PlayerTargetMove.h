#pragma once
#include "PlayerMoveState.h"

namespace HashiTaku
{
	class PlayerTargetMove : public PlayerMoveState
	{
		/// @brief ��]���x
		float rotateSpeed;

	public:
		PlayerTargetMove();
		~PlayerTargetMove() {}

		/// @brief �Z�[�u����
		/// @return �Z�[�u�f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		void Load(const json& _data) override;
	private:
		void UpdateBehavior() override;
		void OnEndBehavior() override;
		void TransitionCheckUpdate() override;

		/// @brief �u�����h�������Z�b�g
		void ApplyBlendAnim();

		/// @brief ���[�g���[�V�������Z�b�g
		void ApplyRootMotion();

		void ImGuiDebug() override;
	};
}
