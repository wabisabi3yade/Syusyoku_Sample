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

void CP_Weapon::Init()
{
	grabBoneName = "mixamorig:RightHand";
}

void CP_Weapon::Start()
{
	SetBoneFromParent();
}

void CP_Weapon::LateUpdate()
{
	if (!IsCanUpdate()) return;

	UpdateTransform();
}

void CP_Weapon::Draw()
{
}

void CP_Weapon::ImGuiSetting()
{
	ImGui::Text(grabBoneName.c_str());
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

void CP_Weapon::SetSkeletalMeshData(SkeletalMesh& _skeletalMesh)
{
	loadMeshScale = _skeletalMesh.GetLoadOffsetScale();
	loadMeshAngles = _skeletalMesh.GetLoadOffsetAngles();

	Bone* pBone = _skeletalMesh.GetBoneByName(grabBoneName);
	if (pBone)
	{
		pGrabBone = pBone;
	}
}

std::string CP_Weapon::GetGrabBoneName() const
{
	return grabBoneName;
}

nlohmann::json CP_Weapon::Save()
{
	auto data = Component::Save();

	data["grabName"] = grabBoneName;

	HashiTaku::SaveJsonVector3("offsetPos", offsetPosition, data);
	HashiTaku::SaveJsonVector3("offsetAngles", offsetAngles, data);

	return data;
}

void CP_Weapon::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	std::string grabBoneName;
	HashiTaku::LoadJsonString("grabName", grabBoneName, _data);

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

	const Matrix& boneMtx = pGrabBone->GetGlobalMtx();
	Transform& t = GetTransform();

	// ボーン行列からトランスフォームを求める
	Vector3 bonePos = {
		boneMtx._41,
		boneMtx._42,
		boneMtx._43
	};

	t.SetLocalPosition(bonePos + offsetPosition);

	Vector3 s = Vec3::WorldMtxToScale(boneMtx);

	Matrix rotateMtx = Matrix(
		boneMtx._11 / s.x, boneMtx._12 / s.x, boneMtx._13 / s.x, 0.0f,
		boneMtx._21 / s.y, boneMtx._22 / s.y, boneMtx._23 / s.y, 0.0f,
		boneMtx._31 / s.z, boneMtx._32 / s.z, boneMtx._33 / s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	Quaternion rot = Quaternion::CreateFromRotationMatrix(rotateMtx);
	rot = Quat::Multiply(rot, Quat::ToQuaternion(offsetAngles));
	t.SetLocalRotation(Quat::Multiply(rot, Quat::ToQuaternion(offsetAngles)));
}

void CP_Weapon::ImGuiSetBone()
{
	static char str[IM_INPUT_BUF];
	if (ImGui::InputText("boneName", str, IM_INPUT_BUF, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		grabBoneName = str;
	}

	if (ImGui::Button("Update"))
	{
		SetBoneFromParent();
	}
}

bool CP_Weapon::IsCanUpdate()
{
	if (!pGrabBone) return false;

	return true;
}

void CP_Weapon::SetBoneFromParent()
{
	Transform* pParent = GetTransform().GetParent();

	if (!pParent) return;

	CP_MeshRenderer* pMr = pParent->GetGameObject().GetComponent<CP_MeshRenderer>();
	if (!pMr) return;

	SkeletalMesh* pSk = dynamic_cast<SkeletalMesh*>(pMr->GetRenderMesh());
	if (!pSk) return;

	SetSkeletalMeshData(*pSk);
}
