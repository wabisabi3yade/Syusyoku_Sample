#include "pch.h"
#include "PlayerActionController.h"

// 各アクション状態
#include "PlayerState_Base.h"
#include "PlayerMoveState.h"

std::vector<std::string> g_stateNames =
{
	"Move",
};

PlayerActionController::PlayerActionController(GameObject & _pPlayerObject) : pCurrentState(nullptr), pPlayerObject(&_pPlayerObject)
{
	using enum State;

	std::unique_ptr<PlayerState_Base> pActionState = std::make_unique<PlayerMoveState>(*this, *pPlayerObject);
	stateList[Move] = std::move(pActionState);

	pCurrentState = stateList[Move].get();
	nowState = Move;
}



void PlayerActionController::Update()
{
	pCurrentState->Update();
}

void PlayerActionController::TransitionState(State _nextState)
{
	// 変更前アクション終了処理
	pCurrentState->Terminal();

	// 指定した状態に遷移
	pCurrentState = stateList[_nextState].get();

	// 変更後アクション初期処理
	pCurrentState->Init();
}

void PlayerActionController::ImGuiSetting()
{
	if (!ImGui::TreeNode("Action")) return;
	u_int stateNum = static_cast<u_int>(nowState);
	std::string text = "NowState:" + g_stateNames[stateNum];
	ImGui::Text(text.c_str());

	ImGui::TreePop();
}
