#include "pch.h"
#include "CP_Camera.h"

#include "GameObject.h"

using namespace DirectX::SimpleMath;

// デフォルト値
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

CP_Camera::CP_Camera() : viewPortSlot(0)
{
	name = "Camera";

	// 初期値代入
	fov = DEFAULT_FOV;
	nearZ = DEFAULT_NEARZ;
	farZ = DEFAULT_FARZ;
	
	SetPerspective();	// 透視投影から始める
}

void CP_Camera::Init()
{
	GetTransform().position.y = 2.0f;
	GetTransform().position.z = -5.0f;

	//GetTransform().position.y = 200.0f;
	//GetTransform().position.z = 400.0f;
	GetTransform().SetEularAngles(Vector3(10.0f, 0.0f, 0.0f));
}

void CP_Camera::LateUpdate()
{
	UpdateViewMatrix();

	if (isOrthographic)
		UpdateOrthographic();
	else
		UpdatePerspective();
}

void CP_Camera::ImGuiSetting()
{
	ImGui::DragFloat("fov", &fov, 1.0f, 15.0f, 180.0f);
}

void CP_Camera::UpdateViewMatrix()
{
	focusPos = GetTransform().position + GetTransform().Forward();

	if (Vector3::Distance(GetTransform().position, focusPos) < Mathf::smallValue)
	{
		focusPos.z += Mathf::smallValue;
	}

	// ビュー変換行列を求める
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().position,		// カメラ座標
		focusPos,	// 注視点
		Vector3::Up // 上ベクトル
	);
	viewMatrix = viewMatrix.Transpose();

	// ビュー変換行列をセット
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void CP_Camera::SetPerspective()
{
	isOrthographic = false;
	UpdatePerspective();
}

void CP_Camera::SetOrthographic()
{
	isOrthographic = true;
	UpdateOrthographic();
}

void CP_Camera::SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos)
{
	focusPos = _focusPos;
}

void CP_Camera::SetViewportSlot(u_int _slot)
{
	viewPortSlot = _slot;
}

void CP_Camera::UpdatePerspective()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ビューポートを取得
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(viewPortSlot);
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

void CP_Camera::UpdateOrthographic()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ビューポートを取得
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(viewPortSlot);
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
