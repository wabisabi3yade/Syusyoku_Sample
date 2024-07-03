#include "pch.h"
#include "CollisionChecker.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

bool CollisionChecker::SphereCollision(Collider& _sphere, Collider& _other)
{
	bool isHit = false;
	
	switch (_other.GetType())	// ������̓����蔻��̎�ނ��擾����
	{
	case Collider::Type::Box:
		SphereCollider::CollisionBox(_sphere, _other);	// ���ƃ{�b�N�X�̔���
		break;

	case Collider::Type::Sphere:
		isHit = SphereCollider::CollisionSphere(_sphere, _other);	// �����m�̔���
		break;
	}

	return isHit;
}
bool CollisionChecker::BoxCollision(Collider& _box, Collider& _other)
{
	bool isHit = false;

	switch (_other.GetType())	// ������̓����蔻��̎�ނ��擾����
	{
	case Collider::Type::Box:
		isHit = BoxCollider::CollisionBox(_box, _other);	// �{�b�N�X���m�̔���
		break;

	case Collider::Type::Sphere:
		isHit = SphereCollider::CollisionBox(_other, _box);	// ���ƃ{�b�N�X�̔���
		break;
	}

	return isHit;
}
CollisionChecker::~CollisionChecker()
{
	colliders.clear();
}

void CollisionChecker::CollisionCheck()
{
	if (colliders.size() == 0) return;

	// �����蔻��������̂Ǝ��Ȃ����ŕ�����
	std::vector<Collider*> checkColliders;	// �Ƃ�z��
	for (auto col : colliders)
	{
		col->ResetColliders();

		// ������
		if (col->GetGameObject().GetIsActive() == false) continue;	// �A�N�e�B�u��ԂłȂ��Ȃ�

		checkColliders.push_back(col);	// �`�F�b�N�z��ɒǉ�
	}

	// ������Ă���
	for (u_int i = 0; i < checkColliders.size() - 1; i++)
	{
		// ��ڂ̓����蔻��擾
		Collider& col1 = *checkColliders[i];
		
		// �^�C�v�œ����蔻��̊֐������߂�
		bool (*pFunc)(Collider&, Collider&);	// �֐��|�C���^
		switch (checkColliders[i]->GetType())
		{
		// �{�b�N�X
		case Collider::Type::Box: pFunc = &BoxCollision; break;

		// ��
		case Collider::Type::Sphere: pFunc = &SphereCollision; break;

		default: break;
		}

		// 1�����߂Ă���
		for (u_int j = i + 1; j < checkColliders.size(); j++)
		{
			Collider& col2 = *checkColliders[j];

			pFunc(col1, col2);	// �����蔻������
		}
	}
}

void CollisionChecker::AddCollider(Collider& _collider)
{
	// ����������邩�m�F����
	auto itr = std::find(colliders.begin(), colliders.end(), &_collider);
	if (itr != colliders.end())
	{
		ImGuiDebugLog::Add("�����R���C�_�[�͒ǉ��ł��܂���");
		return;
	}

	colliders.push_back(&_collider);

}

void CollisionChecker::PopCollider(Collider& _collider)
{
	// ����������邩�m�F����
	auto itr = std::find(colliders.begin(), colliders.end(), &_collider);
	if (itr == colliders.end())
	{
		ImGuiDebugLog::Add("�폜����R���C�_�[������܂���");
		return;
	}

	colliders.erase(itr);	// �폜����
}

