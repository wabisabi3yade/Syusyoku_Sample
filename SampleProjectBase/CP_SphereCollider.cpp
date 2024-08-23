#include "pch.h"
#include "CP_SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void CP_SphereCollider::Start()
{
	CP_Collider::Start();	// 追加処理する
}

void CP_SphereCollider::Draw()
{
	// あたり判定描画
	const Transform& t = gameObject->transform;
	Vector3 centerPos = t.GetPosition() + center;	// 中心座標
	Geometory::SetPosition(centerPos);

	// 大きさを求める
	float scale = radius * 2;
	Geometory::SetScale(Vector3::One * scale);

	// 当たってるかで色変える
	Geometory::SetColor(normalColor);
	if (hitColliders.size() > 0)
		Geometory::SetColor(hitColor);
	Geometory::DrawSphere(true);
}

void CP_SphereCollider::ImGuiSetting()
{
	ImGui::DragFloat("radius", &radius);
	ImGuiMethod::DragFloat3(center, "posOffset");
}

DirectX::SimpleMath::Vector3 CP_SphereCollider::GetCenterPos() const
{
	return GetTransform().GetPosition() + center;
}

bool CP_SphereCollider::CollisionSphere(CP_Collider& _sphere1, CP_Collider& _sphere2)
{
	// これ以前に種類をチェックし安全なので実行速度重視でstatoc_cast変換する
	CP_SphereCollider& s1 = static_cast<CP_SphereCollider&>(_sphere1);
	CP_SphereCollider& s2 = static_cast<CP_SphereCollider&>(_sphere2);

	// 球の中心座標同士の長さを求める
	Vector3 vec = s1.GetCenterPos() - s2.GetCenterPos();
	float length = vec.Length();

	// 長さが足した2つの球の半径以下なら当たっていると判定する
	if (length <= s1.GetRadius() + s2.GetRadius())
	{
		s1.SetHitCollider(s2);
		s2.SetHitCollider(s1);
		return true;
	}

	return false;
}

bool CP_SphereCollider::CollisionBox(CP_Collider& _sphere, CP_Collider& _box)
{
	return false;
}

nlohmann::json CP_SphereCollider::Save()
{
	nlohmann::json data = CP_Collider::Save();

	data["radius"] = radius;
	HashiTaku::SaveJsonVector3("center", center, data);

	return data;
}

void CP_SphereCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);

	HashiTaku::LoadJsonFloat("radius", radius, _data);
	HashiTaku::LoadJsonVector3("center", center, _data);
}
