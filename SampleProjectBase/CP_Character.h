#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "HitStopper_Base.h"

namespace HashiTaku
{
	/// @brief �v���C���[��G�̊��R���|�[�l���g
	class CP_Character : public Component, public IDamageable, public HitStopper_Base
	{
	private:
		/// @brief ���񂾃t���O
		bool isDead;

		/// @brief �q�b�g�X�g�b�v�����H
		bool isHitStopping;
	protected:
		/// @brief ���݂̗̑�
		float currentHP;

		/// @brief �ő�̗�
		float maxHP;

		/// @brief �ő�̗͐���
		static constexpr float MAXLIMIT_HP = 9999.9f;

	public:
		CP_Character();
		virtual ~CP_Character() {}

		/// @brief �ő�̗͂��Z�b�g
		/// @param _hitPoint �Z�b�g����̗�
		void SetMaxHP(float _hitPoint);

		/// @brief �̗͂��擾
		/// @return ���݂̗̑�
		float GetCurrentHP() const;

		/// @brief �_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		void OnDamage(const AttackInformation& _attackInfo);

		/// @brief ���񂾂Ƃ��̏���
		void OnDeath();

		/// @brief �q�b�g�X�g�b�v�J�n�������̏���
		void OnHitStopBegin() override;

		/// @brief �q�b�g�X�g�b�v�I���������̏���
		void OnHitStopEnd() override;

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const nlohmann::json& _data) override;
	protected:
		/// @brief �q�b�g�X�g�b�v���؎擾
		/// @return �q�b�g�X�g�b�v���Ă邩�H
		bool GetIsHitStopping() const;

		void Start() override;

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		virtual void OnDamageBehavior(const AttackInformation& _attackInfo) = 0;

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		virtual void OnDeathBehavior() = 0;

		void ImGuiSetting() override;
	};
}


