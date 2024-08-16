#include "pch.h"
#include "CP_SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

constexpr float VERTEX_RADIUS(0.5f);	// ���_�쐬���̔��a

using namespace DirectX::SimpleMath;

CP_SphereCollider& CP_SphereCollider::operator=(const CP_SphereCollider& _other)
{
	if (this == &_other) return *this;
	CP_Collider::operator=(_other);

	radius = _other.radius;
	posOffset = _other.posOffset;

	return *this;
}

void CP_SphereCollider::Init()
{
	name = "SphereCollider";

	CP_Collider::Init();	// �ǉ���������

	radius = VERTEX_RADIUS;	// ���a��������
	type = Type::Sphere;	// ���Ɛݒ肷��
}

void CP_SphereCollider::Draw()
{
	// �����蔻��`��
	const Transform& t = gameObject->transform;
	Vector3 centerPos = t.GetPosition() + posOffset;	// ���S���W
	Geometory::SetPosition(centerPos);

	// �傫�������߂�
	float scale = radius / VERTEX_RADIUS;
	Geometory::SetScale(Vector3::One * scale);

	// �������Ă邩�ŐF�ς���
	Geometory::SetColor(normalColor);
	if (hitColliders.size() > 0)
		Geometory::SetColor(hitColor);
	Geometory::DrawSphere(true);
}

void CP_SphereCollider::ImGuiSetting()
{
	ImGui::DragFloat("radius", &radius);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
}

DirectX::SimpleMath::Vector3 CP_SphereCollider::GetCenterPos() const
{
	return GetTransform().GetPosition() + posOffset;
}

bool CP_SphereCollider::CollisionSphere(CP_Collider& _sphere1, CP_Collider& _sphere2)
{
	// ����ȑO�Ɏ�ނ��`�F�b�N�����S�Ȃ̂Ŏ��s���x�d����statoc_cast�ϊ�����
	CP_SphereCollider& s1 = static_cast<CP_SphereCollider&>(_sphere1);
	CP_SphereCollider& s2 = static_cast<CP_SphereCollider&>(_sphere2);

	// ���̒��S���W���m�̒��������߂�
	Vector3 vec = s1.GetCenterPos() - s2.GetCenterPos();
	float length = vec.Length();

	// ������������2�̋��̔��a�ȉ��Ȃ瓖�����Ă���Ɣ��肷��
	if (length <= s1.GetRadius() + s2.GetRadius())
	{
		s1.SetHitCollider(s2);
		s2.SetHitCollider(s1);
		return true;
	}

	return false;
}

bool CP_SphereCollider::CollisionBox(CP_Collider& _sphere, CP_Collider& _box)
{
	return false;
}
