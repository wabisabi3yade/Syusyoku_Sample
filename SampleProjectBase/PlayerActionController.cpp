#include "pch.h"
#include "PlayerActionController.h"

// �e�A�N�V�������
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

	// �s���N���X�𐶐�
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
	// �ύX�O�A�N�V�����I������
	pCurrentState->Terminal();

	// �w�肵����ԂɑJ��
	nowState = _nextState;
	pCurrentState = pActions[_nextState].get();

	// �ύX��A�N�V������������
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
