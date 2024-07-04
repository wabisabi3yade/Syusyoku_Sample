#include "pch.h"
#include "CollisionRegister.h"

#include"CP_Collider.h"
#include "CollisionChecker.h"

void CollisionRegister::SetCollisionChecker(CollisionChecker& _pCollisionChecker)
{
	pCollisionChecker = &_pCollisionChecker;
}

void CollisionRegister::AddCollider(CP_Collider& _collider)
{
	pCollisionChecker->AddCollider(_collider);	// �R���C�_�[��ǉ�����
}

void CollisionRegister::PopCollider(CP_Collider& _collider)
{
	pCollisionChecker->PopCollider(_collider);	// �R���C�_�[�����X�g����폜����
}
