#pragma once

namespace HashiTaku
{
	/// @brief  �U���̏��
	class AttackInformation
	{
		/// @brief �U�����̃_���[�W
		float atkDamage;

	public:
		AttackInformation();
		AttackInformation(float _atkDamage);
		~AttackInformation() {}

		/// @brief �_���[�W�l���擾
		/// @return �_���[�W�l
		float GetDamageValue() const;
	};
}


