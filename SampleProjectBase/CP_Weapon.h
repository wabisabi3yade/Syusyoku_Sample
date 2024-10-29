#pragma once
#include "Component.h"
#include "AttackInformation.h"
#include "IDamageable.h"

/// @brief ����R���|�[�l���g
class CP_Weapon : public Component
{
	/// @brief �U�����
	HashiTaku::AttackInformation atkInfomation;

	/// @brief �U������R���W����
	bool isAttackCollision;
public:
	CP_Weapon();
	~CP_Weapon() {}

	// �R���|�[�l���g����
	void OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo);

	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation);

	/// @brief ����̍U������R���W����
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);

private:
	/// @brief �U������
	/// @param _damager �U���^����Ώ�
	void OnAttack(HashiTaku::IDamageable& _damager);
};



