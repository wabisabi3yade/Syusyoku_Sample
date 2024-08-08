#include "pch.h"
#include "CP_PlayerMove.h"
#include "GameObject.h"
#include "Geometory.h"
#include "GameMode.h"
#include "Camera.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

constexpr float MOVE_SPEED(10.0f);

void CP_PlayerMove::Move()
{
	CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();

	Vector3 cameraVec = camera.GetGameObject().transform.Forward();

	// カメラを考慮した移動方向を決定する
	moveVec = Vector3::Zero;

	Vector2 input = MainApplication::GetInput().GetValue("Left");

	moveVec.x = cameraVec.x * input.x;
	moveVec.z = cameraVec.z * input.y;
	moveVec.y = 0.0f;
	moveVec.Normalize();

	// 座標を更新
	gameObject->transform.position += moveVec * moveSpeed * MainApplication::DeltaTime();
}

void CP_PlayerMove::Init()
{
	name = "PlayerMove";
	moveSpeed = MOVE_SPEED;

	camera = &GameMode::GetInstance()->GetCamera();
}

void CP_PlayerMove::Update()
{
	Move();	// 移動
}

void CP_PlayerMove::ImGuiSetting()
{
	ImGui::DragFloat("moveSpeed", &moveSpeed);
}