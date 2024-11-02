#include "pch.h"
#include "PlayerMoveState.h"

#include "CP_Camera.h"
#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

constexpr float IDLE_ANIM_PLAYSPEED(1.0f);

PlayerMoveState::PlayerMoveState()
	: currentSpeed(0.0f), maxSpeed(10.0f), acceleration(40.0f), rotateSpeed(7.0f), decadeSpeedTimes(0.98f)
{
}

nlohmann::json PlayerMoveState::Save()
{
	auto data =  PlayerActState_Base::Save();
	data["maxSpeed"] = maxSpeed;
	data["acceleration"] = acceleration;
	data["decade"] = decadeSpeedTimes;
	data["rotateSpeed"] = rotateSpeed;
	return data;
}

void PlayerMoveState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	LoadJsonFloat("maxSpeed", maxSpeed, _data);
	LoadJsonFloat("acceleration", acceleration, _data);
	LoadJsonFloat("decade", decadeSpeedTimes, _data);
	LoadJsonFloat("rotateSpeed", rotateSpeed, _data);
}

void PlayerMoveState::OnStartBehavior()
{

}

void PlayerMoveState::UpdateBehavior()
{
	Move();

	Rotation();

}

void PlayerMoveState::OnEndBehavior()
{
}

void PlayerMoveState::TransitionCheckUpdate()
{
	// アタックStateに遷移
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		ChangeState(StateType::Attack11);
	}
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_RockOn))
	{
		ChangeState(StateType::TargetMove);
	}
	else if (currentSpeed <= Mathf::epsilon)	// 移動速度が0以下になると
	{
		ChangeState(StateType::Idle);
	}
}

bool PlayerMoveState::IsRunning()
{
	if (currentSpeed > Mathf::epsilon)  return true;

	return false;
}

void PlayerMoveState::ImGuiSetting()
{
	curve.ImGuiCall();

	std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
	ImGui::Text(text.c_str());
	ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("acceleration", &acceleration, 0.1f);
	ImGui::DragFloat("decadeTimes", &decadeSpeedTimes, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("rotateSpeed", &rotateSpeed, 0.1f);
}

void PlayerMoveState::Move()
{
	float deltaTime = MainApplication::DeltaTime();

	// 移動方向・移動量決定
	Vector3 camForwardVec = pCamera->GetTransform().Forward();
	Vector3 camRightVec = pCamera->GetTransform().Right();
	Vector2 input = GetInputLeftStick();
	moveVector = camRightVec * input.x;
	moveVector += camForwardVec * input.y;
	moveVector.y = 0.0f;

	Vector3 moveSpeed = moveVector *  maxSpeed;
	currentSpeed = moveSpeed.Length();

	// 移動
	Vector3 pos = pPlayerObject->GetTransform().GetPosition();
	pos += moveVector * maxSpeed * MainApplication::DeltaTime();
	pPlayerObject->GetTransform().SetPosition(pos);

	// アニメーションのブレンド割合をセット
	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);

	// ルートモーションと移動速度から移動速度の再生速度を調整する
	if (IsRunning())
	{
		float rootMotion = abs(pAnimation->GetMotionPosSpeedPerSec().z);

		if (rootMotion > Mathf::epsilon)
		{
			float animPlaySpeed = currentSpeed / rootMotion;

			pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
		}
	}
	else
	{
		pAnimation->SetCurNodePlayerSpeed(IDLE_ANIM_PLAYSPEED);
	}

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

bool PlayerMoveState::IsMoveInput()
{
	Vector2 absInput = Vec2::Abs(GetInputLeftStick());
	if (absInput.x < Mathf::smallValue && absInput.y < Mathf::smallValue)
		return false;

	return true;
}
