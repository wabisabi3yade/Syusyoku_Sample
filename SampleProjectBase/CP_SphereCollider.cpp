#include "pch.h"
#include "CP_SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace HashiTaku;
using namespace DirectX::SimpleMath;

void CP_SphereCollider::Draw()
{
}

void CP_SphereCollider::ImGuiDebug()
{
	CP_Collider::ImGuiDebug();

	float changeRadius = radius;
	if (ImGui::DragFloat("radius", &changeRadius, 0.01f, 0.0f, 1000.0f))
		SetRadius(changeRadius);
}

float CP_SphereCollider::GetRadius() const
{
	return radius;
}

nlohmann::json CP_SphereCollider::Save()
{
	nlohmann::json data = CP_Collider::Save();

	data["radius"] = radius;
	return data;
}

void CP_SphereCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);

	if (HashiTaku::LoadJsonFloat("radius", radius, _data))
		SetRadius(radius);
}

void CP_SphereCollider::CreateShape()
{
	float worldRadius = 1.0f;
	ApplyObjectScale(worldRadius);
	pCollisionShape = std::make_unique<btSphereShape>(Bullet::ToBtScalar(worldRadius));
}

void CP_SphereCollider::SetRadius(float _radius)
{
	radius = _radius;
	RecreateShape();
}

void CP_SphereCollider::ApplyObjectScale(float& _outRadius)
{
	// スケール値で3軸の中の一番大きい値を掛ける
	Transform& transform = GetTransform();
	Vector3 scale = transform.GetScale();
	float maxVal = scale.x;
	maxVal = std::max(maxVal, scale.y);
	maxVal = std::max(maxVal, scale.z);

	_outRadius = radius * maxVal;
}
