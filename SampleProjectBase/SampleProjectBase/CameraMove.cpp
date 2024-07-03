#include "pch.h"
#include "CameraMove.h"
#include "GameObject.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;


void CameraMove::UpdateVector()
{
	Transform transform = GetTransform();

	InputClass& input = MainApplication::GetInput();

	// 座標移動
	Vector2 inputL = input.GetValue("Left");

	// 移動方向を求める
	moveVec = Vector3::Zero;
	moveVec += transform.Forward() * inputL.y;
	moveVec += transform.Right() * inputL.x;

	// 上昇・下降
	if (input.GetGamePad().ButtonPress(GamePad::Button::Sankaku) || input.GetKeyboard().GetKey(DIK_SPACE))
		moveVec += transform.Up();
	if (input.GetGamePad().ButtonPress(GamePad::Button::Batsu) || input.GetKeyboard().GetKey(DIK_LCONTROL))
		moveVec -= transform.Up();
	moveVec.Normalize();

	//　視点移動
	Vector2 inputR = input.GetValue("Right");
	

	rotateVec.y = inputR.x;
	rotateVec.x = -inputR.y;
	rotateVec.z = -inputR.y;

}

void CameraMove::Move()
{
	Transform& transform = GetTransform();

	transform.position += moveVec * moveSpeed * MainApplication::DeltaTime();

	transform.rotation += rotateVec * lookSpeed * MainApplication::DeltaTime();
}


void CameraMove::Init()
{
	name = "CameraMove";

	camera = static_cast<Camera*>(gameObject);	// カメラを取得する

	moveSpeed = 5.0f;
	lookSpeed = 180.0f;
}

void CameraMove::LateUpdate()
{
	UpdateVector();
	Move();
}

void CameraMove::ImGuiSetting()
{
	ImGui::DragFloat("moveSpeed", &moveSpeed);
	ImGui::DragFloat("lookSpeed", &lookSpeed);
}