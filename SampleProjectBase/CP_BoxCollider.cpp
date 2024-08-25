#include "pch.h"
#include "CP_BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

// �R���|�[�l���g�C���N���[�h�w�b�_�[
#include "ComponentDefine.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

CP_BoxCollider::CP_BoxCollider()
	: CP_Collider(CP_Collider::Type::Box), length(Vector3::One)
{
}

void CP_BoxCollider::Start()
{
	CP_Collider::Start();

	// ���f���̃T�C�Y���画��̑傫�������߂�
	SizeFromModelSize();

	CreateBoxShape();
}

void CP_BoxCollider::CreateBoxShape()
{
	// �{�b�N�X�`��쐬
	Vector3 wide = length / 2;
	pCollisionShape = std::make_unique<btBoxShape>(Bullet::ToBtVector3(wide));

	SendShapeToRb(*pCollisionShape);
}

void CP_BoxCollider::LateUpdate()
{
	PosUpdate();
	LengthUpdate();
}

void CP_BoxCollider::ImGuiSetting()
{
	ImGui::Checkbox("AABB", &isAABB);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
	ImGuiMethod::DragFloat3(length, "length");
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetCenterPos() const
{
	return centerPos;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetLength() const
{
	return length;
}

nlohmann::json CP_BoxCollider::Save()
{
	auto data = CP_Collider::Save();

	HashiTaku::SaveJsonVector3("posOffset", posOffset, data);
	HashiTaku::SaveJsonVector3("length", length, data);
	data["AABB"] = isAABB;

	return data;
}

void CP_BoxCollider::Load(const nlohmann::json& _data)
{
	CP_Collider::Load(_data);

	HashiTaku::LoadJsonVector3("posOffset", posOffset, _data);
	HashiTaku::LoadJsonVector3("length", length, _data);
	HashiTaku::LoadJsonBoolean("AABB", isAABB, _data);
}

void CP_BoxCollider::SizeFromModelSize()
{
	// ���b�V�������_���[���擾
	CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();

	if (pMeshRenderer == nullptr) return;

	const Mesh_Group* pModel = pMeshRenderer->GetRenderMesh();

	if (pModel == nullptr) return;

	// �T�C�Y�擾
	Vector3 modelSize = pModel->GetSize();

	// �T�C�Y���擾���Ă��Ȃ�������I���
	if (modelSize.x <= Mathf::epsilon) return;

	length = modelSize;
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

void CP_BoxCollider::LengthUpdate()
{
}
