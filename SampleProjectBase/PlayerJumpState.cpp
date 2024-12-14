#include "pch.h"
#include "PlayerJumpState.h"
#include "CP_Player.h"

constexpr auto JUMPTRIGGER_ANIMPARAM("jumpTrigger");

namespace DXSimp = HashiTaku::DXSimp;

PlayerJumpState::PlayerJumpState() :
	jumpPower(10.0f),
	crouchTime(0.3f),
	stateElapsedTime(0.0f),
	isAlreadyJump(false)
{
}

PlayerJumpState::~PlayerJumpState()
{
}

nlohmann::json PlayerJumpState::Save()
{
	auto data = PlayerGroundState::Save();

	data["jumpPower"] = jumpPower;
	data["crouchTime"] = crouchTime;

	return data;
}

void PlayerJumpState::Load(const nlohmann::json& _data)
{
	PlayerGroundState::Load(_data);

	HashiTaku::LoadJsonFloat("jumpPower", jumpPower, _data);
	HashiTaku::LoadJsonFloat("crouchTime", crouchTime, _data);
}

void PlayerJumpState::OnStartBehavior()
{
	// 入力方向にすぐに向ける
	LookInputVectorInstant();

	// ジャンプトリガーを入れる
	GetAnimation()->SetTrigger(JUMPTRIGGER_ANIMPARAM);

	GetPlayer().SetIsInvicible(true);

	stateElapsedTime = 0.0f;
	isAlreadyJump = false;
}

void PlayerJumpState::UpdateBehavior()
{
	if (isAlreadyJump) return;

	stateElapsedTime += DeltaTime();

	// かがむ時間を過ぎると上に飛ぶ
	if (stateElapsedTime > crouchTime)
	{
		OnBeginJump();
	}
}

void PlayerJumpState::OnEndBehavior()
{
	GetPlayer().SetIsInvicible(false);
}

void PlayerJumpState::OnBeginJump()
{
	isAlreadyJump = true;

	// 上に飛ぶ
	GetRB().AddImpulse(Vec3::Up * jumpPower);
}

void PlayerJumpState::LookInputVectorInstant()
{
	// カメラを考慮した入力方向のXZ成分
	DXSimp::Vector2 inputFromCam = GetInputLeftStickFromCam();

	// 長さが0なら
	if (inputFromCam.x < Mathf::epsilon && inputFromCam.y < Mathf::epsilon) return;

	// 入力方向の回転量を求める
	DXSimp::Quaternion targetRot =
		Quat::RotateToVector(DXSimp::Vector3(inputFromCam.x, 0.0f, inputFromCam.y));

	// プレイヤーをその方向に向ける
	GetMyTransform().SetRotation(targetRot);
}

void PlayerJumpState::ImGuiDebug()
{
	PlayerGroundState::ImGuiDebug();

	ImGui::DragFloat("JumpPower", &jumpPower, 0.01f, 0.0f, 10000.0f);
	ImGui::DragFloat("CrouchTime", &crouchTime, 0.001f, 0.0f, 10.0f);
}
