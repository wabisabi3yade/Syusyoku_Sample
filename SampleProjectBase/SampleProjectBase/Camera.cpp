#include "pch.h"
#include "Camera.h"

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
	GameObject::LateUpdate();
}

void Camera::UpdateViewMatrix()
{
	if (transform.position == focusPos)
	{
		transform.position.z += 0.001f;
	}

	UpdateFocus();	// 注視点を求めるs

	// ビュー変換行列を求める
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&transform.position),		// カメラ座標
		DirectX::XMLoadFloat3(&focusPos),	// 注視点
		DirectX::XMLoadFloat3(&camUp)	// 上ベクトル
	);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	// ビュー変換行列をセット
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void Camera::LookAt(DirectX::SimpleMath::Vector3 _targetPos)
{
	focusPos = _targetPos; 
}
