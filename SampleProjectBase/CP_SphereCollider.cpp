#include "pch.h"
#include "CP_SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace HashiTaku;
using namespace DirectX::SimpleMath;

void CP_SphereCollider::Draw()
{
}

void CP_SphereCollider::ImGuiSetting()
{
	CP_Collider::ImGuiSetting();

	float changeRadius = radius;
	if (ImGui::DragFloat("radius", &changeRadius, 0.01f, 0.0f, 1000.0f))
		SetRadius(changeRadius);
}

DirectX::SimpleMath::Vector3 CP_SphereCollider::GetCenterPos() const
{
	return GetTransform().GetPosition() + posOffset;
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

	HashiTaku::LoadJsonFloat("radius", radius, _data);
	SetRadius(radius);
}

void CP_SphereCollider::CreateShape()
{
	pCollisionShape = std::make_unique<btSphereShape>(Bullet::ToBtScalar(radius));
}

void CP_SphereCollider::SetRadius(float _radius)
{
	radius = _radius;
	SetShape();
}
