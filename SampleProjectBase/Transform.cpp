#include "Transform.h"
#include "NullTransform.h"

#include "InSceneSystemManager.h"

#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace HashiTaku;

Transform::Transform(GameObject* _pGameObject, bool _isInit)
	: pParent(nullptr), scale(Vector3::One), localScale(Vector3::One), up(Vec3::Up), right(Vec3::Right), forward(Vec3::Forward), isHaveParent(false)
{
	if (_isInit)
		pParent = new NullTransform();

	pGameObject = _pGameObject;
}

Transform::Transform(const Transform& _other)
	: pParent(nullptr), scale(Vector3::One), localScale(Vector3::One), up(Vec3::Up), right(Vec3::Right), forward(Vec3::Forward), isHaveParent(false)
{
	Copy(_other);
}

Transform::~Transform()
{
	// �e�g�����X�t�H�[����NullTransform�Ȃ�
	if (!isHaveParent)
		CLASS_DELETE(pParent);
}

Transform& Transform::operator=(const Transform& _other)
{
	Copy(_other);

	return *this;
}

void Transform::UpdateVector()
{
	// �N�H�[�^�j�I�������]�s������߂�
	Matrix rotateMatrix(Matrix::CreateFromQuaternion(rotation));

	// �����x�N�g�����X�V
	right = Mtx::GetRightVector(rotateMatrix);
	up = Mtx::GetUpVector(rotateMatrix);
	forward = Mtx::GetForwardVector(rotateMatrix);
}

void Transform::LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector)
{
	Vector3 vec = _worldPos - position;
	Quaternion q = Quat::RotateToVector(vec);

	SetRotation(q);
}

void Transform::RemoveChild(Transform& _removeTransform)
{
	pChilds.remove(&_removeTransform);
}

void Transform::SetParent(Transform& _parent)
{
	_parent.SetChild(*this);
}

void Transform::SetChild(Transform& _child)
{
	if (std::find(pChilds.begin(), pChilds.end(), &_child) != pChilds.end())
	{
		HASHI_DEBUG_LOG("���Ɏq�I�u�W�F�N�g�ł�");
		return;
	}

	pChilds.push_back(&_child);

	if (!_child.isHaveParent)	// �e�g�����X�t�H�[����null�Ȃ�
		CLASS_DELETE(_child.pParent);

	_child.pParent = this;
	_child.isHaveParent = true;

	// ���[�J���p�����[�^�ɔ��f
	Vector3 diffPos = (_child.position - position) / GetScale() ;
	_child.SetLocalPosition(diffPos);

	Vector3 diffScale = _child.scale / scale;
	_child.SetLocalScale(diffScale);

	Quaternion diffRot = Quat::RotationDifference(pParent->GetRotation(), _child.GetRotation());
	_child.SetLocalRotation(diffRot);
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& _pos)
{
	position = _pos;
	localPosition = position - pParent->position;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& _scale)
{
	scale = _scale;
	localScale = scale / pParent->scale;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	eularAngles = _eularAngles;
	localEularAngles = eularAngles - pParent->eularAngles;

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quat::ToQuaternion(eularAngles);
	localRotation = Quat::RotationDifference(pParent->GetRotation(), rotation);

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	rotation = _quaternion;
	localRotation = Quat::RotationDifference(rotation, pParent->GetRotation());

	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->eularAngles;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}


void Transform::SetLocalPosition(const DirectX::SimpleMath::Vector3& _localPos)
{
	localPosition = _localPos;
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetLocalScale(const DirectX::SimpleMath::Vector3& _scale)
{
	localScale = _scale;
	scale = pParent->scale * localScale;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	localEularAngles = _eularAngles;

	// �N�H�[�^�j�I���ɔ��f������
	localRotation = Quat::ToQuaternion(localEularAngles);

	eularAngles = pParent->eularAngles + localEularAngles;
	rotation = Quat::Multiply(localRotation, pParent->GetRotation());

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	localRotation = _quaternion;

	// �p�x�ɔ��f������
	localEularAngles = Quat::ToEulerAngles(localRotation);

	rotation = Quat::Multiply(localRotation, pParent->GetRotation());
	eularAngles = rotation.ToEuler();

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// RigidBody�Ɠ���������
	SetRigidBodyTransform();
}

void Transform::SetGameObject(GameObject& _go)
{
	pGameObject = &_go;
}

DirectX::SimpleMath::Vector3 Transform::GetPosition() const
{
	return position;
}

DirectX::SimpleMath::Vector3 Transform::GetScale() const
{
	return scale;
}

DirectX::SimpleMath::Vector3 Transform::GetEularAngles() const
{
	return eularAngles;
}

DirectX::SimpleMath::Quaternion Transform::GetRotation() const
{
	return rotation;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition() const
{
	return localPosition;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalScale() const
{
	return localScale;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalEularAngles() const
{
	return localEularAngles;
}

DirectX::SimpleMath::Quaternion Transform::GetLocalRotation() const
{
	return localRotation;
}

GameObject& Transform::GetGameObject()
{
	return *pGameObject;
}

Transform* Transform::GetParent()
{
	if (!isHaveParent)
		return nullptr;

	return pParent;
}

u_int Transform::GetChilidCnt() const
{
	return static_cast<u_int>(pChilds.size());
}

nlohmann::json Transform::Save()
{
	nlohmann::json transformData;

	SaveJsonVector3("pos", position, transformData);
	SaveJsonVector3("scale", scale, transformData);
	SaveJsonVector3("angle", eularAngles, transformData);

	auto& childData = transformData["child"];
	for (auto& child : pChilds)
	{
		childData.push_back(child->GetGameObject().GetName());
	}

	return transformData;
}

void Transform::Load(const nlohmann::json& _transformData)
{
	Vector3 v;
	LoadJsonVector3("pos", v, _transformData);
	SetPosition(v);

	LoadJsonVector3("scale", v, _transformData);
	SetScale(v);

	LoadJsonVector3("angle", v, _transformData);
	SetEularAngles(v);
}

void Transform::LoadChildTransform(const nlohmann::json& _transformData)
{
	// �e�q�g�����X�t�H�[���擾
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	if (IsJsonContains(_transformData, "child"))
	{
		const auto& childData = _transformData["child"];
		u_int childNum = static_cast<u_int>(childData.size());
		for (u_int c_i = 0; c_i < childNum; c_i++)
		{
			GameObject* go = sceneObjects.GetSceneObject(childData[c_i]);

			if (!go) continue;
			SetChild(go->GetTransform());
		}
	}
}

void Transform::Copy(const Transform& _other)
{
	if (this == &_other) return;

	position = _other.position;
	scale = _other.scale;
	rotation = _other.rotation;
}

void Transform::UpdateHierarchyPositions()
{
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;

	// �ċA�ŌĂяo��
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();
}

void Transform::UpdateHierarchyScales()
{
	scale = pParent->scale * localScale;

	// �ċA�ŌĂяo��
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();
}

void Transform::UpdateHierarchyRotations()
{
	rotation = Quat::Multiply(localRotation, pParent->GetRotation());
	eularAngles = pParent->eularAngles + localEularAngles;

	// �ċA�ŌĂяo��
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}

void Transform::SetRigidBodyTransform()
{
	if (!pGameObject->GetHasRigidBody()) return;

	CP_RigidBody* pRb = pGameObject->GetRigidBody();
	assert(pRb && "RigidBody������܂���");

	pRb->SetTransformDxToBt(*this);
}

void Transform::RemoveParentChild()
{
	if (pParent)
		pParent->RemoveChild(*this);

	for (auto& child : pChilds)
	{
		child->pGameObject->Destroy();
	}
}
