#include "pch.h"
#include "SphereCollider.h"
#include "Geometory.h"
#include "GameObject.h"

constexpr float VERTEX_RADIUS(0.5f);	// 頂点作成時の半径

using namespace DirectX::SimpleMath;

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Init()
{
	name = "SphereCollider";

	Collider::Init();	// 追加処理する

	radius = VERTEX_RADIUS;	// 半径を初期化
	type = Type::Sphere;	// 球と設定する
}

void SphereCollider::Draw()
{
	//// あたり判定描画
	//const Transform& t = gameObject->transform;
	//Vector3 centerPos = t.position + posOffset * t.scale;	// 中心座標
	//Geometory::SetPosition(centerPos);
	//// 大きさを求める
	//float scale = radius / VERTEX_RADIUS;
	//Geometory::SetScale(Vector3::One * scale);

	//// 当たってるかで色変える
	//Geometory::SetColor(normalColor);
	//if (hitColliders.size() > 0)
	//	Geometory::SetColor(hitColor);

	//Geometory::DrawSphere(true);
}

void SphereCollider::SetParameter()
{
	ImGui::DragFloat("radius", &radius);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
}

DirectX::SimpleMath::Vector3 SphereCollider::GetCenterPos() const
{
	const Transform& t = gameObject->transform;
	return t.position + posOffset * t.scale;
}

bool SphereCollider::CollisionSphere(Collider& _sphere1, Collider& _sphere2)
{
	// これ以前に種類をチェックし安全なので実行速度重視でstatoc_cast変換する
	SphereCollider& s1 = static_cast<SphereCollider&>(_sphere1);
	SphereCollider& s2 = static_cast<SphereCollider&>(_sphere2);

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