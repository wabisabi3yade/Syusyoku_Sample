#include "pch.h"
#include "BulletMove.h"
#include "GameObject.h"

void BulletMove::Update()
{
	// �ړ�����
	GetTransform().position += moveVector * moveSpeed * MainApplication::DeltaTime();

	// ���Ԃ��o�߂�����
	elapsedTime += MainApplication::DeltaTime();
	if (elapsedTime > destroyTime)	// �폜�̎��Ԃ��ɂȂ�����
		gameObject->Destroy();	// �������폜����
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