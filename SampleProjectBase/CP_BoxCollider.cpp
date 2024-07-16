#include "pch.h"
#include "CP_BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

// コンポーネントインクルードヘッダー
#include "ComponentDefine.h"

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

	// モデルのサイズから判定の大きさを決める
	SizeFromModelSize();
}

void CP_BoxCollider::Draw()
{
	// ボックス表示
	const Transform& t = gameObject->transform;

	// オブジェクトのスケールも考慮する
	Vector3 pos_w = t.position + posOffset * t.scale;
	Geometory::SetPosition(pos_w);

	Vector3 scale_w = t.scale * size;
	Geometory::SetScale(scale_w);

	Vector3 rotation_w;

	if (!isAABB)
		rotation_w = t.rotation + angleOffset;

	Geometory::SetRotation(rotation_w);

	// 色
	Geometory::SetColor(CP_Collider::normalColor);

	// 何かに当たってるなら
	if (hitColliders.size() > 0)	
		Geometory::SetColor(CP_Collider::hitColor);		// 色を変える

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
	return GetTransform().position + posOffset * GetTransform().scale;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldRotation() const
{
	return GetTransform().rotation + angleOffset;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldScale() const
{
	return GetTransform().scale * size;
}

bool CP_BoxCollider::CollisionBox(CP_Collider& _box1, CP_Collider& _box2)
{
	// AABBとOBBで処理を変える



	return false;
}

void CP_BoxCollider::SizeFromModelSize()
{
	// メッシュレンダラーを取得
	CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();

	if (pMeshRenderer == nullptr) return;

	const Mesh_Base* pModel = pMeshRenderer->GetRenderMesh();

	if (pModel == nullptr) return;

	// サイズ取得
	Vector3 modelSize = pModel->GetSize();

	// サイズを取得していなかったら終わる
	if (modelSize.x == 0.0f) return;

	size = modelSize;
	posOffset = pModel->GetCenterPosition();
}
