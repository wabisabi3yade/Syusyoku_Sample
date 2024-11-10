#include "pch.h"
#include "CP_Rig.h"

#include "Bone.h"

#include "Geometory.h"
#include "GameObject.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

CP_Rig::CP_Rig() : grabBoneName(""), pGrabBone(nullptr), pMeshRenderer(nullptr), loadMeshScale(0.0f), isDisplayPos(false)
{
}

void CP_Rig::Init()
{
}

void CP_Rig::Start()
{
	SetBoneFromParent();
}

void CP_Rig::LateUpdate()
{
	if (!IsCanUpdate()) return;

	UpdateTransform();
}

void CP_Rig::Draw()
{
#ifdef EDIT
	if (isDisplayPos)
	{
		Geometory::SetPosition(GetTransform().GetPosition());
		Geometory::SetScale(Vector3::One * 0.2f);
		Geometory::SetRotation(GetTransform().GetEularAngles());
		Geometory::DrawCube();
	}
#endif // EDIT
}

void CP_Rig::ImGuiDebug()
{
	ImGui::Checkbox("display", &isDisplayPos);
	ImGui::Text(grabBoneName.c_str());
	ImGuiSetBone();
}

void CP_Rig::SetGrabBone(const Bone* _grabBone)
{
	pGrabBone = _grabBone;
}

void CP_Rig::SetGrabBoneName(const std::string& _grabName)
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

void CP_Rig::SetSkeletalMeshData(SkeletalMesh& _skeletalMesh)
{
	loadMeshScale = _skeletalMesh.GetLoadOffsetScale();
	loadMeshRot = Quat::ToQuaternion(_skeletalMesh.GetLoadOffsetAngles());

	Bone* pBone = _skeletalMesh.GetBoneByName(grabBoneName);
	if (pBone)
	{
		pGrabBone = pBone;
	}
}

std::string CP_Rig::GetGrabBoneName() const
{
	return grabBoneName;
}

nlohmann::json CP_Rig::Save()
{
	auto data = Component::Save();

	data["grabName"] = grabBoneName;

	return data;
}

void CP_Rig::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
	HashiTaku::LoadJsonString("grabName", grabBoneName, _data);
}

void CP_Rig::UpdateTransform()
{
	if (!pGrabBone)
	{
		HASHI_DEBUG_LOG("ボーンが設定されていません");
		return;
	}

	const Matrix& boneMtx = pGrabBone->GetGlobalMtx();
	Transform& t = GetTransform();
	Vector3 pos;
	Vector3 scale;
	Quaternion rot;

	// ボーン行列から座標と回転量を求める
	Mtx::GetTransformFromWldMtx(boneMtx, pos, scale, rot);

	t.SetLocalPosition(pos);

	t.SetLocalRotation(rot);
}

void CP_Rig::ImGuiSetBone()
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

bool CP_Rig::IsCanUpdate()
{
	if (!pGrabBone) return false;

	return true;
}

void CP_Rig::SetBoneFromParent()
{
	Transform* pParent = GetTransform().GetParent();

	if (!pParent) return;

	CP_MeshRenderer* pMr = pParent->GetGameObject().GetComponent<CP_MeshRenderer>();
	if (!pMr) return;

	SkeletalMesh* pSk = dynamic_cast<SkeletalMesh*>(pMr->GetRenderMesh());
	if (!pSk) return;

	SetSkeletalMeshData(*pSk);
}
