#include "pch.h"
#include "CameraMove.h"
#include "GameObject.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

void CameraMove::Roll(Vector2 _input)
{
	// 横に回転する
	centerAngle += -_input.x * rotateSpeed * MainApplication::DeltaTime();
	centerAngle = std::fmod(centerAngle, Mathf::roundDeg);	// 角度を正規化

	// 高さを変える
	nowHeight += -_input.y * heightSpeed * MainApplication::DeltaTime();
	nowHeight = std::clamp(nowHeight, minHeight, maxHeight);	// 範囲内におさめる
}

void CameraMove::UpdatePosition()
{
	// カメラ座標の目標座標を求める
	Vector3 posTarget = pPlayer->transform.position;
	float rad = DirectX::XMConvertToRadians(centerAngle - 90.0f);
	// XZ平面は中心との角度から距離を求める
	posTarget.x += cos(rad) * distance;
	posTarget.z += sin(rad) * distance;
	// プレイヤーからの一定の高さを保つ
	posTarget.y += nowHeight;

	// カメラの座標を更新する
	// 目標座標に線形補間を使用して近づけていく
	Transform& t = GetTransform();
	t.position = Vector3::Lerp(t.position, posTarget, moveSpeed * MainApplication::DeltaTime());
}

void CameraMove::UpdateLook()
{
	// 注視点の目標座標
	Vector3 lookTarget = pPlayer->transform.position + Vector3::Up * lookOffsetY;

	// 線形補間で現在の注視点を目標座標に近づける
	lookPos = Vector3::Lerp(lookPos, lookTarget, lookSpeed * MainApplication::DeltaTime());
	Transform& t = GetTransform();
	t.LookAt(lookPos);
}

void CameraMove::Init()
{
	name = "CameraMove";

	camera = static_cast<Camera*>(gameObject);	// カメラを取得する

	rotateSpeed = 360.0f;	// 回転速度
	heightSpeed = 10.0f;	// 高さ速度
	maxHeight = 3.0f;	// 高さの最大値（プレイヤーから）
	nowHeight = 1.0f;	//	現在の高さ
	lookSpeed = 3.0f;
	lookOffsetY = 1.0f;
	moveSpeed = 5.0f;
	distance = 15.0f;

}
void CameraMove::LateUpdate()
{
	UpdatePosition();

	UpdateLook();
}

void CameraMove::SetParameter()
{
	ImGui::Text("Roll");
	ImGui::DragFloat("rotateSpeed{", &rotateSpeed);
	ImGui::DragFloat("centerAngle", &centerAngle);
	ImGui::DragFloat(" heightSpeed", &heightSpeed);
	ImGui::DragFloat("minHeight", &minHeight);
	ImGui::DragFloat("nowHeight", &nowHeight);

	ImGui::Text("UpdatePosition");
	ImGui::DragFloat("moveSpeed", &moveSpeed);
	ImGui::DragFloat("playDistance", &playDistance);
	ImGui::DragFloat("distnce", &distance);

	ImGui::Text("UpdateLook");
	ImGui::DragFloat("lookSpeed", &lookSpeed);
	ImGui::DragFloat("lookOffsetY", &lookOffsetY);

}