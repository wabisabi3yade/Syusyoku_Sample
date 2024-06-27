#include "pch.h"
#include "Transform.h"
using namespace DirectX::SimpleMath;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
void Transform::UpdateVector()
{
	// ��]�s������Ƃ߂�
	Matrix rotateMatrix = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(rotation.y),
		DirectX::XMConvertToRadians(rotation.x),
		DirectX::XMConvertToRadians(rotation.z)
	);

	// �X�V����
	// rotateMatrix[0] �� �E����
	right.x = rotateMatrix.m[0][0];
	right.y = rotateMatrix.m[0][1];
	right.z = rotateMatrix.m[0][2];

	// rotateMatrix[1] �� �㐬��
	up.x = rotateMatrix.m[1][0];
	up.y = rotateMatrix.m[1][1];
	up.z = rotateMatrix.m[1][2];

	// rotateMatrix[2] �� �O����
	forward.x = rotateMatrix.m[2][0];
	forward.y = rotateMatrix.m[2][1];
	forward.z = rotateMatrix.m[2][2];
}


void Transform::ResetParameter()
{
	// �p�����[�^������
	position = Vector3::Zero;
	rotation = Vector3::Zero;
	scale = Vector3::One;
	quaternion = Quaternion::Identity;
}

void Transform::LookAt(DirectX::SimpleMath::Vector3 _worldPos, const DirectX::SimpleMath::Vector3 _upVector)
{
	// Atan2�֐�����x�N�g���Ŋp�x�����߂�
	Vector3 vec = _worldPos - position;
	vec.Normalize();
	forward = vec;	// ���ʃx�N�g���Ƃ���
	up = Vector3::Up;	// �����
	right = up.Cross(forward);	// �E�����x�N�g�������߂�
	right.Normalize();
	up = forward.Cross(right);	// �^��������Čv�Z����
	up.Normalize();	

	//float sy = sqrtf(forward.x * forward.x + up.x * up.x);
	//bool singular = sy < 1e-6;	// �߂�����Ƃ���
	//if (!singular)
	//{
	//	rotation.x = atan2f(forward.y, forward.z) * Mathf::radToDeg;
	//	rotation.y = atan2f(-forward.x, sy) * Mathf::radToDeg;
	//	rotation.z = atan2f(up.x, right.x) * Mathf::radToDeg;
	//}
	//else {
	//	rotation.x = atan2f(-up.z, up.y) * Mathf::radToDeg;
	//	rotation.y = atan2f(-forward.x, sy) * Mathf::radToDeg;
	//	rotation.z = 0;
	//}

	Vector3 vec = _worldPos - position;
	float distance = vec.Length();
	if (distance == 0.0f)
	{
		position.z -= 0.0001f;
	}

	Matrix rotateMatrix = Matrix::CreateLookAt(position, _worldPos, Vector3::Up);
	// �X�V����
	// rotateMatrix[0] �� �E����
	right.x = rotateMatrix.m[0][0];
	right.y = rotateMatrix.m[0][1];
	right.z = rotateMatrix.m[0][2];

	// rotateMatrix[1] �� �㐬��
	up.x = rotateMatrix.m[1][0];
	up.y = rotateMatrix.m[1][1];
	up.z = rotateMatrix.m[1][2];

	// rotateMatrix[2] �� �O����
	forward.x = rotateMatrix.m[2][0];
	forward.y = rotateMatrix.m[2][1];
	forward.z = rotateMatrix.m[2][2];
}
