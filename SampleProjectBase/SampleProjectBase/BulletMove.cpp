#include "pch.h"
#include "BulletMove.h"
#include "GameObject.h"

void BulletMove::Update()
{
	// ˆÚ“®‚·‚é
	GetTransform().position += moveVector * moveSpeed * MainApplication::DeltaTime();

	// ŠÔ‚ğŒo‰ß‚³‚¹‚é
	elapsedTime += MainApplication::DeltaTime();
	if (elapsedTime > destroyTime)	// íœ‚ÌŠÔ‚ğ‚É‚È‚Á‚½‚ç
		gameObject->Destroy();	// ©•ª‚ğíœ‚·‚é
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