#include "pch.h"
#include "BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void BoxCollider::Init()
{
	name = "BoxCollider";

	Collider::Init();	// �ǉ�����������

	size = Vector3::One;
	type = Collider::Type::Box;	// �{�b�N�X�ɐݒ肷��
}

void BoxCollider::Draw()
{
	// �{�b�N�X�\��
	const Transform& t = gameObject->transform;

	// ���[���h���W�n�ŋ��߂�
	Vector3 centerPos_w = t.position + posOffset * t.scale; // �X�P�[���ɑΉ��������I�t�Z�b�g���W�𑫂�
	Geometory::SetPosition(centerPos_w);
	Vector3 rotation_w = t.rotation + angleOffset;	// ����
	Geometory::SetRotation(rotation_w);
	Vector3 scale_w = t.scale * size;// �|����
	Geometory::SetScale(scale_w);

	// �F
	Geometory::SetColor(Collider::normalColor);
	if (hitColliders.size() > 0)	// �����ɓ������Ă�Ȃ�
	{
		Geometory::SetColor(Collider::hitColor);	// �F��ς���
	}
	Geometory::DrawCube(true);
}

void BoxCollider::SetParameter()
{
	ImGui::Checkbox("AABB", &isAABB);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
	ImGuiMethod::DragFloat3(angleOffset, "angleOffset");
	ImGuiMethod::DragFloat3(size, "size");
}

DirectX::SimpleMath::Vector3 BoxCollider::GetCenterPos() const
{
	const Transform& t = gameObject->transform;
	return t.position + posOffset * t.scale;
}

DirectX::SimpleMath::Vector3 BoxCollider::GetRotation() const
{
	return gameObject->transform.rotation + angleOffset;
}

DirectX::SimpleMath::Vector3 BoxCollider::GetScale() const
{
	return gameObject->transform.scale * size;
}
