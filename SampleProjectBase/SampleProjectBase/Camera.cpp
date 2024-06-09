#include "pch.h"
#include "Camera.h"

Camera::Camera() :eyePos(0, 0, -5), focusPos(0,0,0)
{
}

Camera::~Camera()
{
}

void Camera::LateUpdate()
{
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	// �r���[�ϊ��s������߂�
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eyePos),		// �J�������W
		DirectX::XMLoadFloat3(&focusPos),	// �����_
		DirectX::XMLoadFloat3(&camUp)	// ��x�N�g��
	);

	return viewMatrix;
}
