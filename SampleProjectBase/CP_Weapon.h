#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "Tag.h"
#include "IAttacker.h"
#include "IParryAccepter.h"

namespace HashiTaku
{
	class CP_RigidBody;

	/// @brief ����R���|�[�l���g
	class CP_Weapon : public Component
	{
		/// @brief �U���Ώۃ^�O�̏��
		static constexpr u_int ATTACK_TAG_MAX{ 3 };

		/// @brief �U�����
		AttackInformation* pAtkInfomation;

		/// @brief �U���ɂ�����^�O
		std::array<Tag::Type, ATTACK_TAG_MAX> attackableTags;

		/// @brief ���̍U������ŏd�����Ȃ��悤�ɋL�^����p���X�g
		std::vector<const CP_RigidBody*> attackedRbs;

		/// @brief ���폊�L�҂̃|�C���^
		IAttacker* pAttacker;

		/// @brief �U���^�O�̐�
		u_int attackTagCnt;

		/// @brief �U������R���W����
		bool isAttackCollision;

#ifdef EDIT
		/// @brief �U���t���O�`��
		bool isDebugAttackDisplay{ true };
#endif // EDIT

	public:
		CP_Weapon();
		virtual ~CP_Weapon() {}

		// �R���|�[�l���g����
		void OnCollisionStay(const CollisionInfo& _otherColInfo);

		/// @brief �U�������Z�b�g����
		/// @param _attackInformation �U�����
		void SetAttackInfo(AttackInformation& _attackInformation);

		/// @brief ����̍U������R���W����
		/// @param _isAttackCollision 
		void SetIsAttackCollision(bool _isAttackCollision);

		/// @brief ���L�҂��Z�b�g����
		/// @param _weaponOwner ���L��
		void SetWeaponOwner(IAttacker& _weaponOwner);

		/// @brief �U���ς݃R���W���������Z�b�g
		void ClearAttackedRb();

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;
	private:
		void Draw() override;

		/// @brief �U���ł��邩�擾
		/// @return �U���ł��邩�H
		bool CanAttack() const;

		/// @brief �U������
		/// @param _damager �U���^����Ώ�
		/// @param _contactPos �Փ˒n�_
		void OnAttack(IDamageable& _damager, const DXSimp::Vector3& _contactPos);

		/// @brief ���ɍU�����������X�g�ɒǉ�
		/// @param _rb �U���ς݂�Rb
		void AddAttackedRb(const CP_RigidBody& _rb);

		/// @brief �U���ł��邩�m�F
		/// @param _targetObject �Ώۂ̃I�u�W�F�N�g
		/// @return �U���ł��邩�H
		bool CheckAttackableTag(GameObject& _targetObject) const;

		/// @brief �U���ł��邩�m�F
		/// @param  _targetRb �Ώۂ̃I�u�W�F�N�g
		/// @return �U���ł��邩�H
		bool CheckAttackedRb(const CP_RigidBody& _targetRb) const;

		// �f�o�b�O�p�U���t���O�������Ă��邩���邽�߂̕`��
		void DebugAttackFlag();

		/// @brief �U���ł���^�O��ǉ�
		/// @param _addTag �^�O
		void AddAttackableTag(Tag::Type _addTag);

		/// @brief �Ō���̍U���ł���^�O���폜
		/// @param _addTag �^�O
		void RemoveBackAttackableTag();

		void ImGuiDebug() override;
	};
}