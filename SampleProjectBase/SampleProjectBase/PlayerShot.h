#pragma once
#include "Component.h"
#include "Bullet.h"

// ��Ԃ̒e���˃N���X
class PlayerShot : public Component
{
	// �e�̈ړ����x
	float shotSpeed;	

	// �e�̔�΂�����
	DirectX::SimpleMath::Vector3 shotVec;		

	// �e�v���n�u
	std::unique_ptr<Bullet> bullet;

	// ���S����̔��ˏꏊ����
	float shotDistance = 0.0f;

public:
	using Component::Component;

	void Init() override;
	void Shot();	// �e�𔭎˂���
};

