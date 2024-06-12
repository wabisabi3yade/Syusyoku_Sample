#include "pch.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

using namespace DirectX::SimpleMath;

Camera::Camera() :focusPos(0,0,0)
{
	transform.position.z = -9.f;
}

Camera::~Camera()
{
}

void Camera::LateUpdate()
{
	ImGui::Begin("Camera");
	ImGui::SliderFloat("pos", &transform.position.z, -30.f, 10.f);
	ImGui::SliderFloat("look", &focusPos.x, -30.f, 10.f);
	ImGui::End();

	ImGuiDebugLog::Add(std::to_string(transform.position.z));
}

void Camera::UpdateViewMatrix()
{
	if (transform.position == focusPos)
	{
		transform.position.z += 0.001f;
	}

	Vector3 p = {0,0,-10.f};

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
