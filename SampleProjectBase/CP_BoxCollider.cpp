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
	Vector3 worldLength;
	CalcWorldLength(worldLength);

	btVector3 btLength;
	btLength.setValue(worldLength.x, worldLength.y, worldLength.z);
	pCollisionShape = std::make_unique<btBoxShape>(btLength * 0.5);
}

void CP_BoxCollider::LateUpdate()
{
	CP_Collider::LateUpdate();
}

void CP_BoxCollider::ImGuiSetting()
{
	CP_Collider::ImGuiSetting();

	ImGui::Checkbox("AABB", &isAABB);

	Vector3 changeFloat = length;
	if (ImGuiMethod::DragFloat3(changeFloat, "length", 0.01f))
		SetLength(changeFloat);
}

void CP_BoxCollider::SetLength(const DirectX::SimpleMath::Vector3& _length)
{
	length = _length;

	RecreateShape();
}

const DirectX::SimpleMath::Vector3& CP_BoxCollider::GetLength() const
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
	if(HashiTaku::LoadJsonVector3("length", loadLength, _data))
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

void CP_BoxCollider::CalcWorldLength(DirectX::SimpleMath::Vector3& _out)
{
	_out = GetTransform().GetScale() * length;
}
