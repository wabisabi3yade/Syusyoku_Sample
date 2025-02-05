#include "pch.h"
#include "BossIdleState.h"
#include "BossActionController.h"

namespace HashiTaku
{
	BossIdleState::BossIdleState() : nextState(BossState::Idle), isDecideNextState(false)
	{
	}

	json BossIdleState::Save()
	{
		auto data = BossActState_Base::Save();

#ifdef EDIT
		data["isIdleOnly"] = isIdleOnly;
#endif // EDIT
		return data;
	}

	void BossIdleState::Load(const json& _data)
	{
#ifdef EDIT
		LoadJsonBoolean("isIdleOnly", isIdleOnly, _data);
#endif // EDIT
	}

	void BossIdleState::OnStartBehavior()
	{
		isDecideNextState = false;
	}

	void BossIdleState::UpdateBehavior()
	{
#ifdef EDIT
		if (isIdleOnly) return;
#endif
		using ActDis = BossActionController::ActDistance;

		Transform* pPlayerTrans = GetPlayerTransform();
		if (!pPlayerTrans) return;

		BossActionController& bossActCon = GetBossActionController();

		// 距離を求める
		DXSimp::Vector3 playerToDistance =
			pPlayerTrans->GetPosition() - GetMyTransform().GetPosition();
		float playerToLength = playerToDistance.Length();

		// どの距離か取得
		ActDis curDisType = bossActCon.JudgeActDistance(playerToLength);

		// 次の状態をランダムで受け取る
		nextState = bossActCon.RandState(curDisType);

		// 決まったことを伝える
		isDecideNextState = true;

		HASHI_DEBUG_LOG(std::string(magic_enum::enum_name<BossState>(nextState)));
	}

	void BossIdleState::TransitionCheckUpdate()
	{
		BossActState_Base::TransitionCheckUpdate();

		// 次の状態が決まってるならその状態にする
		if (!isDecideNextState) return;
		ChangeState(nextState);
	}

	void BossIdleState::ImGuiDebug()
	{
#ifdef EDIT
		BossActState_Base::ImGuiDebug();

		ImGui::Checkbox("IdleOnly", &isIdleOnly);
#endif // EDIT
	}
}