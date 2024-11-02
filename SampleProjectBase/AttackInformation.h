#pragma once

namespace HashiTaku
{
	/// @brief  �U���̏��
	class AttackInformation
	{
	public:
		/// @brief �U�����x��
		enum class AttackLevel
		{
			Low, // ��
			Mid,	// ��
			High,	// ��
			SuperHigh	// �ő�
		};

	private:
		/// @brief �U�����̃_���[�W
		float atkDamage;

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[����
		u_int hitStopFlame;

		/// @brief �U�����x��
		AttackLevel atkLevel;
	public:
		AttackInformation();
		AttackInformation(float _atkDamage, AttackLevel _atkLevel);
		~AttackInformation() {}

		/// @brief �_���[�W�l���Z�b�g
		/// @param _atkDamage �_���[�W�l
		void SetDamageValue(float _atkDamage);

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[�������擾
		/// @return �q�b�g�X�g�b�v��
		void SetHitStopFlame(u_int _hitStopFlame);

		/// @brief �U�����x�����Z�b�g
		/// @param _atkLevel �U�����x��
		void SetAttackLevel(AttackLevel _atkLevel);

		/// @brief �_���[�W�l���擾
		/// @return �_���[�W�l
		float GetDamageValue() const;

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[�������擾
		/// @return �q�b�g�X�g�b�v��
		u_int GetHitStopFlame() const;

	private:
		/// @brief �U�����x������e�p�����[�^��K�p
		void ApplyFromAttackLevel();

	private:
		// Low
		static constexpr u_int LOW_HITSTOP = 6;

		// Mid
		static constexpr u_int MID_HITSTOP = 8;

		// High
		static constexpr u_int HIGH_HITSTOP = 10;

		// SuperHigh
		static constexpr u_int SUPERHIGH_HITSTOP = 12;
	};
}


