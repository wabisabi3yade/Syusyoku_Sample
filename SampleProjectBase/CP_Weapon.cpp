#include "pch.h"
#include "CP_Weapon.h"

#include "Bone.h"

#include "Geometory.h"

using namespace DirectX::SimpleMath;

CP_Weapon::CP_Weapon() : grabBoneName(""), pGrabBone(nullptr)
{
	name = "Weapon";
}

void CP_Weapon::LateUpdate()
{
	UpdateTransform();
}

void CP_Weapon::Draw()
{
	Geometory::SetPosition(GetTransform().GetPosition());
	Geometory::DrawSphere();
}

void CP_Weapon::SetGrabBone(const Bone& _grabBone)
{
	pGrabBone = &_grabBone;
}

void CP_Weapon::SetGrabBoneName(const std::string& _grabName)
{
	grabBoneName = _grabName;
}

std::string CP_Weapon::GetGrabBoneName() const
{
	return grabBoneName;
}

void CP_Weapon::UpdateTransform()
{
	if (!pGrabBone)
	{
		HASHI_DEBUG_LOG("ボーンが設定されていません");
		return;
	}

	const Matrix& boneMtx = pGrabBone->GetCombMtx();
	Transform& t = GetTransform();

	// 行列からトランスフォームを求める
	t.SetLocalPos({ boneMtx.m[3][0] * 0.01f, boneMtx.m[3][1] * 0.01f, boneMtx.m[3][2] * 0.01f });

	Vector3 s = Vec3::WorldMtxToScale(boneMtx);

	Matrix rotateMtx = Matrix(
		boneMtx.m[0][0] / s.x, boneMtx.m[0][1] / s.x, boneMtx.m[0][2] / s.x, 0.0f,
		boneMtx.m[1][0] / s.y, boneMtx.m[1][1] / s.y, boneMtx.m[1][2] / s.y, 0.0f,
		boneMtx.m[2][0] / s.z, boneMtx.m[2][1] / s.z, boneMtx.m[2][2] / s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	t.SetLocalRotation(Quaternion::CreateFromRotationMatrix(rotateMtx));
}
