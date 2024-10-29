#include "pch.h"
#include "CP_Weapon.h"
#include "GameObject.h"

CP_Weapon::CP_Weapon() : isAttackCollision(true)
{
}

void CP_Weapon::OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo)
{
	// �U���t���O�����Ă��Ȃ��Ȃ�
	if (!isAttackCollision) return;

	// �_���[�W���󂯂�C���^�[�t�F�[�X�����邩�m�F
	GameObject& gameObject = _otherColInfo.pRigidBodyCp->GetGameObject();
	HashiTaku::IDamageable* pDamager = gameObject.GetComponent<HashiTaku::IDamageable>();
	if (!pDamager) return;

	OnAttack(*pDamager);
}

void CP_Weapon::SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation)
{
	atkInfomation = _attackInformation;
}

void CP_Weapon::SetIsAttackCollision(bool _isAttackCollision)
{
	isAttackCollision = _isAttackCollision;
}

void CP_Weapon::OnAttack(HashiTaku::IDamageable& _damager)
{
	// �_���[�W��^����
	_damager.OnDamage(atkInfomation);
}
