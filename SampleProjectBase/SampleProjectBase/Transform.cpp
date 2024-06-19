#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;


void Transform::UpdateVector()
{
	//�O�������Z�b�g
	Vector3 initForward = Vector3::Forward;

	// ��]�s������Ƃ߂�
	Matrix rotateMatrix = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(rotation.y),
		DirectX::XMConvertToRadians(rotation.x),
		DirectX::XMConvertToRadians(rotation.z)
	);
	rotateMatrix.Transpose();

	forward = Vector3::Transform(initForward, rotateMatrix);	// �X�V����
	forward.Normalize();	// ���K��
}


void Transform::ResetParameter()
{
	// �p�����[�^������
	position = Vector3::Zero;
	rotation = Vector3::Zero;
	scale = Vector3::One;
	quaternion = Quaternion::Identity;
}

void Transform::LookAt(DirectX::SimpleMath::Vector3 _worldPos)
{

}
