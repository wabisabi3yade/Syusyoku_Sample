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
	// ボックス表示
	const Transform& t = gameObject->transform;
	Geometory::SetPosition(t.position + offset);	// 足す
	Geometory::SetRotation(t.rotation + angle);	// 足す
	Geometory::SetScale(t.scale * size);// 掛ける

	// 色
	Geometory::SetColor(Color(0,1,0,1));
	if(isHit)
	Geometory::SetColor(Color(1, 1, 0, 1));

	//BaseObject::SetColor(normalColor);
	//if (isHit)	// 当たってるなら
	//{
	//	BaseObject::SetColor(hitColor);	// 色を変える
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
