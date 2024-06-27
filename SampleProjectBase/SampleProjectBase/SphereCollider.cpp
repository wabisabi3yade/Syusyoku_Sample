#include "pch.h"
#include "SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

constexpr float VERTEX_RADIUS(0.5f);	// ���_�쐬���̔��a

using namespace DirectX::SimpleMath;

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Init()
{
	name = "SphereCollider";

	Collider::Init();	// �ǉ���������

	radius = VERTEX_RADIUS;	// ���a��������
	type = Type::Sphere;	// ���Ɛݒ肷��
}

void SphereCollider::Draw()
{
	//// �����蔻��`��
	//const Transform& t = gameObject->transform;
	//Vector3 centerPos = t.position + posOffset * t.scale;	// ���S���W
	//Geometory::SetPosition(centerPos);
	//// �傫�������߂�
	//float scale = radius / VERTEX_RADIUS;
	//Geometory::SetScale(Vector3::One * scale);

	//// �������Ă邩�ŐF�ς���
	//Geometory::SetColor(normalColor);
	//if (hitColliders.size() > 0)
	//	Geometory::SetColor(hitColor);

	//Geometory::DrawSphere(true);
}

void SphereCollider::SetParameter()
{
	ImGui::DragFloat("radius", &radius);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
}

DirectX::SimpleMath::Vector3 SphereCollider::GetCenterPos() const
{
	const Transform& t = gameObject->transform;
	return t.position + posOffset * t.scale;
}

bool SphereCollider::CollisionSphere(Collider& _sphere1, Collider& _sphere2)
{
	// ����ȑO�Ɏ�ނ��`�F�b�N�����S�Ȃ̂Ŏ��s���x�d����statoc_cast�ϊ�����
	SphereCollider& s1 = static_cast<SphereCollider&>(_sphere1);
	SphereCollider& s2 = static_cast<SphereCollider&>(_sphere2);

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