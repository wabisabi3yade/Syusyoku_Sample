#pragma once
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

// �V�[�����̂����蔻�蓯�m�̔�������N���X
class CollisionChecker
{
	std::vector<Collider*> colliders;	// �V�[�����̑S�Ă̂����蔻��
	
	// �֐��|�C���^���g�p���邽�߂�static�֐�
	// 1�ڂ������������̃R���W����
	static bool SphereCollision(Collider& _sphere, Collider& _other);
	
public:
	CollisionChecker(){};
	~CollisionChecker();

	void CollisionCheck();	// ���t���[�������蔻�蓯�m�̊m�F����
	
	void AddCollider(Collider& _collider);	// �R���C�_�[��ǉ�
	void PopCollider(Collider& _collider);	// �R���C�_�[���폜����	
};

