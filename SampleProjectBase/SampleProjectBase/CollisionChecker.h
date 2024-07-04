#pragma once
#include "CP_Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

// �V�[�����̂����蔻�蓯�m�̔�������N���X
class CollisionChecker
{
	std::vector<CP_Collider*> colliders;	// �V�[�����̑S�Ă̂����蔻��
	
	// �֐��|�C���^���g�p���邽�߂�static�֐�
	// 1�ڂ������������̃R���W����
	static bool SphereCollision(CP_Collider& _sphere, CP_Collider& _other);
	
	static bool BoxCollision(CP_Collider& _box, CP_Collider& _other);
public:
	CollisionChecker(){};
	~CollisionChecker();

	void CollisionCheck();	// ���t���[�������蔻�蓯�m�̊m�F����
	
	void AddCollider(CP_Collider& _collider);	// �R���C�_�[��ǉ�
	void PopCollider(CP_Collider& _collider);	// �R���C�_�[���폜����	
};

