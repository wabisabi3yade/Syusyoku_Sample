#include "pch.h"
#include "Transform.h"
#include "NullTransform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Transform::Transform(bool _isInit)
	: pParent(nullptr), scale(Vector3::One), localScale(Vector3::One), up(Vec3::Up), right(Vec3::Right), forward(Vec3::Forward), isHaveParent(false)
{
	if (_isInit)
		pParent = new NullTransform();
}

Transform::~Transform()
{
	// �e�g�����X�t�H�[����NullTransform�Ȃ�
	if (!isHaveParent)
		CLASS_DELETE(pParent);
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
	Quaternion q = Quaternion::CreateFromRotationMatrix(
		Matrix::CreateBillboard(position, _worldPos, _upVector)
	);

	SetRotation(q);
}

void Transform::RemoveParent()
{
	pParent = new NullTransform();
	isHaveParent = false;
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
	Vector3 diffPos = _child.position - position;
	_child.SetLocalPos(diffPos);

	Vector3 diffScale = _child.scale / scale;
	_child.SetLocalScale(diffScale);

	Quaternion invRot;
	rotation.Inverse(invRot);
	Quaternion diffRot = Quat::Multiply(_child.rotation, invRot);
	_child.SetLocalRotation(diffRot);
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& _pos)
{
	position = _pos;
	localPosition = position - pParent->position;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& _scale)
{
	scale = _scale;
	localScale = scale / pParent->scale;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();
}

void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	eularAngles = _eularAngles;
	localEularAngles = eularAngles - pParent->eularAngles;

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quat::ToQuaternion(eularAngles);
	localRotation = Quat::RotationDifference(pParent->rotation, rotation);

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}

void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	rotation = _quaternion;
	localRotation = Quat::RotationDifference(pParent->rotation, rotation);

	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->eularAngles;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}


void Transform::SetLocalPos(const DirectX::SimpleMath::Vector3& _localPos)
{
	localPosition = _localPos;
	position = pParent->position;
	position += pParent->Right() * localPosition.x;
	position += pParent->Up() * localPosition.y;
	position += pParent->Forward() * localPosition.z;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();
}

void Transform::SetLocalScale(const DirectX::SimpleMath::Vector3& _scale)
{
	localScale = _scale;
	scale = pParent->scale * localScale;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();
}

void Transform::SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	localEularAngles = _eularAngles;

	// �N�H�[�^�j�I���ɔ��f������
	localRotation = Quat::ToQuaternion(localEularAngles);

	eularAngles = pParent->eularAngles + localEularAngles;
	rotation = Quat::Multiply(localRotation, pParent->rotation);

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}

void Transform::SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	localRotation = _quaternion;

	// �p�x�ɔ��f������
	localEularAngles = Quat::ToEulerAngles(localRotation);

	eularAngles = pParent->eularAngles + localEularAngles;
	rotation = Quat::Multiply(localRotation, pParent->rotation);

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
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

u_int Transform::GetChilidCnt() const
{
	return static_cast<u_int>(pChilds.size());
}

void Transform::UpdateHierarchyPositions()
{
	position = pParent->position;
	position += pParent->Right() * localPosition.x;
	position += pParent->Up() * localPosition.y;
	position += pParent->Forward() * localPosition.z;

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
	rotation = Quat::Multiply(localRotation, pParent->rotation);
	eularAngles = pParent->eularAngles + localEularAngles;

	// �ċA�ŌĂяo��
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}