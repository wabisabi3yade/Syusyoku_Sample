#include "pch.h"
#include "PlayerMoveState.h"
#include "PlayerGroundActionController.h"
#include "CP_Camera.h"
#include "CP_Player.h"
#include "GameInput.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	constexpr float IDLE_ANIM_PLAYSPEED(1.0f);

	PlayerMoveState::PlayerMoveState()
		: currentSpeed(0.0f), maxSpeed(10.0f), acceleration(40.0f), rotateSpeed(7.0f), decadeSpeed(2.0f)
	{
	}

	json PlayerMoveState::Save()
	{
		auto data = PlayerActState_Base::Save();

		data["maxSpeed"] = maxSpeed;
		data["acceleration"] = acceleration;
		data["decade"] = decadeSpeed;
		data["rotateSpeed"] = rotateSpeed;

		return data;
	}

	void PlayerMoveState::Load(const json& _data)
	{
		using namespace HashiTaku;

		PlayerActState_Base::Load(_data);

		LoadJsonFloat("maxSpeed", maxSpeed, _data);
		LoadJsonFloat("acceleration", acceleration, _data);
		LoadJsonFloat("decade", decadeSpeed, _data);
		LoadJsonFloat("rotateSpeed", rotateSpeed, _data);
	}

	void PlayerMoveState::UpdateBehavior()
	{
		ApplyRootMotion();
		
		Move();

		Rotation();
	}

	void PlayerMoveState::OnEndBehavior()
	{
	}

	void PlayerMoveState::TransitionCheckUpdate()
	{
		PlayerGroundState::TransitionCheckUpdate();

		if (currentSpeed <= Mathf::epsilon)	// 移動速度が0以下になると
		{
			ChangeState(PlayerState::Idle);
		}
	}

	bool PlayerMoveState::IsRunning()
	{
		if (currentSpeed >= Mathf::epsilon)  return true;

		return false;
	}

	void PlayerMoveState::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
		ImGui::Text(text.c_str());
		ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("acceleration", &acceleration, 0.1f);
		ImGui::DragFloat("decadeTimes", &decadeSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("rotateSpeed", &rotateSpeed, 0.1f);
	}

	void PlayerMoveState::Move()
	{
		float deltaSpeed = GetDeltaSpeed();

		// 移動方向・移動量決定
		Transform& camTransform = GetGroundController().GetCamera().GetTransform();
		DXSimp::Vector3 camForwardVec = camTransform.Forward();
		DXSimp::Vector3 camRightVec = camTransform.Right();
		DXSimp::Vector2 input = GetInputLeftStick();

		// 傾きの大きさを取得
		float inputMagnitude = std::min(input.Length(), 1.0f);

		// 移動方向を求める
		if (inputMagnitude > 0.0f)
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

		// y軸はそのまま
		IActionController& actCon = GetActionController();
		moveSpeed.y = actCon.GetVelocity().y;

		// 移動
		actCon.SetVelocity(moveSpeed);

		// アニメーションのブレンド割合をセット
		actCon.SetAnimationFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);
	}

	void PlayerMoveState::ApplyRootMotion()
	{
		// ルートモーションと移動速度から移動速度の再生速度を調整する
		if (IsRunning())
		{
			CP_Animation* pAnimation = GetActionController().GetAnimation();
			if (!pAnimation) return;

			float rootMotion = abs(pAnimation->GetMotionPosSpeedPerSec().z);

			if (rootMotion > Mathf::epsilon)
			{
				float animPlaySpeed = currentSpeed / rootMotion;

				pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
			}
		}
	}

	void PlayerMoveState::Rotation()
	{
		if (!IsMoveInput()) return;

		GameObject& playerObj = GetPlayer().GetGameObject();

		// 入力方向へ向ける回転量を求める
		DXSimp::Quaternion targetRotation = Quat::RotateToVector(moveVector);

		// 現在の回転量を球面線形補間で向けていく。
		DXSimp::Quaternion rotation = playerObj.GetTransform().GetRotation();
		rotation = DXSimp::Quaternion::Slerp(rotation, targetRotation, rotateSpeed * DeltaTime());
		playerObj.GetTransform().SetRotation(rotation);
	}

	bool PlayerMoveState::IsMoveInput()
	{
		DXSimp::Vector2 absInput = Vec2::Abs(GetInputLeftStick());
		if (absInput.x < Mathf::smallValue && absInput.y < Mathf::smallValue)
			return false;

		return true;
	}
}