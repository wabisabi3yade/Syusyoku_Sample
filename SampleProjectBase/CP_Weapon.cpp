#include "pch.h"
#include "CP_Weapon.h"
#include "GameObject.h"

CP_Weapon::CP_Weapon() : isAttackCollision(true)
{
}

void CP_Weapon::OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo)
{
	// 攻撃フラグがついていないなら
	if (!isAttackCollision) return;

	// ダメージを受けるインターフェースがあるか確認
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
	// ダメージを与える
	_damager.OnDamage(atkInfomation);
}
