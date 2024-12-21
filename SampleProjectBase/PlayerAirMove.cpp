#include "pch.h"
#include "PlayerAirMove.h"
#include "PlayerAirActionController.h"

namespace HashiTaku
{
	PlayerAirMove::PlayerAirMove() :
		currentSpeed(0.0f),
		maxSpeed(10.0f),
		acceleration(40.0f),
		vectorRotateSpeed(7.0f),
		decadeSpeed(2.0f),
		enterJumpSpeed(0.0f),
		maxJumpUpTime(0.0f)
	{
	}

	json PlayerAirMove::Save()
	{
		auto data = PlayerActState_Base::Save();

		data["maxSpeed"] = maxSpeed;
		data["acceleration"] = acceleration;
		data["decade"] = decadeSpeed;
		data["rotateSpeed"] = vectorRotateSpeed;

		return data;
	}

	void PlayerAirMove::Load(const json& _data)
	{
		using namespace HashiTaku;

		PlayerActState_Base::Load(_data);

		LoadJsonFloat("maxSpeed", maxSpeed, _data);
		LoadJsonFloat("acceleration", acceleration, _data);
		LoadJsonFloat("decade", decadeSpeed, _data);
		LoadJsonFloat("rotateSpeed", vectorRotateSpeed, _data);
	}

	void PlayerAirMove::OnStartBehavior()
	{
		// ステート開始時のジャンプ速度を取得
		enterJumpSpeed = GetRB().GetVelocity().y;
		HASHI_DEBUG_LOG(std::to_string(enterJumpSpeed));

		moveVector = DXSimp::Vector3::Zero;
	}

	void PlayerAirMove::UpdateBehavior()
	{
		Move();

		//	Rotation();
	}

	void PlayerAirMove::Move()
	{
		float deltaSpeed = GetDeltaSpeed();

		// 移動方向・移動量決定
		Transform& camTransform = pActionController->GetCamera().GetTransform();
		DXSimp::Vector3 camForwardVec = camTransform.Forward();
		DXSimp::Vector3 camRightVec = camTransform.Right();
		DXSimp::Vector2 input = GetInputLeftStick();

		// 傾きの大きさを取得
		float inputMagnitude = std::min(input.Length(), 1.0f);

		// 移動方向を求める
		if (inputMagnitude >= Mathf::epsilon)
		{
			moveVector = camRightVec * input.x;
			moveVector += camForwardVec * input.y;
			moveVector.y = 0.0f;
			moveVector.Normalize();
		}

		// 移動速度を求める
		float curMaxSpd = maxSpeed * inputMagnitude;
		if (currentSpeed > curMaxSpd)
		{
			// 減速率をかける
			currentSpeed -= decadeSpeed * DeltaTime();
			currentSpeed = std::max(currentSpeed, curMaxSpd);
		}
		else
		{
			// 加速
			currentSpeed += acceleration * inputMagnitude;
			currentSpeed = std::min(currentSpeed, curMaxSpd);
		}

		DXSimp::Vector3 moveSpeed = moveVector * currentSpeed * deltaSpeed;

		CP_RigidBody& rb = GetRB();
		moveSpeed.y = rb.GetVelocity().y;

		// 移動
		rb.SetVelocity(moveSpeed);

		// アニメーションのブレンド割合をセット
		pActionController->SetAnimationFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);
	}

	void PlayerAirMove::Rotation()
	{
		//if (!isStickInput) return;	// スティック入力されていないなら

		Transform& transform = GetMyTransform();

		// 入力方向へ向ける回転量を求める
		DXSimp::Quaternion targetRotation = Quat::RotateToVector(moveVector);

		// 現在の回転量を球面線形補間で向けていく。
		DXSimp::Quaternion rotation = transform.GetRotation();
		rotation = DXSimp::Quaternion::Slerp(rotation, targetRotation, vectorRotateSpeed * DeltaTime());
		transform.SetRotation(rotation);
	}

	void PlayerAirMove::ImGuiDebug()
	{
		PlayerAirState::ImGuiDebug();

		std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
		ImGui::Text(text.c_str());
		ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("acceleration", &acceleration, 0.1f);
		ImGui::DragFloat("decadeTimes", &decadeSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("rotateSpeed", &vectorRotateSpeed, 0.1f);
	}
}