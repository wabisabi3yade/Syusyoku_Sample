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
	// ビュー変換行列を求める
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eyePos),		// カメラ座標
		DirectX::XMLoadFloat3(&focusPos),	// 注視点
		DirectX::XMLoadFloat3(&camUp)	// 上ベクトル
	);

	return viewMatrix;
}
