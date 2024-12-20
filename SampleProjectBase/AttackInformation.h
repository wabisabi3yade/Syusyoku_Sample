#pragma once
#include "CameraShakeParameter.h"

namespace HashiTaku
{
	class VisualEffect;

	/// @brief  �U���̏��
	class AttackInformation : public ISaveLoad, public IImGuiUser
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
		/// @brief �q�b�g�G�t�F�N�g���
		CreateVfxInfo hitVfxInfo;

		/// @brief �J������h�炷�p�����[�^
		CameraShakeParameter pCamShakeParam;

		/// @brief �U�����̃_���[�W
		float atkDamage;

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[����
		u_int hitStopFrame;

		/// @brief �U�����x��
		AttackLevel atkLevel;

		/// @brief �J������h�炷���H
		bool isCamShake;
	public:
		AttackInformation();
		virtual ~AttackInformation() {}

		/// @brief �_���[�W�l���Z�b�g
		/// @param _atkDamage �_���[�W�l
		void SetDamageValue(float _atkDamage);

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[�������擾
		/// @return �q�b�g�X�g�b�v��
		void SetHitStopFlame(u_int _hitStopFlame);

		/// @brief �U�����x�����Z�b�g
		/// @param _atkLevel �U�����x��
		void SetAttackLevel(AttackLevel _atkLevel);

		/// @brief �q�b�g�G�t�F�N�g�����擾����
		/// @return �q�b�g�G�t�F�N�g���
		const CreateVfxInfo& GetHitVfxInfo() const;

		/// @brief �J������h�炷�p�����[�^���擾
		/// @return �J������h�炷�p�����[�^
		const CameraShakeParameter& GetCamShakeParam() const;

		/// @brief �_���[�W�l���擾
		/// @return �_���[�W�l
		float GetDamageValue() const;

		/// @brief �q�b�g�X�g�b�v�Ŏ~�߂�t���[�������擾
		/// @return �q�b�g�X�g�b�v��
		u_int GetHitStopFlame() const;

		/// @brief �U�����x�����擾
		/// @return �U�����x��
		AttackLevel GetAttackLevel() const;

		/// @brief �J������h�炷���ǂ���
		/// @return �J�����h�炷�H
		bool GetIsShake() const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	protected:
		void ImGuiDebug() override;
	private:
		/// @brief �U�����x������e�p�����[�^��K�p
		void ApplyFromAttackLevel();
	private:
		// Low
		static constexpr u_int LOW_HITSTOP = 0;

		// Mid
		static constexpr u_int MID_HITSTOP = 3;

		// High
		static constexpr u_int HIGH_HITSTOP = 5;

		// SuperHigh
		static constexpr u_int SUPERHIGH_HITSTOP = 8;
	};
}


