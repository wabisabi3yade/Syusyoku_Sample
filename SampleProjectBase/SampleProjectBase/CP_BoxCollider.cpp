#include "pch.h"
#include "CP_BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void CP_BoxCollider::CheckCollisionAABB(CP_Collider& _box1, CP_Collider& _box2)
{

}

void CP_BoxCollider::CheckCollisionOBB(CP_Collider& _box1, CP_Collider& _box2)
{
}

CP_BoxCollider& CP_BoxCollider::operator=(const CP_BoxCollider& _other)
{
	if (this == &_other) return *this;
	CP_Collider::operator=(_other);

	// �p�����[�^���
	posOffset = _other.posOffset;
	angleOffset = _other.angleOffset;
	size = _other.size;
	isAABB = _other.isAABB;

	return *this;
}

void CP_BoxCollider::Init()
{
	name = "BoxCollider";

	CP_Collider::Init();	// �ǉ�����������

	size = Vector3::One;
	type = CP_Collider::Type::Box;	// �{�b�N�X�ɐݒ肷��
}

void CP_BoxCollider::Draw()
{
	// �{�b�N�X�\��
	const Transform& t = gameObject->transform;

	// ���[���h���W�n�ŋ��߂�
	Vector3 centerPos_w = t.position + posOffset * t.scale; // �X�P�[���ɑΉ��������I�t�Z�b�g���W�𑫂�
	Geometory::SetPosition(centerPos_w);
	
	Vector3 scale_w = t.scale * size;// �|����
	Geometory::SetScale(scale_w);

	// OBB�͉�]
	Vector3 rotation_w;
	if (!isAABB)
		rotation_w = t.rotation + angleOffset;	// ����
	Geometory::SetRotation(rotation_w);

	// �F
	Geometory::SetColor(CP_Collider::normalColor);
	if (hitColliders.size() > 0)	// �����ɓ������Ă�Ȃ�
	{
		Geometory::SetColor(CP_Collider::hitColor);	// �F��ς���
	}
	Geometory::DrawCube(true);
}

void CP_BoxCollider::ImGuiSetting()
{
	ImGui::Checkbox("AABB", &isAABB);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
	ImGuiMethod::DragFloat3(angleOffset, "angleOffset");
	ImGuiMethod::DragFloat3(size, "size");
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldCenterPos() const
{
	const Transform& t = gameObject->transform;
	return t.position + posOffset * t.scale;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldRotation() const
{
	return gameObject->transform.rotation + angleOffset;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldScale() const
{
	return gameObject->transform.scale * size;
}

bool CP_BoxCollider::CollisionBox(CP_Collider& _box1, CP_Collider& _box2)
{
	// AABB��OBB�ŏ�����ς���



	return false;
}
