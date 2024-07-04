#pragma once
#include "Singleton_Base.h"

class CP_Collider;
class CollisionChecker;

// �R���C�_�[���V�[���̓����蔻��`�F�b�J�[�ɒǉ�����N���X
// �R���|�[�l���g������ǉ������������ׁA�V���O���g���N���X
class CollisionRegister : public Singleton_Base<CollisionRegister>
{
	friend class Singleton_Base<CollisionRegister>;

	CollisionChecker* pCollisionChecker;	// �R���C�_�[�̓o�^�Ώۂ̓����蔻��`�F�b�J�[

	CollisionRegister() : pCollisionChecker(nullptr) {};
	~CollisionRegister() {};
public:
	// �����蔻��`�F�b�J�[�N���X���Z�b�g
	void SetCollisionChecker(CollisionChecker& _pCollisionChecker);	
	// �����蔻��`�F�b�J�[�N���X�ɒǉ�����
	void AddCollider(CP_Collider& _collider);
	// �����蔻��`�F�b�J�[�N���X
	void PopCollider(CP_Collider& _collider);
};

