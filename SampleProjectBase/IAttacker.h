#pragma once

namespace HashiTaku
{
	class IDamageable;

	// �U��������C���^�[�t�F�[�X
	class IAttacker
	{

	public:
		IAttacker() {}
		virtual ~IAttacker() {}

		/// @brief �U������
		/// @param _damager �U���^����Ώ�
		virtual void OnAttack(IDamageable& _damager) = 0;
	};
}

