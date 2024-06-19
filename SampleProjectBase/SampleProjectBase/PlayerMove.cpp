#include "pch.h"
#include "PlayerMove.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

constexpr float MOVE_SPEED(10.0f);

void PlayerMove::Input()
{
	GamePad* pad = MainApplication::GamePad();

	moveVec = Vector3::Zero;

	if (pad->ButtonPress(GamePad::Button::Arrow_Up))
	{
		moveVec.y = 1.0f;
	}
	if (pad->ButtonPress(GamePad::Button::Arrow_Down))
	{
		moveVec.y = -1.0f;
	}
	if (pad->ButtonPress(GamePad::Button::Arrow_Right))
	{
		moveVec.x = 1.0f;
	}
	if (pad->ButtonPress(GamePad::Button::Arrow_Left))
	{
		moveVec.x = -1.0f;
	}
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
