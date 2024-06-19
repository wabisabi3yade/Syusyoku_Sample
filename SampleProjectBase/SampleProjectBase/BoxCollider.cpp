#include "pch.h"
#include "BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void BoxCollider::Init()
{
	size = Vector3::One;
}

void BoxCollider::Update()
{
}

void BoxCollider::Draw()
{
	// �{�b�N�X�\��
	const Transform& t = gameObject->transform;
	Geometory::SetPosition(t.position + offset);	// ����
	Geometory::SetRotation(t.rotation + angle);	// ����
	Geometory::SetScale(t.scale * size);// �|����

	// �F
	Geometory::SetColor(Color(0,1,0,1));
	if(isHit)
	Geometory::SetColor(Color(1, 1, 0, 1));

	//BaseObject::SetColor(normalColor);
	//if (isHit)	// �������Ă�Ȃ�
	//{
	//	BaseObject::SetColor(hitColor);	// �F��ς���
	//}
	Geometory::DrawCube(true);
}

void BoxCollider::SetParameter()
{
	if(ImGui::TreeNode("BoxCollider"))
	{
		ImGuiMethod::DragFloat3(offset, "Offset");
		ImGuiMethod::DragFloat3(angle, "Angle");
		ImGuiMethod::DragFloat3(size, "Size");
		ImGui::TreePop();
	}
}
