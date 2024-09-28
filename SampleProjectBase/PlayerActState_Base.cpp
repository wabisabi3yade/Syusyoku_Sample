#include "pch.h"
#include "PlayerActState_Base.h"

#include "PlayerActionController.h"
#include "PlayerAnimController.h"

PlayerActState_Base::PlayerActState_Base(StateType _stateType)
	: pPlayerObject(nullptr), pAnimController(nullptr), stateType(_stateType)
{
	changeStateSubject = std::make_unique<HashiTaku::Subject<int>>();
}

void PlayerActState_Base::Init(GameObject& _gameObject, HashiTaku::IObserver<int>& _changeObserver)
{
	pPlayerObject = &_gameObject;
	changeStateSubject->AddObserver(_changeObserver);
}

void PlayerActState_Base::OnStartCall()
{
	OnStart();
}

void PlayerActState_Base::UpdateCall()
{
	// 各アクション更新
	Update();
}

void PlayerActState_Base::OnEndCall()
{
	OnEnd();
}

void PlayerActState_Base::SetAnimController(AnimationController& _pController)
{
	pAnimController = &_pController;
}

std::string PlayerActState_Base::StateTypeToStr(StateType _stateType)
{
	using enum StateType;
	switch (_stateType)
	{
	case Move: return "Move";

	case Jump: return "Jump";

	case Attack: return "Attack";

	default: break;
	}

	return std::string();
}

PlayerActState_Base::StateType PlayerActState_Base::GetActStateType() const
{
	return stateType;
}

void PlayerActState_Base::ChangeState(StateType _changeState)
{
	changeStateSubject->NotifyAll(static_cast<int>(_changeState));
}
