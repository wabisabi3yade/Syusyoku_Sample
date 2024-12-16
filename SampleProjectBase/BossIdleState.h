#pragma once
#include "BossGroundState.h"

namespace HashiTaku
{
	/// @brief �ҋ@���(�X�e�[�g�ƃX�e�[�g�̊Ԃ��q�������)
	class BossIdleState : public BossActState_Base
	{
		/// @brief ���̏��
		BossState nextState;

		/// @brief ���̏�Ԃ����܂������H
		bool isDecideNextState;

#ifdef EDIT
		bool isIdleOnly{ false };
#endif EDIT

	public:
		BossIdleState();
		~BossIdleState() {}

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief �J�n����
		void OnStartBehavior() override;
		/// @brief �X�V
		void UpdateBehavior() override;
		/// @brief  �J�ڐ؂�ւ�
		void TransitionCheckUpdate() override;

		void ImGuiDebug() override;
	};
}