#include "pch.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

Camera::Camera() :focusPos(0, 0, 0), fov(DEFAULT_FOV), nearZ(DEFAULT_NEARZ), farZ(DEFAULT_FARZ),
	isOrthographic(false)
{
	transform.position.z = -10.0f;
	SetPerspective();	// 透視投影から始める
}

Camera::~Camera()
{
}

void Camera::UpdateViewMatrix()
{
	focusPos = transform.position + transform.Forward() * 1.0f;

	if (Vector3::Distance(transform.position, focusPos) < 0.0001f)
	{
		focusPos.z += 0.001f;
	}

	// ビュー変換行列を求める
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		transform.position,		// カメラ座標
		focusPos,	// 注視点
		Vector3::Up // 上ベクトル
	);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	// ビュー変換行列をセット
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void Camera::UpdatePerspective(u_int _viewPortSlot)
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ビューポートを取得
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(_viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// ビュー変換行列を作成する
	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		screenWidth / screenHeight,   // アスペクト比
		nearZ,
		farZ);
	mat = XMMatrixTranspose(mat);

	// 投影行列の参照を取得し、ビュー変換行列を代入する
	RenderParam& param = renderer.GetParameter();
	param.SetProjection(mat);
}

void Camera::UpdateOrthographic(u_int _viewPortSlot)
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ビューポートを取得
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(_viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;
	// 正投影行列を作成する
	Matrix mat = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,	// 左上
		screenWidth, 		// 右上
		screenHeight,		// 左下
		0.0f,	// 右下
		0.0f,
		1.0f);
	mat = mat.Transpose();

	// 投影行列の参照を取得し、ビュー変換行列を代入する
	RenderParam& param = renderer.GetParameter();
	param.SetProjection(mat);
}

void Camera::SetOrthographic()
{
	isOrthographic = true;
	UpdateOrthographic(0);
}

void Camera::SetPerspective()
{
	isOrthographic = false;
	UpdatePerspective(0);
}

void Camera::SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos)
{
	focusPos = _focusPos;
}
