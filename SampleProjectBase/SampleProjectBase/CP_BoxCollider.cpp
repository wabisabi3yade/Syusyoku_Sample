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

	// パラメータ代入
	posOffset = _other.posOffset;
	angleOffset = _other.angleOffset;
	size = _other.size;
	isAABB = _other.isAABB;

	return *this;
}

void CP_BoxCollider::Init()
{
	name = "BoxCollider";

	CP_Collider::Init();	// 追加処理をする

	size = Vector3::One;
	type = CP_Collider::Type::Box;	// ボックスに設定する
}

void CP_BoxCollider::Draw()
{
	// ボックス表示
	const Transform& t = gameObject->transform;

	// ワールド座標系で求める
	Vector3 centerPos_w = t.position + posOffset * t.scale; // スケールに対応させたオフセット座標を足す
	Geometory::SetPosition(centerPos_w);
	
	Vector3 scale_w = t.scale * size;// 掛ける
	Geometory::SetScale(scale_w);

	// OBBは回転
	Vector3 rotation_w;
	if (!isAABB)
		rotation_w = t.rotation + angleOffset;	// 足す
	Geometory::SetRotation(rotation_w);

	// 色
	Geometory::SetColor(CP_Collider::normalColor);
	if (hitColliders.size() > 0)	// 何かに当たってるなら
	{
		Geometory::SetColor(CP_Collider::hitColor);	// 色を変える
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
	// AABBとOBBで処理を変える



	return false;
}
