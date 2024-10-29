#pragma once

namespace HashiTaku
{
	/// @brief  �U���̏��
	class AttackInformation
	{
		/// @brief �U�����̃_���[�W
		float atkDamage;

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[����
		u_int hitStopFlame;

	public:
		AttackInformation();
		AttackInformation(float _atkDamage, u_int _hitStopFlame);
		~AttackInformation() {}

		/// @brief �_���[�W�l���擾
		/// @return �_���[�W�l
		float GetDamageValue() const;

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[�������擾
		/// @return �q�b�g�X�g�b�v��
		u_int GetHitStopFlame() const;
	};
}


