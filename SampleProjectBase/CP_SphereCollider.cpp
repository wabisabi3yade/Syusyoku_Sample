#include "pch.h"
#include "CP_SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

namespace HashiTaku
{
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

	json CP_SphereCollider::Save()
	{
		json data = CP_Collider::Save();

		data["radius"] = radius;
		return data;
	}

	void CP_SphereCollider::Load(const json& _data)
	{
		CP_Collider::Load(_data);

		if (LoadJsonFloat("radius", radius, _data))
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
		DXSimp::Vector3 scale = transform.GetScale();
		float maxVal = scale.x;
		maxVal = std::max(maxVal, scale.y);
		maxVal = std::max(maxVal, scale.z);

		_outRadius = radius * maxVal;
	}
}