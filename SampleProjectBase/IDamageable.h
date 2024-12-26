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
		/// @param _contactPos �Փ˒n�_���W
		virtual void OnDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos,
			const DXSimp::Vector3& _contactPos) = 0;

		/// @brief ���񂾂Ƃ��̏���
		virtual void OnDeath() = 0;
	};
}
