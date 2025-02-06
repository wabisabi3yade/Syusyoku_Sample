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

		// ‹——£‚ğ‹‚ß‚é
		DXSimp::Vector3 playerToDistance =
			pPlayerTrans->GetPosition() - GetMyTransform().GetPosition();
		float playerToLength = playerToDistance.Length();

		// ‚Ç‚Ì‹——£‚©æ“¾
		ActDis curDisType = bossActCon.JudgeActDistance(playerToLength);

		// Ÿ‚Ìó‘Ô‚ğƒ‰ƒ“ƒ_ƒ€‚Åó‚¯æ‚é
		nextState = bossActCon.RandState(curDisType);

		// Œˆ‚Ü‚Á‚½‚±‚Æ‚ğ“`‚¦‚é
		isDecideNextState = true;

		HASHI_DEBUG_LOG(std::string(magic_enum::enum_name<BossState>(nextState)));
	}

	void BossIdleState::TransitionCheckUpdate()
	{
		BossActState_Base::TransitionCheckUpdate();

		// Ÿ‚Ìó‘Ô‚ªŒˆ‚Ü‚Á‚Ä‚é‚È‚ç‚»‚Ìó‘Ô‚É‚·‚é
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