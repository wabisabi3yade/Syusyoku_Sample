#include "pch.h"
#include "PlayerMove.h"
#include "GameObject.h"
#include "Geometory.h"

using namespace DirectX::SimpleMath;

constexpr float MOVE_SPEED(10.0f);

void PlayerMove::Input()
{
	GamePad* pad = MainApplication::GamePad();

	moveVec = Vector3::Zero;

	moveVec.x = pad->GetValue(GamePad::Value::StickL_X);
	moveVec.z = pad->GetValue(GamePad::Value::StickL_Y);
}

void PlayerMove::Init()
{
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
	if (ImGui::TreeNode("PlayerMove"))
	{
		ImGui::DragFloat("moveSpeed", &moveSpeed);

		ImGui::TreePop();
	}
}
