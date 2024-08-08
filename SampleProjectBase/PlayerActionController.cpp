#include "pch.h"
#include "PlayerActionController.h"

// �e�A�N�V�������
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
	// �ύX�O�A�N�V�����I������
	pCurrentState->Terminal();

	// �w�肵����ԂɑJ��
	pCurrentState = stateList[_nextState].get();

	// �ύX��A�N�V������������
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
