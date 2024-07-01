#include "pch.h"
#include "Transform.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
void Transform::UpdateVector()
{
	// ��]�s������߂�
	Matrix rotateMatrix = Matrix::CreateFromYawPitchRoll(
		rotation.y * Mathf::degToRad,
		rotation.x * Mathf::degToRad,
		rotation.z * Mathf::degToRad
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
	// �������Ȃ��Ȃ�
	Vector3 vec = _worldPos - position;
	float distance = vec.Length();
	if (distance == 0.0f)
		position.z -= 0.0001f;	// �������炷
	vec.Normalize();

	// ���W�ւ̃r���[�s������߁A�t�s��ɂ���
	Matrix lookMatrix = DirectX::XMMatrixLookAtLH(position, _worldPos, Vector3::Up);
	Matrix invertMatrix = lookMatrix.Invert();

	// �X�V����
	// invertMatrix[0] �� �E����
	right.x = invertMatrix.m[0][0];
	right.y = invertMatrix.m[0][1];
	right.z = invertMatrix.m[0][2];

	// invertMatrix[1] �� �㐬��
	up.x = invertMatrix.m[1][0];
	up.y = invertMatrix.m[1][1];
	up.z = invertMatrix.m[1][2];

	// invertMatrix[2] �� �O����
	forward.x = invertMatrix.m[2][0];
	forward.y = invertMatrix.m[2][1];
	forward.z = invertMatrix.m[2][2];

	// Atan2�֐�����x�N�g���Ŋp�x�����߂�
	// ���ʃx�N�g���Ƃ���
	forward = vec;
	up = Vec3::Up;

	// �E�����x�N�g�������߂�
	right = forward.Cross(up);
	right.Normalize();

	// �^��������Čv�Z����
	up = right.Cross(forward);
	up.Normalize();

	// �e���p�x�����߂�
	// Y��
	Vector3 v = up;

	// ���߂������̐�����0(XZ����)
	v.y = 0.0f;
	v.Normalize();

	// ��̃��[���h�n�̒P�ʎ��x�N�g���Ɠ���
	float dotXZ = Vec3::Dot(v, Vec3::Right);
	rotation.y = acosf(dotXZ) * Mathf::radToDeg - 90.0f;

	// X��
	v = right;
	v.x = 0.0f;
	v.Normalize();
	float dotYZ = Vec3::Dot(v, Vec3::Forward);
	rotation.x = acosf(dotYZ) * Mathf::radToDeg;

	// Z��
	v = forward;
	v.z = 0.0f;
	v.Normalize();
	float dotXY = Vec3::Dot(v, Vec3::Right);
	rotation.z = acosf(dotXY) * Mathf::radToDeg;
}
