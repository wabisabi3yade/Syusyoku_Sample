#include "pch.h"
#include "Transform.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

Transform::Transform() : scale(DirectX::SimpleMath::Vector3::One), up(0,1,0), right(1,0,0), forward(0,0,1), pParent(nullptr)
{
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

void Transform::ResetParameter()
{
	// �p�����[�^������
	position = Vector3::Zero;
	eularAngles = Vector3::Zero;
	scale = Vector3::One;
	rotation = Quaternion::Identity;
}

void Transform::LookAt(DirectX::SimpleMath::Vector3 _worldPos, const DirectX::SimpleMath::Vector3 _upVector)
{
	Quaternion q = Quaternion::CreateFromRotationMatrix(Matrix::CreateBillboard(position, _worldPos, _upVector));

	SetRotation(q);
}

void Transform::SetParent(Transform& _parent)
{
	pParent = &_parent;
}

void Transform::SetEularAngleX(float _angle_x)
{
	eularAngles.x = _angle_x;

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quaternion::CreateFromYawPitchRoll(
		eularAngles.y * Mathf::degToRad,
		eularAngles.x * Mathf::degToRad,
		eularAngles.z * Mathf::degToRad
	);
}

void Transform::SetEularAngleY(float _angle_y)
{
	eularAngles.y = _angle_y;

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quaternion::CreateFromYawPitchRoll(
		eularAngles.y * Mathf::degToRad,
		eularAngles.x * Mathf::degToRad,
		eularAngles.z * Mathf::degToRad
	);
}

void Transform::SetEularAngleZ(float _angle_z)
{
	eularAngles.z = _angle_z;

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quaternion::CreateFromYawPitchRoll(
		eularAngles.y * Mathf::degToRad,
		eularAngles.x * Mathf::degToRad,
		eularAngles.z * Mathf::degToRad
	);
}

void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	eularAngles = _eularAngles;

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quaternion::CreateFromYawPitchRoll(
		eularAngles.y * Mathf::degToRad,
		eularAngles.x * Mathf::degToRad,
		eularAngles.z * Mathf::degToRad
	);
}

void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	rotation = _quaternion;

	eularAngles = Quat::ToEulerAngles(rotation);
}

DirectX::SimpleMath::Vector3 Transform::GetEularAngles() const
{
	return eularAngles;
}

DirectX::SimpleMath::Quaternion Transform::GetRotation() const
{
	return rotation;
}
