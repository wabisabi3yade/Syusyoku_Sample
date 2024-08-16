#include "pch.h"
#include "PlayerMoveState.h"

#include "CP_Camera.h"
#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

PlayerMoveState::PlayerMoveState(PlayerActionController& _controller)
	: PlayerActState_Base(_controller), currentSpeed(0.0f), maxSpeed(10.0f), acceleration(15.0f), rotateSpeed(2.0f)
{
}

void PlayerMoveState::Init()
{
	using enum PlayerAnimController::AnimType;
	ChangeAnimation((u_int)Move);
}

void PlayerMoveState::Update()
{
	Move();


	Rotation();
}

void PlayerMoveState::Terminal()
{
}

void PlayerMoveState::ImGuiSetting()
{
	if (!ImGui::TreeNode("Move")) return;

	std::string text = TO_UTF8("速度") + std::to_string(currentSpeed);
	ImGui::Text(text.c_str());

	ImGui::DragFloat(TO_UTF8("加速度"), &acceleration, 0.1f);

	ImGui::DragFloat(TO_UTF8("回転速度"), &rotateSpeed, 0.1f);

	ImGui::TreePop();
}

void PlayerMoveState::Move()
{
	float deltaTime = MainApplication::DeltaTime();

	CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();
	Vector3 camForwardVec = camera.GetTransform().Forward();
	Vector3 camRightVec = camera.GetTransform().Right();

	Vector2 input = InputValue();

	// 移動方向決定
	moveVector.x = camRightVec.x * input.x;
	moveVector.z = camForwardVec.z * input.y;
	moveVector.y = 0.0f;
	moveVector.Normalize();

	// 速度決定
	if (IsMoveInput())	// 入力されていたら
	{
		currentSpeed += acceleration * deltaTime;

		if (currentSpeed > maxSpeed)
			currentSpeed = maxSpeed;

	}
	else
	{
		currentSpeed = 0.0f;
	}


	Vector3 moveSpeed = moveVector * currentSpeed;

	// 移動
	Vector3 pos = pPlayerObject->transform.GetPosition();
	pos += moveSpeed * MainApplication::DeltaTime();
	pPlayerObject->transform.SetPosition(pos);

	pAnimController->SetMoveSpeedRatio(currentSpeed / maxSpeed);
}

void PlayerMoveState::Rotation()
{
	if (!IsMoveInput()) return;

	// 入力方向へ向ける回転量を求める
	Quaternion targetRotation = Quat::RotateToVector(moveVector);

	// 現在の回転量を球面線形補間で向けていく。
	Quaternion rotation = pPlayerObject->transform.GetRotation();
	rotation = Quaternion::Slerp(rotation, targetRotation, rotateSpeed * MainApplication::DeltaTime());
	pPlayerObject->transform.SetRotation(rotation);
}

Vector2 PlayerMoveState::InputValue()
{
	GameInput* input = GameInput::GetInstance();
	return input->GetValue(GameInput::ValueType::Player_Move);
}

bool PlayerMoveState::IsMoveInput()
{
	Vector2 absInput = Vec2::Abs(InputValue());
	if (absInput.x < Mathf::smallValue && absInput.y < Mathf::smallValue)
		return false;

	return true;
}

void PlayerMoveState::TransitionCheck()
{
	using enum PlayerActionController::State;
	u_int stateNum = static_cast<u_int>(Attack);

	ButtonChangeState(GameInput::ButtonType::Player_Attack, stateNum);
}
