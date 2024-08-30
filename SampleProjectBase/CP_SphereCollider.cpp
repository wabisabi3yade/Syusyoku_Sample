#include "pch.h"
#include "CP_SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void CP_SphereCollider::Draw()
{
}

void CP_SphereCollider::ImGuiSetting()
{
	ImGui::DragFloat("radius", &radius);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
}

DirectX::SimpleMath::Vector3 CP_SphereCollider::GetCenterPos() const
{
	return GetTransform().GetPosition() + posOffset;
}

nlohmann::json CP_SphereCollider::Save()
{
	nlohmann::json data = CP_Collider::Save();

	data["radius"] = radius;
	HashiTaku::SaveJsonVector3("posOffset", posOffset, data);

	return data;
}

void CP_SphereCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);

	HashiTaku::LoadJsonFloat("radius", radius, _data);
	HashiTaku::LoadJsonVector3("posOffset", posOffset, _data);
}
