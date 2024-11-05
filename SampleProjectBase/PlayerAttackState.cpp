#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerActionController.h"
#include "GameObject.h"

PlayerAttackState::PlayerAttackState()
	: nextCombAtkState(PlayerState::None), senkoInputTime(0.15f)
{
	pAttackInfo = std::make_unique<HashiTaku::AttackInformation>();
}

void PlayerAttackState::OnStartBehavior()
{
	// 攻撃情報を更新
	UpdateAttackInfo();

	// 攻撃フラグを立てる
	pAnimation->SetTrigger(ATTACKTRIGGER_PARAMNAME);
}

void PlayerAttackState::UpdateBehavior()
{
}

void PlayerAttackState::OnEndBehavior()
{
}

void PlayerAttackState::TransitionCheckUpdate()
{
	// キャンセルできるか？
	if (!CheckCanCancel()) return;

	// 攻撃入力されたらステート遷移する
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack, senkoInputTime))
		ChangeState(nextCombAtkState);
}

void PlayerAttackState::UpdateAttackInfo()
{
	pPlayer->SetAttackInfo(*pAttackInfo);
}

void PlayerAttackState::ImGuiSetting()
{
	ImGui::DragFloat("Senko", &senkoInputTime, 0.05f, 0.0f, 3.0f);
	ImGuiComboPlayerState("Next", nextCombAtkState);

	ImGui::Text("AtkInfo");
	pAttackInfo->ImGuiCall();
}

bool PlayerAttackState::CheckCanCancel() const
{
	return pAnimation->GetBool(CANCEL_PARAMNAME);
}

nlohmann::json PlayerAttackState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["nextCombAttack"] = nextCombAtkState;
	data["senko"] = senkoInputTime;
	data["atkInfo"] = pAttackInfo->Save();

	return data;
}

void PlayerAttackState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerActState_Base::Load(_data);

	LoadJsonEnum<PlayerState>("nextCombAttack", nextCombAtkState, _data);
	LoadJsonFloat("senko", senkoInputTime, _data);

	nlohmann::json atkData;
	if (LoadJsonData("atkInfo", atkData, _data))
	{
		pAttackInfo->Load(atkData);
	}
}
