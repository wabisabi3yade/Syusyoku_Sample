#include "pch.h"
#include "Camera.h"
#include "Direct3D11.h"

Camera::Camera() :eyePos(0, 0, -5), focusPos(0,0,0)
{
}

Camera::~Camera()
{
}

void Camera::LateUpdate()
{
}

void Camera::UpdateViewMatrix()
{
	// �r���[�ϊ��s������߂�
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&eyePos),		// �J�������W
		DirectX::XMLoadFloat3(&focusPos),	// �����_
		DirectX::XMLoadFloat3(&camUp)	// ��x�N�g��
	);

	// �r���[�ϊ��s����Z�b�g
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}
