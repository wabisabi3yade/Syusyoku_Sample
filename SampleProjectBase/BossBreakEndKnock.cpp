#include "pch.h"
#include "BossBreakEndKnock.h"
#include "CP_Boss.h"

namespace HashiTaku
{
	constexpr auto ANIMATION_NAME("BreakEndKnock");	// ブレイク終了のノックアニメーション名

	BossBreakEndKnock::BossBreakEndKnock() :
		canBreakValue(10.0f),
		knockDistance(4.0f)
	{
	}

	float BossBreakEndKnock::GetCanBreakValue() const
	{
		return canBreakValue;
	}

	json BossBreakEndKnock::Save()
	{
		auto data = BossDamageState::Save();
		data["canBreak"] = canBreakValue;
		data["knockDistance"] = knockDistance;
		return data;
	}

	void BossBreakEndKnock::Load(const json& _data)
	{
		BossDamageState::Load(_data);

		LoadJsonFloat("canBreak", canBreakValue, _data);
		LoadJsonFloat("knockDistance", knockDistance, _data);
	}

	void BossBreakEndKnock::OnStartBehavior()
	{
		BossDamageState::OnStartBehavior();

		// ノックの座標を求めてワープモーションに渡す
		CalcKnockTargetPos();
	}

	void BossBreakEndKnock::CalcKnockTargetPos()
	{
		// ブレイク値をリセット
		GetBossActionController().GetBoss().SetBreakValue(0.0f);

		Transform& transform = GetMyTransform();
		DXSimp::Vector3 targetPos =
			transform.GetPosition() - transform.Forward() * knockDistance;

		// ワープモーションの距離を設定
		SetWarpTargetPos(targetPos);
	}

	void BossBreakEndKnock::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{
		// アニメーション終了すると待機
		if (_fromAnimNodeName == ANIMATION_NAME)
		{
			ChangeState(BossState::Idle);
		}
	}

	void BossBreakEndKnock::ImGuiDebug()
	{
		BossDamageState::ImGuiDebug();

		ImGui::DragFloat("CanBreakEnd", &canBreakValue, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("KnockDis", &knockDistance, 0.01f, 0.0f, 100.0f);
	}
}