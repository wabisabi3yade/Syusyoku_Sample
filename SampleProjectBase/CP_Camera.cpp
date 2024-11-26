#include "pch.h"
#include "CP_Camera.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

// デフォルト値
constexpr float DEFAULT_FOV = 45.0f;
constexpr float NEAR_Z = 0.01f;
constexpr float DEFAULT_DISTANCE = 1000.0f;

CP_Camera::CP_Camera() :fov(DEFAULT_FOV), distance(DEFAULT_DISTANCE), viewPortSlot(0)
{	
}

void CP_Camera::Init()
{
	// メインカメラに設定する
	InSceneSystemManager::GetInstance()->SetCamera(*this);

	SetPerspective();	// 透視投影から始める
}

void CP_Camera::OnDestroy()
{
	// メインカメラから外す
	InSceneSystemManager::GetInstance()->DeleteCamera(*this);
}

const DirectX::SimpleMath::Vector3& CP_Camera::GetCamUp() const
{
	return camUp;
}

const DirectX::SimpleMath::Vector3& CP_Camera::GetEyePos() const
{
	return GetTransform().GetPosition();
}

const DirectX::SimpleMath::Vector3& CP_Camera::GetTarget() const
{
	return target;
}

float CP_Camera::GetFov() const
{
	return fov;
}

void CP_Camera::ImGuiDebug()
{
	ImGui::Text("Right");
	ImGuiMethod::Text(GetTransform().Right());
	ImGui::Text("Up");
	ImGuiMethod::Text(GetTransform().Up());
	ImGui::Text("Forward");
	ImGuiMethod::Text(GetTransform().Forward());

	bool isChangeFov = false;
	bool isChangeDis = false;
	isChangeFov = ImGui::DragFloat("fov", &fov, 1.0f, 15.0f, 110.0f);
	isChangeDis = ImGui::DragFloat("distance", &distance, 1.0f, 0.1f, 2000.0f);

	if (isChangeFov || isChangeDis)
	{
		UpdatePerspective();
	}
}

void CP_Camera::UpdateViewMatrix()
{
	// 注視点を求める
	target = GetTransform().GetPosition() + GetTransform().Forward();

	// ビュー変換行列を求める
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().GetPosition(),		// カメラ座標
		target,	// 注視点
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

void CP_Camera::SetFov(float _setFov)
{
	fov = _setFov;
	UpdatePerspective();
}

void CP_Camera::SetViewportSlot(u_int _slot)
{
	viewPortSlot = _slot;
}

nlohmann::json CP_Camera::Save()
{
	auto data = Component::Save();

	data["fov"] = fov;
	data["distance"] = distance;
	data["viewSlot"] = viewPortSlot;

	return data;
}

void CP_Camera::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonFloat("fov", fov, _data);
	LoadJsonFloat("distance", distance, _data);
	LoadJsonUnsigned("viewSlot", viewPortSlot, _data);
}

void CP_Camera::UpdateProjection()
{
	if (isOrthographic)
		UpdateOrthographic();
	else
		UpdatePerspective();
}

void CP_Camera::UpdatePerspective()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ビューポートを取得
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	float nearZ = NEAR_Z;
	float farZ = nearZ + distance;

	// ビュー変換行列を作成する
	Matrix mat = DirectX::XMMatrixPerspectiveFovLH(
		fov * Mathf::degToRad,
		screenWidth / screenHeight,   // アスペクト比
		nearZ,	// 描画最近
		farZ);	// 描画最遠

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
