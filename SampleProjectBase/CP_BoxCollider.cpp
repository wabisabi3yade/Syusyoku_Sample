#include "pch.h"
#include "CP_BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

// コンポーネントインクルードヘッダー
#include "ComponentDefine.h"

using namespace DirectX::SimpleMath;

CP_BoxCollider::CP_BoxCollider()
	: CP_Collider(CP_Collider::Type::Box), length(Vector3::One), sizeOffset(Vector3::One) 
{
}

void CP_BoxCollider::Start()
{
	CP_Collider::Start();	// 追加処理をする

	sizeOffset = Vector3::One;
	type = CP_Collider::Type::Box;	// ボックスに設定する

	// モデルのサイズから判定の大きさを決める
	SizeFromModelSize();
}

void CP_BoxCollider::LateUpdate()
{
	VectorUpdate();
	PosUpdate();
	LengthUpdate();
}

void CP_BoxCollider::Draw()
{
	// ボックス表示
	const Transform& t = gameObject->transform;

	// オブジェクトのスケールも考慮する
	Vector3 pos_w = t.GetPosition() + posOffset;
	Geometory::SetPosition(pos_w);

	Vector3 scale_w = t.GetScale() * sizeOffset;
	Geometory::SetScale(scale_w);

	Vector3 rotation_w;

	if (!isAABB)
		rotation_w = t.GetEularAngles();

	Geometory::SetRotation(rotation_w);

	// 色
	Geometory::SetColor(CP_Collider::normalColor);

	// 何かに当たってるなら
	if (hitColliders.size() > 0)	
		Geometory::SetColor(CP_Collider::hitColor);		// 色を変える

	Geometory::DrawCube(true);
}

void CP_BoxCollider::ImGuiSetting()
{
	ImGui::Checkbox("AABB", &isAABB);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
	/*ImGuiMethod::DragFloat3(angleOffset, "angleOffset");*/
	ImGuiMethod::DragFloat3(sizeOffset, "size");
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetCenterPos() const
{
	return centerPos;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetRightVector() const
{
	return rightVector;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetUpVector() const
{
	return upVector;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetForwardVector() const
{
	return forwardVector;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetLength() const
{
	return length;
}

bool CP_BoxCollider::CollisionBox(CP_Collider& _box1, CP_Collider& _box2)
{
	//CP_BoxCollider& obb1 = static_cast<CP_BoxCollider&>(_box1);
	//CP_BoxCollider& obb2 = static_cast<CP_BoxCollider&>(_box2);

	//// AABBとOBBで処理を変える
	//// 各方向ベクトルの確保
 //  // （N***:標準化方向ベクトル）
	//Vector3 NAe1 = obb1.GetRightVector(), Ae1 = NAe1 * obb1.GetLength().x;
	//Vector3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	//Vector3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	//Vector3 NBe1 = obb2.GetRightVector(), Be1 = NBe1 * obb2.GetLength().x;
	//Vector3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	//Vector3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	//Vector3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	//// 分離軸 : Ae1
	//FLOAT rA = D3DXVec3Length(&Ae1);
	//FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	//FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
	//if (L > rA + rB)
	//	return false; // 衝突していない

	//// 分離軸 : Ae2
	//rA = D3DXVec3Length(&Ae2);
	//rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &NAe2));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Ae3
	//rA = D3DXVec3Length(&Ae3);
	//rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &NAe3));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Be1
	//rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	//rB = D3DXVec3Length(&Be1);
	//L = fabs(D3DXVec3Dot(&Interval, &NBe1));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Be2
	//rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	//rB = D3DXVec3Length(&Be2);
	//L = fabs(D3DXVec3Dot(&Interval, &NBe2));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : Be3
	//rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	//rB = D3DXVec3Length(&Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &NBe3));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C11
	//Vector3 Cross;
	//D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	//rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C12
	//D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	//rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C13
	//D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	//rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C21
	//D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	//rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C22
	//D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	//rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C23
	//D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	//rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C31
	//D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	//rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C32
	//D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	//rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離軸 : C33
	//D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	//rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	//rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	//L = fabs(D3DXVec3Dot(&Interval, &Cross));
	//if (L > rA + rB)
	//	return false;

	//// 分離平面が存在しないので「衝突している」
	//return true;

	return true;
}

nlohmann::json CP_BoxCollider::Save()
{
	auto data = CP_Collider::Save();

	HashiTaku::SaveJsonVector3("posOffset", posOffset, data);
	HashiTaku::SaveJsonVector3("size", sizeOffset, data);
	/*HashiTaku::SaveJsonVector3("angleOffset", angleOffset, data);*/
	data["AABB"] = isAABB;

	return data;
}

void CP_BoxCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);

	HashiTaku::LoadJsonVector3("posOffset", posOffset, _data);
	HashiTaku::LoadJsonVector3("size", sizeOffset, _data);
	/*HashiTaku::LoadJsonVector3("angleOffset", angleOffset, _data);*/
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
	if (modelSize.x == 0.0f) return;

	sizeOffset = modelSize;
	posOffset = pModel->GetCenterPosition();
}

void CP_BoxCollider::PosUpdate()
{
	Transform& t = GetTransform();

	centerPos = t.GetPosition();
	centerPos += t.Up() * posOffset.y * t.GetScale().y;
	centerPos += t.Right() * posOffset.x * t.GetScale().x;
	centerPos += t.Forward() * posOffset.z * t.GetScale().z;
}

void CP_BoxCollider::VectorUpdate()
{
	if (isAABB)
	{
		rightVector = Vec3::Right;
		upVector = Vec3::Up;
		forwardVector = Vec3::Forward;
	}
	else
	{
		Transform& t = GetTransform();
		rightVector = t.Right();
		upVector = t.Up();
		forwardVector = t.Forward();
	}
}

void CP_BoxCollider::LengthUpdate()
{
}
