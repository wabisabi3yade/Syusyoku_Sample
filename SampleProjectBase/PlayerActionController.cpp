#include "pch.h"
#include "PlayerActionController.h"

// 各アクション状態
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"

#include "PlayerAnimController.h"

PlayerActionController::PlayerActionController(GameObject & _pPlayerObject) 
	: pCurrentState(nullptr), pAnimController(nullptr), pPlayerObject(&_pPlayerObject)
{
	// 状態遷移オブザーバー生成
	pStateChangeObserver = std::make_unique<PlayerActChangeObserver>("StateChangeObserver", *this);

	// 行動クラスを生成
	using enum PlayerActState_Base::StateType;
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerAttackState>(Attack);

	// デフォルト状態をセット
	DefaultState(PlayerActState_Base::StateType::Move);
}
void PlayerActionController::Begin(AnimationController& _animationController)
{
	// アニメーションコントローラーを各ステートに渡す
	pAnimController = &_animationController;
	for (auto& actState : pActions)
	{
		actState.second->SetAnimController(_animationController);
	}	
}

void PlayerActionController::Update()
{
	pCurrentState->UpdateCall();
}

void PlayerActionController::ChangeState(PlayerActState_Base::StateType _nextState)
{
	// 変更前アクション終了処理
	pCurrentState->OnEndCall();

	// 指定した状態に遷移
	pCurrentState = pActions[_nextState].get();

	// 変更後アクション初期処理
	pCurrentState->OnStartCall();

	HASHI_DEBUG_LOG(PlayerActState_Base::StateTypeToStr(_nextState) + "に遷移");
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

	for (auto& pAct : pActions)	// 各アクションの調整
		pAct.second->ImGuiCall();

	ImGui::TreePop();
}
