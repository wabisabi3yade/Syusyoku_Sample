#pragma once
#include "PerlinShakeParameter.h"

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
			SuperHigh,	// �ő�
			MaxNum
		};

	private:
		/// @brief �q�b�g�G�t�F�N�g���
		CreateVfxInfo hitVfxInfo;

		/// @brief �J������h�炷�p�����[�^
		PerlinShakeParameter pCamShakeParam;

		/// @brief �q�b�g���̃p�����[�^
		PlaySoundParameter hitSoundParameter;

		/// @brief �U�����̃_���[�W
		float atkDamage;

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

		/// @brief �U�����x�����Z�b�g
		/// @param _atkLevel �U�����x��
		void SetAttackLevel(AttackLevel _atkLevel);

		/// @brief �q�b�g�G�t�F�N�g�����擾����
		/// @return �q�b�g�G�t�F�N�g���
		const CreateVfxInfo& GetHitVfxInfo() const;

		/// @brief �J������h�炷�p�����[�^���擾
		/// @return �J������h�炷�p�����[�^
		const PerlinShakeParameter& GetCamShakeParam() const;

		/// @brief �q�b�gSE���擾
		/// @return �q�b�gSE
		const PlaySoundParameter& GetHitSEParam() const;

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
		bool GetIsCamShake() const;

		/// @brief �p�b�h�U���͂��擾
		/// @return �p�b�h�U���̗�
		float GetPadShakePower() const;

		/// @brief �p�b�h�U���̎��Ԏ擾
		/// @return �p�b�h�U������
		float GetPadShakeTime() const;

		json Save() override;
		void Load(const json& _data) override;
	protected:

		void ImGuiDebug() override;
	private:

		// �U�����x���ŕύX����p�����[�^
		void ImGuiLevelParamerter();
	private:
		static std::array<u_int, static_cast<u_int>(AttackLevel::MaxNum)> hitStopFrames;

		static std::array<float, static_cast<u_int>(AttackLevel::MaxNum)> padShakePowers;

		static std::array<float, static_cast<u_int>(AttackLevel::MaxNum)> padShakeTimes;
	};
}


