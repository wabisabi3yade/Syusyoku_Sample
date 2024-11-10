#include "pch.h"
#include "CP_CapsuleCollider.h"

CP_CapsuleCollider::CP_CapsuleCollider()
	: CP_Collider(ShapeType::Capsule), radius(0.5f), height(1.0f)
{
}

void CP_CapsuleCollider::SetRadius(float _setRadius)
{
	radius = _setRadius;

	RecreateShape();
}

void CP_CapsuleCollider::SetHeight(float _setHeight)
{
	height = _setHeight;

	RecreateShape();
}

nlohmann::json CP_CapsuleCollider::Save()
{
	auto data = CP_Collider::Save();

	data["radius"] = radius;
	data["height"] = height;

	return data;
}

void CP_CapsuleCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);
	
	if (HashiTaku::LoadJsonFloat("radius", radius, _data) &&
		HashiTaku::LoadJsonFloat("height", height, _data))
	{
		RecreateShape();
	}
}

void CP_CapsuleCollider::CreateShape()
{
	using namespace HashiTaku::Bullet;

	// オブジェクトのスケール値を反映
	float worldRadius = 1.0f;
	float worldHeight = 1.0f;
	ApplyObjectScale(worldRadius, worldHeight);

	// 作成
	pCollisionShape = std::make_unique<btCapsuleShape>(
		ToBtScalar(worldRadius), 
		ToBtScalar(worldHeight)
	);
}

void CP_CapsuleCollider::ApplyObjectScale(float& _outRadius, float& _outHeight)
{
	// 半径はXZの最大の大きさ、高さはY
	Transform& transform = GetTransform();
	const DirectX::SimpleMath::Vector3& scale = transform.GetScale();
	float maxVal = scale.x;
	maxVal = std::max(maxVal, scale.z);

	_outRadius = radius * maxVal;
	_outHeight = height * scale.y;
}

void CP_CapsuleCollider::ImGuiDebug()
{
	CP_Collider::ImGuiDebug();

	if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.0f, 1000.0f))
		RecreateShape();

	if (ImGui::DragFloat("Height", &height, 0.01f, 0.0f, 1000.0f))
		RecreateShape();
}
