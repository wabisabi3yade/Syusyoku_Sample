#include "pch.h"
#include "PlayerAirSlashHigh.h"

namespace HashiTaku
{
	constexpr const char* AIR_SLASHHIGH_ANIMNAME("SlashHigh");

	PlayerAirSlashHigh::PlayerAirSlashHigh() :
		maxJumpMovement(5.0f),
		prevJumpMovement(0.0f)
	{
		SetIsApplyDownForce(false);
	}

	json PlayerAirSlashHigh::Save()
	{
		json data = PlayerAirState::Save();

		data["maxJumpMovement"] = maxJumpMovement;
		data["jumpMovementCurve"] = jumpMovementCurve.Save();

		return data;
	}

	void PlayerAirSlashHigh::Load(const json& _data)
	{
		PlayerAirState::Load(_data);

		LoadJsonFloat("maxJumpMovement", maxJumpMovement, _data);
		json curveData;
		if (LoadJsonData("jumpMovementCurve", curveData, _data))
		{
			jumpMovementCurve.Load(curveData);
		}
	}

	void PlayerAirSlashHigh::OnStartBehavior()
	{
		PlayerAirState::OnStartBehavior();

		prevJumpMovement = 0.0f;
	}

	void PlayerAirSlashHigh::UpdateBehavior()
	{
		PlayerAirState::UpdateBehavior();

		// 上昇移動の処理
		MoveUpdate();
	}

	void PlayerAirSlashHigh::OnAnimationEnd(const std::string& _fromAnimNodeName,
		const std::string& _toAnimNodeName)
	{
		// アニメーションが終われば移動へ
		if (_fromAnimNodeName == AIR_SLASHHIGH_ANIMNAME)
			ChangeState(PlayerState::Move);
	}

	void PlayerAirSlashHigh::MoveUpdate()
	{
		// 現在のジャンプの移動量を取得
		float curAnimRatio = GetAnimation()->GetCurrentPlayRatio();
		float curJumpMovement = jumpMovementCurve.GetValue(curAnimRatio) * maxJumpMovement;

		// 前回から移動量を進める
		float deltaTime = std::max(DeltaTime(), Mathf::epsilon);
		float curVelocity = (curJumpMovement - prevJumpMovement) / deltaTime;
		// 速度がマイナスなら
		if (curVelocity < Mathf::epsilon)
			curVelocity = 0.0f;

		// 速度を設定
		SetVelocity(Vec3::Up * curVelocity);

		// 次フレームの為に更新
		prevJumpMovement = curJumpMovement;
	}

	void PlayerAirSlashHigh::ImGuiDebug()
	{
		PlayerAirState::ImGuiDebug();

		ImGui::DragFloat("MaxMovement", &maxJumpMovement, 0.1f, 0.0f, 1000.0f);
		jumpMovementCurve.ImGuiCall();
	}
}

