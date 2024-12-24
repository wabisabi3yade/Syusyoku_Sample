#pragma once
#include "BossGroundState.h"

namespace HashiTaku
{
	/// @brief �{�X�n��ړ�
	class BossGroundMove : public BossActState_Base
	{
		/// @brief  �ő�ړ����x
		float maxSpeed;

		/// @brief �����x
		float acceleration;

		/// @brief ���ݑ��x
		float currentSpeed;

		/// @brief ��]���x
		float rotateSpeed;
	public:
		BossGroundMove();
		virtual ~BossGroundMove() {}

		json Save() override;
		void Load(const json& _data) override;

	protected:
		/// @brief �J�ڐ؂�ւ�
		void TransitionCheckUpdate() override;

		void ImGuiDebug() override;
	private:
		/// @brief �X�V����
		void UpdateBehavior() override;

		/// @brief ��Ԑ؂�ւ��I������
		void OnEndBehavior() override;

		/// @brief ���g����]����
		void Rotation();

		/// @brief �ړ�����
		void Move();

		/// @brief �ߐڂ̋����ɂȂ��Ă��邩�m�F
		/// @return �J�ڂ��邩�H
		bool CheckNearTransition();
	};
}