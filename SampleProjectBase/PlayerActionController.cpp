#include "pch.h"
#include "PlayerActionController.h"

// �e�A�N�V�������
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"

PlayerActionController::PlayerActionController(GameObject & _pPlayerObject) 
	: pCurrentState(nullptr), pAnimation(nullptr), pPlayerObject(&_pPlayerObject)
{
	// ��ԑJ�ڃI�u�U�[�o�[����
	pStateChangeObserver = std::make_unique<PlayerActChangeObserver>("StateChangeObserver", *this);

	// �s���N���X�𐶐�
	using enum PlayerActState_Base::StateType;
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerAttackState>(Attack);

	// �f�t�H���g��Ԃ��Z�b�g
	DefaultState(PlayerActState_Base::StateType::Move);
}
void PlayerActionController::Begin(CP_Animation& _animationController)
{
	// �A�j���[�V�����R���g���[���[���e�X�e�[�g�ɓn��
	pAnimation = &_animationController;
	for (auto& actState : pActions)
	{
		actState.second->SetAnimation(_animationController);
	}	
}

void PlayerActionController::Update()
{
	pCurrentState->UpdateCall();
}

void PlayerActionController::ChangeState(PlayerActState_Base::StateType _nextState)
{
	// �ύX�O�A�N�V�����I������
	pCurrentState->OnEndCall();

	// �w�肵����ԂɑJ��
	pCurrentState = pActions[_nextState].get();

	// �ύX��A�N�V������������
	pCurrentState->OnStartCall();

	HASHI_DEBUG_LOG(PlayerActState_Base::StateTypeToStr(_nextState) + "�ɑJ��");
}

void PlayerActionController::DefaultState(PlayerActState_Base::StateType _defaultState)
{
	pCurrentState = pActions[_defaultState].get();
}

void PlayerActionController::ImGuiSetting()
{
	if (!ImGuiMethod::TreeNode("Action")) return;

	std::string text = "NowState:" + PlayerActState_Base::StateTypeToStr(pCurrentState->GetActStateType());
	ImGui::Text(text.c_str());

	for (auto& pAct : pActions)	// �e�A�N�V�����̒���
		pAct.second->ImGuiCall();

	ImGui::TreePop();
}
