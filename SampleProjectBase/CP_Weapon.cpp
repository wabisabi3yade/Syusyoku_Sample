#include "pch.h"
#include "CP_Weapon.h"

#include "Bone.h"

#include "Geometory.h"
#include "GameObject.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

CP_Weapon::CP_Weapon() : grabBoneName(""), pGrabBone(nullptr), pMeshRenderer(nullptr), loadMeshScale(0.0f), isDisplayPos(false)
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

void CP_Weapon::ImGuiSetting()
{
	ImGui::Checkbox("display", &isDisplayPos);
	ImGui::Text(grabBoneName.c_str());
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
	loadMeshRot = Quat::ToQuaternion(_skeletalMesh.GetLoadOffsetAngles());

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

	return data;
}

void CP_Weapon::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	std::string grabBoneName;
	HashiTaku::LoadJsonString("grabName", grabBoneName, _data);
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
	Vector3 pos;
	Vector3 scale;
	Quaternion rot;

	// ボーン行列から座標と回転量を求める
	Mtx::GetTransformFromWldMtx(boneMtx, pos, scale, rot);

	pos *= loadMeshScale;
	pos = Vector3::Transform(pos, Matrix::CreateFromQuaternion(loadMeshRot));

	t.SetLocalPosition(pos);
	t.SetLocalRotation(Quat::Multiply(rot, loadMeshRot));
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
