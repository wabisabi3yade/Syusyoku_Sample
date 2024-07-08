#include "pch.h"
#include "CollisionChecker.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

bool CollisionChecker::SphereCollision(CP_Collider& _sphere, CP_Collider& _other)
{
	bool isHit = false;
	
	switch (_other.GetType())	// ������̓����蔻��̎�ނ��擾����
	{
	case CP_Collider::Type::Box:
		CP_SphereCollider::CollisionBox(_sphere, _other);	// ���ƃ{�b�N�X�̔���
		break;

	case CP_Collider::Type::Sphere:
		isHit = CP_SphereCollider::CollisionSphere(_sphere, _other);	// �����m�̔���
		break;
	}

	return isHit;
}
bool CollisionChecker::BoxCollision(CP_Collider& _box, CP_Collider& _other)
{
	bool isHit = false;

	switch (_other.GetType())	// ������̓����蔻��̎�ނ��擾����
	{
	case CP_Collider::Type::Box:
		isHit = CP_BoxCollider::CollisionBox(_box, _other);	// �{�b�N�X���m�̔���
		break;

	case CP_Collider::Type::Sphere:
		isHit = CP_SphereCollider::CollisionBox(_other, _box);	// ���ƃ{�b�N�X�̔���
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
	std::vector<CP_Collider*> checkColliders;	// �Ƃ�z��
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
		CP_Collider& col1 = *checkColliders[i];
		
		// �^�C�v�œ����蔻��̊֐������߂�
		bool (*pFunc)(CP_Collider&, CP_Collider&);	// �֐��|�C���^
		switch (checkColliders[i]->GetType())
		{
		// �{�b�N�X
		case CP_Collider::Type::Box: pFunc = &BoxCollision; break;

		// ��
		case CP_Collider::Type::Sphere: pFunc = &SphereCollision; break;

		default: break;
		}

		// 1�����߂Ă���
		for (u_int j = i + 1; j < checkColliders.size(); j++)
		{
			CP_Collider& col2 = *checkColliders[j];

			assert(pFunc == nullptr);

			pFunc(col1, col2);	// �����蔻������
		}
	}
}

void CollisionChecker::AddCollider(CP_Collider& _collider)
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

void CollisionChecker::PopCollider(CP_Collider& _collider)
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

