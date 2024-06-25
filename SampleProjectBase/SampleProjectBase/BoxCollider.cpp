#include "pch.h"
#include "BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void BoxCollider::Init()
{
	name = "BoxCollider";

	Collider::Init();	// 追加処理をする

	size = Vector3::One;
	type = Collider::Type::Box;	// ボックスに設定する
}

void BoxCollider::Draw()
{
	// ボックス表示
	const Transform& t = gameObject->transform;

	// ワールド座標系で求める
	Vector3 centerPos_w = t.position + posOffset * t.scale; // スケールに対応させたオフセット座標を足す
	Geometory::SetPosition(centerPos_w);
	Vector3 rotation_w = t.rotation + angleOffset;	// 足す
	Geometory::SetRotation(rotation_w);
	Vector3 scale_w = t.scale * size;// 掛ける
	Geometory::SetScale(scale_w);

	// 色
	Geometory::SetColor(Collider::normalColor);
	if (hitColliders.size() > 0)	// 何かに当たってるなら
	{
		Geometory::SetColor(Collider::hitColor);	// 色を変える
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
