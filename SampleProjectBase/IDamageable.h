#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief �_���[�W���󂯂鑤�̃C���^�[�t�F�[�X
	class IDamageable
	{

	public:
		IDamageable() {}
		virtual ~IDamageable() {}

		/// @brief �_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _attackerPos �U���������̂̍��W
		virtual void OnDamage(const AttackInformation& _attackInfo, const DirectX::SimpleMath::Vector3& _attackerPos) = 0;

		/// @brief ���񂾂Ƃ��̏���
		virtual void OnDeath() = 0;
	};
}
