#include "pch.h"
#include "CP_CylinderCollider.h"

namespace HashiTaku
{
	CP_CylinderCollider::CP_CylinderCollider() :
		CP_Collider(ShapeType::Cyllinder), radius(1.0f), height(1.0f)
	{
	}

	void CP_CylinderCollider::SetRadius(float _setRadius)
	{
		radius = std::max(_setRadius, 0.0f);
		RecreateShape();
	}

	void CP_CylinderCollider::SetHeight(float _setHeight)
	{
		height = std::max(_setHeight, 0.0f);
		RecreateShape();
	}

	json CP_CylinderCollider::Save()
	{
		auto data = CP_Collider::Save();

		data["radius"] = radius;
		data["height"] = height;

		return data;
	}

	void CP_CylinderCollider::Load(const json& _data)
	{
		CP_Collider::Load(_data);

		if (LoadJsonFloat("radius", radius, _data) &&
			LoadJsonFloat("height", height, _data))
		{
			RecreateShape();
		}
	}

	void CP_CylinderCollider::CreateShape()
	{
		using namespace Bullet;

		// オブジェクトのスケール値を反映
		float worldRadius = 1.0f;
		float worldHeight = 1.0f;
		ApplyObjectScale(worldRadius, worldHeight);

		// 作成
		btVector3 cylinderParam(worldRadius, worldHeight / 2.0f, worldRadius / 2.0f);
		pCollisionShape = std::make_unique<btCylinderShape>(cylinderParam);
	}

	void CP_CylinderCollider::ApplyObjectScale(float& _outRadius, float& _outHeight)
	{
		// 半径はXZの最大の大きさ、高さはY
		Transform& transform = GetTransform();
		const DXSimp::Vector3& scale = transform.GetScale();

		float maxVal = scale.x;
		maxVal = std::max(maxVal, scale.z);

		_outRadius = radius * maxVal;
		_outHeight = height * scale.y;
	}

	void CP_CylinderCollider::ImGuiDebug()
	{
		CP_Collider::ImGuiDebug();

		if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.0f, 1000.0f))
			RecreateShape();

		if (ImGui::DragFloat("Height", &height, 0.01f, 0.0f, 1000.0f))
			RecreateShape();
	}
}