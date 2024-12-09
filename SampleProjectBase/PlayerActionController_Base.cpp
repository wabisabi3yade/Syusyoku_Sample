#include "pch.h"
#include "PlayerActionController_Base.h"
#include "PlayerAction.h"

#include "InSceneSystemManager.h"

// アニメーションコントローラ内のプレイヤー名
constexpr  auto STATEANIM_PARAMNAME{ "state" };

PlayerActionController_Base::PlayerActionController_Base(PlayerAction& _pAction, CP_Player& _player, const std::string& _controllerName) :
	CharacterActionController(_player, _controllerName),
	place(ActionPlace::Ground)
{
	pAction = &_pAction;
	pInput = &InSceneSystemManager::GetInstance()->GetInput();
}

void PlayerActionController_Base::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
{
	CharacterActionController::Init(_pAnimation, _pRigidBody);
}

void PlayerActionController_Base::OnEnd()
{
	pCurrentNode->OnEnd();
}

const ITargetAccepter* PlayerActionController_Base::GetTargetAccepter() const
{
	return pAction->GetTargetAccepter();
}

CP_Player& PlayerActionController_Base::GetPlayer()
{
	return static_cast<CP_Player&>(*pCharacter);
}

CP_Camera& PlayerActionController_Base::GetCamera()
{
	return pAction->GetCamera();
}

bool PlayerActionController_Base::GetIsTargeting() const
{
	return pAction->GetIsTargeting();
}

bool PlayerActionController_Base::GetCanInput() const
{
	return pAction->GetCanInput();
}

bool PlayerActionController_Base::GetCanAttack() const
{
	return pAction->GetCanAttack();
}

bool PlayerActionController_Base::GetCanAction() const
{
	return pAction->GetCanCancel();
}

bool PlayerActionController_Base::GetCanMove() const
{
	return pAction->GetCanMove();
}

bool PlayerActionController_Base::ChangeState(int _nextActionState, bool _isForce)
{
	// ステートマシンで変更
	bool isSuccess = StateMachine_Base::ChangeNode(static_cast<int>(_nextActionState), _isForce);
	if (!isSuccess) return false;

	// アニメーション側のstate変数も変更
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));

	return true;
}

bool PlayerActionController_Base::GetCanUpdate()
{
	if (!pRigidBody)
	{
		assert(!"rigidBodyを設定してください");
		return false;
	}

	return true;
}