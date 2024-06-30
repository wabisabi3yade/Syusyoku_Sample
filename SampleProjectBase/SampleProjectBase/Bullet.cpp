#include "pch.h"
#include "Bullet.h"
#include "BulletMove.h"
#include "SphereCollider.h"

Bullet::Bullet()
{
	name = "Bullet";

	// �R���|�[�l���g�A�^�b�`
	AddComponent<BulletMove>();	// �e�ړ�
	AddComponent<SphereCollider>();	// �������蔻��
}

Bullet::~Bullet()
{
}
