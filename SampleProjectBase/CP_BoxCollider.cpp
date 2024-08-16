#include "pch.h"
#include "CP_BoxCollider.h"
#include "Geometory.h"
#include "GameObject.h"

// �R���|�[�l���g�C���N���[�h�w�b�_�[
#include "ComponentDefine.h"

using namespace DirectX::SimpleMath;

void CP_BoxCollider::CheckCollisionAABB(CP_Collider& _box1, CP_Collider& _box2)
{

}

void CP_BoxCollider::CheckCollisionOBB(CP_Collider& _box1, CP_Collider& _box2)
{
}

void CP_BoxCollider::Init()
{
	name = "BoxCollider";

	CP_Collider::Init();	// �ǉ�����������

	size = Vector3::One;
	type = CP_Collider::Type::Box;	// �{�b�N�X�ɐݒ肷��

	// ���f���̃T�C�Y���画��̑傫�������߂�
	SizeFromModelSize();
}

void CP_BoxCollider::Draw()
{
	// �{�b�N�X�\��
	const Transform& t = gameObject->transform;

	// �I�u�W�F�N�g�̃X�P�[�����l������
	Vector3 pos_w = t.GetPosition() + posOffset;
	Geometory::SetPosition(pos_w);

	Vector3 scale_w = t.GetScale() * size;
	Geometory::SetScale(scale_w);

	Vector3 rotation_w;

	if (!isAABB)
		rotation_w = t.GetEularAngles() + angleOffset;

	Geometory::SetRotation(rotation_w);

	// �F
	Geometory::SetColor(CP_Collider::normalColor);

	// �����ɓ������Ă�Ȃ�
	if (hitColliders.size() > 0)	
		Geometory::SetColor(CP_Collider::hitColor);		// �F��ς���

	Geometory::DrawCube(true);
}

void CP_BoxCollider::ImGuiSetting()
{
	ImGui::Checkbox("AABB", &isAABB);
	ImGuiMethod::DragFloat3(posOffset, "posOffset");
	ImGuiMethod::DragFloat3(angleOffset, "angleOffset");
	ImGuiMethod::DragFloat3(size, "size");
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldCenterPos() const
{
	return GetTransform().GetPosition() + posOffset;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldRotation() const
{
	return GetTransform().GetEularAngles() + angleOffset;
}

DirectX::SimpleMath::Vector3 CP_BoxCollider::GetWorldScale() const
{
	return GetTransform().GetScale() * size;
}

bool CP_BoxCollider::CollisionBox(CP_Collider& _box1, CP_Collider& _box2)
{
	// AABB��OBB�ŏ�����ς���



	return false;
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
	if (modelSize.x == 0.0f) return;

	size = modelSize;
	posOffset = pModel->GetCenterPosition();
}
