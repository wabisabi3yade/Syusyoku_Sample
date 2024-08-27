#include "pch.h"
#include "CP_Weapon.h"

#include "Bone.h"

#include "Geometory.h"
#include "GameObject.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

CP_Weapon::CP_Weapon() : grabBoneName(""), pGrabBone(nullptr), pMeshRenderer(nullptr), loadMeshScale(0.0f)
{
}

void CP_Weapon::Start()
{
}

void CP_Weapon::LateUpdate()
{
	if (!IsCanUpdate()) return;

	UpdateTransform();
}

void CP_Weapon::Draw()
{
	Geometory::SetPosition(GetTransform().GetPosition());
	Geometory::SetScale(GetTransform().GetScale());
	Geometory::SetRotation(GetTransform().GetEularAngles());
	Geometory::DrawCube();
}

void CP_Weapon::ImGuiSetting()
{
	if (pGrabBone)
		ImGui::Text(pGrabBone->GetBoneName().c_str());
	ImGuiMethod::DragFloat3(offsetPosition, "offset", 0.01f);
	ImGuiMethod::DragFloat3(offsetAngles, "angle");

	ImGuiSetBone();
}

void CP_Weapon::SetGrabBone(const Bone* _grabBone)
{
	pGrabBone = _grabBone;
}

void CP_Weapon::SetGrabBoneName(const std::string& _grabName)
{
	grabBoneName = _grabName;

	if (CP_MeshRenderer* pRenderer = gameObject->GetComponent<CP_MeshRenderer>())
	{
		Mesh_Group* pMesh = pRenderer->GetRenderMesh();
		if (pMesh->GetType() != Mesh_Group::MeshType::SK) return;

		Bone* pBone = static_cast<SkeletalMesh*>(pMesh)->GetBoneByName(grabBoneName);
		SetGrabBone(pBone);
	}

}

void CP_Weapon::SetSkeletalMeshData(const SkeletalMesh& _skeletalMesh)
{
}

std::string CP_Weapon::GetGrabBoneName() const
{
	return grabBoneName;
}

nlohmann::json CP_Weapon::Save()
{
	auto data = Component::Save();

	if (pGrabBone)
		data["grabName"] = pGrabBone->GetBoneName();

	HashiTaku::SaveJsonVector3("offsetPos", offsetPosition, data);
	HashiTaku::SaveJsonVector3("offsetAngles", offsetAngles, data);

	return data;
}

void CP_Weapon::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	std::string grabBoneName;
	HashiTaku::LoadJsonString("grabName", grabBoneName, _data);
	if(grabBoneName != "")


	HashiTaku::LoadJsonVector3("offsetPos", offsetPosition, _data);
	HashiTaku::LoadJsonVector3("offsetAngles", offsetAngles, _data);
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

	// ボーン行列からトランスフォームを求める
	Vector3 bonePos = {
		boneMtx.m[3][0] * 0.01f,
		boneMtx.m[3][1] * 0.01f,
		boneMtx.m[3][2] * 0.01f
	};

	t.SetLocalPos(bonePos + offsetPosition);

	Vector3 s = Vec3::WorldMtxToScale(boneMtx);
	Matrix rotateMtx = Matrix(
		boneMtx.m[0][0] / s.x, boneMtx.m[0][1] / s.x, boneMtx.m[0][2] / s.x, 0.0f,
		boneMtx.m[1][0] / s.y, boneMtx.m[1][1] / s.y, boneMtx.m[1][2] / s.y, 0.0f,
		boneMtx.m[2][0] / s.z, boneMtx.m[2][1] / s.z, boneMtx.m[2][2] / s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	Quaternion q = Quaternion::CreateFromRotationMatrix(rotateMtx);
	q = Quat::Multiply(q, Quat::ToQuaternion(Vector3(0.0f, 180.f, 0.0f)));

	t.SetLocalRotation(Quat::Multiply(q, Quat::ToQuaternion(offsetAngles)));
}

void CP_Weapon::ImGuiSetBone()
{
	static char str[IM_INPUT_BUF];
	ImGui::InputText("boneName", str, IM_INPUT_BUF);
	if (ImGui::Button("Set"))
	{
		CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
		if (!pMR) return;

		SkeletalMesh* pSk = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());
		if (!pSk) return;

		Bone* pBone = pSk->GetBoneByName(str);
		if (!pBone)
		{
			HASHI_DEBUG_LOG(std::string(str) + " ボーンを見つけられません");
			return;
		}

		SetGrabBone(pBone);
	}
}

bool CP_Weapon::IsCanUpdate()
{
	if (!pGrabBone) return false;

	return true;
}
