#include "pch.h"
#include "PlayerMove.h"
#include "GameObject.h"
#include "Geometory.h"

using namespace DirectX::SimpleMath;

constexpr float MOVE_SPEED(10.0f);

void PlayerMove::Input()
{
	const GamePad& pad = MainApplication::GetInput().GetGamePad();

	moveVec = Vector3::Zero;

	moveVec.x = pad.GetValue(GamePad::Value::StickL_X);
	moveVec.z = pad.GetValue(GamePad::Value::StickL_Y);
}

void PlayerMove::Init()
{
	name = "PlayerMove";
	moveSpeed = MOVE_SPEED;
}

void PlayerMove::Update()
{
	Input();	// “ü—Í
	moveVec.Normalize();
	gameObject->transform.position += moveVec * moveSpeed * MainApplication::DeltaTime();
}

void PlayerMove::LateUpdate()
{
}

void PlayerMove::Draw()
{
}

void PlayerMove::SetParameter()
{
	ImGui::DragFloat("moveSpeed", &moveSpeed);
}
