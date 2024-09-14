#include "pch.h"
#include "CP_BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

// コンポーネントインクルードヘッダー
#include "ComponentDefine.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

CP_BoxCollider::CP_BoxCollider()
	: CP_Collider(CP_Collider::Type::Box), length(Vector3::One)
{
}

void CP_BoxCollider::Init()
{
	CP_Collider::Init();
}

void CP_BoxCollider::CreateShape()
{
	// ボックス形状作成
	pCollisionShape = std::make_unique<btBoxShape>(Bullet::ToBtVector3(length * 0.5f));
}

void CP_BoxCollider::LateUpdate()
{
	CP_Collider::LateUpdate();
	LengthUpdate();
}

void CP_BoxCollider::ImGuiSetting()
{
	CP_Collider::ImGuiSetting();

	ImGui::Checkbox("AABB", &isAABB);

	Vector3 p_Length = length;
	ImGuiMethod::DragFloat3(length, "length", 0.1f);

	if (length != p_Length)
		SetLength(length);
}

void CP_BoxCollider::SetLength(const DirectX::SimpleMath::Vector3& _length)
{
	length = _length;
	SetShape();
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetLength() const
{
	return length;
}

nlohmann::json CP_BoxCollider::Save()
{
	auto data = CP_Collider::Save();

	HashiTaku::SaveJsonVector3("length", length, data);
	data["AABB"] = isAABB;

	return data;
}

void CP_BoxCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);
	
	Vector3 loadLength;
	HashiTaku::LoadJsonVector3("length", loadLength, _data);
	SetLength(loadLength);

	HashiTaku::LoadJsonBoolean("AABB", isAABB, _data);
}

void CP_BoxCollider::SizeFromModelSize()
{
	// メッシュレンダラーを取得
	CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();

	if (pMeshRenderer == nullptr) return;

	const Mesh_Group* pModel = pMeshRenderer->GetRenderMesh();

	if (pModel == nullptr) return;

	// サイズ取得
	Vector3 modelSize = pModel->GetSize();

	// サイズを取得していなかったら終わる
	if (modelSize.x <= Mathf::epsilon) return;

	length = modelSize;
	centerOffset = pModel->GetCenterPosition();
}

void CP_BoxCollider::LengthUpdate()
{
}
