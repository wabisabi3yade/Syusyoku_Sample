#include "pch.h"
#include "Bullet.h"
#include "BulletMove.h"
#include "CP_SphereCollider.h"

Bullet::Bullet()
{
	name = "Bullet";

	// �R���|�[�l���g�A�^�b�`
	AddComponent<BulletMove>();	// �e�ړ�
	AddComponent<CP_SphereCollider>();	// �������蔻��
}

Bullet::~Bullet()
{
}
