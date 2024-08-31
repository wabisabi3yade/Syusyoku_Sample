#include "pch.h"
#include "PlayerActionController.h"

// 各アクション状態
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"

#include "PlayerAnimController.h"

std::vector<std::string> g_stateNames =
{
	"Move",
	"Jump",
	"Action",
};

PlayerActionController::PlayerActionController(GameObject & _pPlayerObject, PlayerAnimController& _animController) 
	: pCurrentState(nullptr), pAnimController(&_animController), pPlayerObject(&_pPlayerObject)
{
	using enum State;

	// 行動クラスを生成
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerAttackState>(Attack);

	pCurrentState = pActions[Move].get();
	nowState = Move;
}

void PlayerActionController::Update()
{
	pCurrentState->UpdateBase();
}

void PlayerActionController::TransitionState(State _nextState)
{
	// 変更前アクション終了処理
	pCurrentState->Terminal();

	// 指定した状態に遷移
	nowState = _nextState;
	pCurrentState = pActions[_nextState].get();

	// 変更後アクション初期処理
	pCurrentState->Init();
}

void PlayerActionController::ImGuiSetting()
{
	if (!ImGui::TreeNode("Action")) return;

	u_int stateNum = static_cast<u_int>(nowState);
	std::string text = "NowState:" + g_stateNames[stateNum];
	ImGui::Text(text.c_str());

	pCurrentState->ImGuiSetting();

	ImGui::TreePop();
}
