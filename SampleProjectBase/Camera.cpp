#include "pch.h"
#include "Camera.h"

#include "GameObject.h"

using namespace DirectX::SimpleMath;

// デフォルト値
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

void Camera::Init()
{
	// 初期値代入
	focusPos = { 0, 0, 0 };
	fov = DEFAULT_FOV;
	nearZ = DEFAULT_NEARZ;
	farZ = DEFAULT_FARZ;
	
	/*GetTransform().position.y = 2.0f;
	GetTransform().position.z = -5.0f;*/

	GetTransform().position.y = 200.0f;
	GetTransform().position.z = 400.0f;
	GetTransform().rotation.x = 30.0f;
	GetTransform().rotation.y = 180.0f;
	SetPerspective();	// 透視投影から始める

	gameObject->AddComponent<CameraMove>();
}

void Camera::UpdateViewMatrix()
{
	focusPos = GetTransform().position + GetTransform().Forward() * 1.0f;

	if (Vector3::Distance(GetTransform().position, focusPos) < 0.0001f)
	{
		focusPos.z += 0.001f;
	}

	// ビュー変換行列を求める
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().position,		// カメラ座標
		focusPos,	// 注視点
		Vector3::Up // 上ベクトル
	);
	viewMatrix =viewMatrix.Transpose();

	// ビュー変換行列をセット
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void Camera::SetPerspective(u_int _viewPortSlot)
{
	isOrthographic = false;
	UpdatePerspective(_viewPortSlot);
}

void Camera::SetOrthographic(u_int _viewPortSlot)
{
	isOrthographic = true;
	UpdateOrthographic(_viewPortSlot);
}

void Camera::SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos)
{
	focusPos = _focusPos;
}

void Camera::UpdatePerspective(u_int _viewPortSlot)
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ビューポートを取得
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(_viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// ビュー変換行列を作成する
	Matrix mat = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		screenWidth / screenHeight,   // アスペクト比
		nearZ,
		farZ);
	mat = mat.Transpose();

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
