#include "pch.h"
#include "PlayerMoveState.h"

#include "CP_Camera.h"
#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

PlayerMoveState::PlayerMoveState()
	: PlayerActState_Base(StateType::Move), currentSpeed(0.0f), maxSpeed(10.0f), acceleration(40.0f), rotateSpeed(7.0f)
{
}

void PlayerMoveState::OnStart()
{
	
}

void PlayerMoveState::Update()
{
	Move();

	Rotation();

	if (GameInput::GetInstance()->GetButton(GameInput::ButtonType::Player_Attack))
		ChangeState(StateType::Attack);
}

void PlayerMoveState::OnEnd()
{
}

void PlayerMoveState::ImGuiSetting()
{
	if (!ImGuiMethod::TreeNode("Move")) return;

	std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
	ImGui::Text(text.c_str()); moveVector;

	text = TO_UTF8("vec");
	ImGui::Text(text.c_str()); ImGui::SameLine();
	ImGuiMethod::Text(moveVector);

	text = TO_UTF8("input x:") + std::to_string(InputValue().x);
	ImGui::Text(text.c_str()); 
	text = TO_UTF8("input y:") + std::to_string(InputValue().y);
	ImGui::Text(text.c_str());
	ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);

	ImGui::DragFloat("acceleration", &acceleration, 0.1f);

	ImGui::DragFloat("rotateSpeed", &rotateSpeed, 0.1f);

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
	moveVector = camRightVec * input.x;
	moveVector += camForwardVec * input.y;
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
	Vector3 pos = pPlayerObject->GetTransform().GetPosition();
	pos += moveSpeed * MainApplication::DeltaTime();
	pPlayerObject->GetTransform().SetPosition(pos);

	// アニメーションのブレンド割合をセット
	//pAnimController->SetMoveSpeedRatio(currentSpeed / maxSpeed);
}

void PlayerMoveState::Rotation()
{
	if (!IsMoveInput()) return;

	// 入力方向へ向ける回転量を求める
	Quaternion targetRotation = Quat::RotateToVector(moveVector);

	// 現在の回転量を球面線形補間で向けていく。
	Quaternion rotation = pPlayerObject->GetTransform().GetRotation();
	rotation = Quaternion::Slerp(rotation, targetRotation, rotateSpeed * MainApplication::DeltaTime());
	pPlayerObject->GetTransform().SetRotation(rotation);
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
