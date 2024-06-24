#include "pch.h"
#include "CollisionRegister.h"

#include"Collider.h"
#include "CollisionChecker.h"

void CollisionRegister::SetCollisionChecker(CollisionChecker& _pCollisionChecker)
{
	pCollisionChecker = &_pCollisionChecker;
}

void CollisionRegister::AddCollider(Collider& _collider)
{
	pCollisionChecker->AddCollider(_collider);	// コライダーを追加する
}

void CollisionRegister::PopCollider(Collider& _collider)
{
	pCollisionChecker->PopCollider(_collider);	// コライダーをリストから削除する
}
