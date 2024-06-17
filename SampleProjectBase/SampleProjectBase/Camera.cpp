#include "pch.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

using namespace DirectX::SimpleMath;

void Camera::UpdateFocus()
{

}

Camera::Camera() :focusPos(0, 0, 0)
{
	transform.position.z = -9.f;
}

Camera::~Camera()
{
}

void Camera::LateUpdate()
{
}

void Camera::UpdateViewMatrix()
{
	if (transform.position == focusPos)
	{
		transform.position.z += 0.001f;
	}

	UpdateFocus();	// �����_�����߂�s

	// �r���[�ϊ��s������߂�
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&transform.position),		// �J�������W
		DirectX::XMLoadFloat3(&focusPos),	// �����_
		DirectX::XMLoadFloat3(&camUp)	// ��x�N�g��
	);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	// �r���[�ϊ��s����Z�b�g
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}
