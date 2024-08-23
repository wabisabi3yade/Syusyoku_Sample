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

void CP_Camera::Awake()
{
	Vector3 initPos = { 0.0f, 2.0f, -5.0f };
	GetTransform().SetPosition(initPos);

	//GetTransform().position.y = 200.0f;
	//GetTransform().position.z = 400.0f;
	GetTransform().SetEularAngles(Vector3(10.0f, 0.0f, 0.0f));

	// メインカメラに設定する
	InSceneSystemManager::GetInstance()->SetCamera(*this);

	SetPerspective();	// 透視投影から始める
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
	ImGui::Text("Right");
	ImGuiMethod::Text(GetTransform().Right());
	ImGui::Text("Up");
	ImGuiMethod::Text(GetTransform().Up());
	ImGui::Text("Forward");
	ImGuiMethod::Text(GetTransform().Forward());

	ImGui::DragFloat("fov", &fov, 1.0f, 15.0f, 180.0f);
	ImGui::DragFloat("distance", &distance, 1.0f, 0.1f, 2000.0f);
}

void CP_Camera::UpdateViewMatrix()
{
	Vector3 focusPos = GetTransform().GetPosition() + GetTransform().Forward();

	if (Vector3::Distance(GetTransform().GetPosition(), focusPos) < Mathf::smallValue)
	{
		focusPos.z += Mathf::smallValue;
	}

	// ビュー変換行列を求める
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().GetPosition(),		// カメラ座標
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

void CP_Camera::SetViewportSlot(u_int _slot)
{
	viewPortSlot = _slot;
}

nlohmann::json CP_Camera::Save()
{
	auto data = Component::Save();

	data["fov"] = fov;
	data["distance"] = distance;
	data["isOrth"] = isOrthographic;
	data["viewSlot"] = viewPortSlot;

	return data;
}

void CP_Camera::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonFloat("fov", fov, _data);
	LoadJsonFloat("distance", distance, _data);
	LoadJsonBoolean("isOrth", isOrthographic, _data);
	LoadJsonUnsigned("viewSlot", viewPortSlot, _data);
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
		fov,
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
