#pragma once
#include "CP_Character.h"
#include "ITargetAccepter.h"

namespace HashiTaku
{
	class CP_Enemy : public CP_Character, public ITargetAccepter
	{
		/// @brief �G�̖���
		std::string enemyName;

		/// @brief ���g���^�[�Q�b�g�ɂ��Ă����
		std::list<IObjectTargeter*> pTargeters;

	public:
		CP_Enemy();
		virtual ~CP_Enemy() {}

		/// @brief �G�����擾
		/// @return �G��
		const std::string& GetEnemyName() const;

		/// @brief ���̃I�u�W�F�N�g�̃��[���h���W���擾
		/// @return ���[���h���W���擾
		const DXSimp::Vector3& GetWorldPos() const override;

		/// @brief �^�[�Q�b�g���鑤���擾����
		/// @param _targeter �^�[�Q�b�g���鑤
		void SetTargeter(IObjectTargeter& _targeter);

		/// @brief �^�[�Q�b�g���鑤���폜����
		/// @param _targeter �폜�^�[�Q�b�g���鑤
		void RemoveTargeter(IObjectTargeter& _targeter) override;

		/// @brief �^�[�Q�b�g���Ɏ��񂾂��Ƃ�ʒm
		void OnRemoveNotifyToTargeter();
	protected:
		void Start() override;
		void OnDestroy() override;

		/// @brief �G�����Z�b�g
		/// @param _enemyName �G��
		void SetEnemyName(const std::string& _enemyName);

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _attackerPos �U���������̍��W
		/// @return �_���[�W���󂯂����H
		bool OnDamageBehavior(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos) override;

		/// @brief �_���[�W�󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _contactPos �Փ˒n�_
		void OnTakeDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos) override;

		/// @brief ���񂾂Ƃ��̏���
		void OnDeathBehavior() override {};
	};
}