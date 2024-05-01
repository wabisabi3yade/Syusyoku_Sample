#include "pch.h"
#include "Camera.h"

Camera::Camera() :eyePos(0, -300, -300)
{
}

Camera::~Camera()
{
}

void Camera::Update()
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
