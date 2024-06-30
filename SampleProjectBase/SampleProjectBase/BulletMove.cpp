#include "pch.h"
#include "BulletMove.h"
#include "GameObject.h"

void BulletMove::Update()
{
	// 移動する
	GetTransform().position += moveVector * moveSpeed * MainApplication::DeltaTime();

	// 時間を経過させる
	elapsedTime += MainApplication::DeltaTime();
	if (elapsedTime > destroyTime)	// 削除の時間をになったら
		gameObject->Destroy();	// 自分を削除する
}

BulletMove& BulletMove::operator=(const BulletMove& _other)
{
	if (this == &_other) return *this;

	return *this;
}

void BulletMove::SetMoveParameter(float _speed, DirectX::SimpleMath::Vector3 _vector,
	float _destroyTime)
{
	moveSpeed = _speed;
	moveVector = _vector;
	destroyTime = _destroyTime;
}