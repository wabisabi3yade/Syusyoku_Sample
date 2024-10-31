#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerActionController.h"
#include "GameObject.h"


PlayerAttackState::PlayerAttackState()
	: nextCombAtkState(StateType::None)
{
	pAttackInfo = std::make_unique<HashiTaku::AttackInformation>();
}

void PlayerAttackState::OnStartBehavior()
{
	// 攻撃情報を更新
	UpdateAttackInfo();
}

void PlayerAttackState::UpdateBehavior()
{
}

void PlayerAttackState::OnEndBehavior()
{
}

void PlayerAttackState::OnTransitionCheck()
{
	// 攻撃入力されたらステート遷移する
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
		ChangeState(nextCombAtkState);
}

void PlayerAttackState::UpdateAttackInfo()
{
	CP_Player& playerComp = *pPlayerObject->GetComponent<CP_Player>();
	playerComp.SetAttackInfo(*pAttackInfo);
}

void PlayerAttackState::ImGuiSetting()
{
	ImGuiComboPlayerState("next", nextCombAtkState);
}

nlohmann::json PlayerAttackState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["nextCombAttack"] = nextCombAtkState;

	return data;
}

void PlayerAttackState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerActState_Base::Load(_data);

	LoadJsonEnum<StateType>("nextCombAttack", nextCombAtkState, _data);
}
