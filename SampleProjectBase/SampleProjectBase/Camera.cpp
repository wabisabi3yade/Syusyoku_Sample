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
	// ビュー変換行列を求める
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&eyePos),		// カメラ座標
		DirectX::XMLoadFloat3(&focusPos),	// 注視点
		DirectX::XMLoadFloat3(&camUp)	// 上ベクトル
	);

	// ビュー変換行列をセット
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}
