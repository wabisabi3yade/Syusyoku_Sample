#include "pch.h"
#include "CameraMove.h"
#include "GameObject.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;
void CameraMove::UpdatePosition()
{
}

void CameraMove::UpdateLook()
{
	// 注視点の目標座標
	Vector3 lookTarget = pPlayer->transform.position + Vector3::Up * lookOffsetY;

	// 線形補間で現在の注視点を目標座標に近づける
	lookPos = Vector3::Lerp(lookPos, lookTarget, lookSpeed * MainApplication::DeltaTime());

	/*Transform& t = GetTransform();
	t.LookAt(lookPos);*/
}

void CameraMove::Init()
{
	name = "CameraMove";

	camera = static_cast<Camera*>(gameObject);	// カメラを取得する
	nowHeight = 7.0f;	//	現在の高さ
	horizonDistance = 10.0f;
	lookSpeed = 3.0f;
	lookOffsetY = 1.0f;
	moveSpeed = 5.0f;
	playDistance = 1.0f;
}
void CameraMove::LateUpdate()
{
	UpdatePosition();
	UpdateLook();
}

void CameraMove::ImGuiSetting()
{
	ImGui::Text("UpdatePosition");
	ImGui::DragFloat("nowHeight", &nowHeight);
	ImGui::DragFloat("moveSpeed", &moveSpeed);
	ImGui::DragFloat("playDistance", &playDistance);

	ImGui::Text("UpdateLook");
	ImGui::DragFloat("lookSpeed", &lookSpeed);
	ImGui::DragFloat("lookOffsetY", &lookOffsetY);
	ImGui::DragFloat("lookPlay", &lookPlayDistance);
}