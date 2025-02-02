#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "HitStopper_Base.h"

namespace HashiTaku
{
	class CP_CameraMove;

	/// @brief �v���C���[��G�̊��R���|�[�l���g
	class CP_Character : public Component, public IDamageable, public HitStopper_Base
	{
	private:
		/// @brief ���񂾃t���O
		bool isDead;

		/// @brief �q�b�g�X�g�b�v�����H
		bool isHitStopping;

		/// @brief ���G�t���O
		bool isInvicible;
	protected:
		/// @brief ���݂̗̑�
		float currentHP;

		/// @brief HS������O�̃I�u�W�F�N�g�̌o�ߎ���
		float hsBeforeDeltaTime;

		/// @brief �ő�̗�
		float maxHP;

		/// @brief �ő�̗͐���
		static constexpr float MAXLIMIT_HP = 9999.9f;
	public:
		CP_Character();
		virtual ~CP_Character() {}

		/// @brief �I�u�W�F�N�g�폜���̍s��
		void OnDestroy() override;

		/// @brief �ő�̗͂��Z�b�g
		/// @param _hitPoint �Z�b�g����̗�
		virtual void SetMaxHP(float _hitPoint);

		/// @brief ���G�t���O���Z�b�g
		/// @param _isInvicible ���G�t���O
		void SetIsInvicible(bool _isInvicible);

		/// @brief �̗͂��擾
		/// @return ���݂̗̑�
		float GetCurrentHP() const;

		/// @brief ���G�t���O���擾
		/// @return ���G���H
		bool GetIsInvicible() const;

		/// @brief ����ł��邩�擾
		/// @return ����ł��邩�H
		bool GetDead() const;

		/// @brief �_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		void OnDamage(AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos) override;

		/// @brief ���񂾂Ƃ��̏���
		void OnDeath();

		/// @brief �q�b�g�X�g�b�v�J�n�������̏���
		void OnHitStopBegin() override;

		/// @brief �q�b�g�X�g�b�v�I���������̏���
		void OnHitStopEnd() override;

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;
	protected:
		/// @brief �q�b�g�X�g�b�v���؎擾
		/// @return �q�b�g�X�g�b�v���Ă邩�H
		bool GetIsHitStopping() const;

		void Start() override;

		/// @brief �q�b�g�X�g�b�v���J�n����
		/// @param _hitStopFlame �q�b�g�X�g�b�v�̃t���[����
		void BeginHitStop(u_int _hitStopFlame);

		/// @brief �̗͂��Z�b�g
		/// @param _setHp �̗�
		virtual void SetCurrentHP(float _setHp);

		/// @brief HP�����炷
		/// @param _damageVal �_���[�W�l
		void DecadeHp(float _damageVal);

		/// @brief �G�t�F�N�g���o��
		/// @param _attackInfo �U�����
		/// @param �����蔻��̏Փ˒n�_
		void CreateVfx(const CreateVfxInfo& _vfxInfo,
			const DXSimp::Vector3& _createPos);

		/// @brief SE���Đ�
		/// @param _attackInfo �U�����
		/// @param �����蔻��̏Փ˒n�_
		void CreateSoundFX(const PlaySoundParameter& _soundParam,
			const DXSimp::Vector3& _soundPos);

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _attackerPos �U���������̍��W
		/// @return �_���[�W���󂯂����H
		virtual bool OnDamageBehavior(AttackInformation& _attackInfo) = 0;

		/// @brief �_���[�W�󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _contactPos �Փ˒n�_
		virtual void OnTakeDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos);

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		virtual void OnDeathBehavior() = 0;

		void ImGuiDebug() override;
	};
}