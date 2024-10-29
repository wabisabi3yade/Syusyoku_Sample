#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

CP_Camera* PlayerActState_Base::pCamera = nullptr;

PlayerActState_Base::PlayerActState_Base()
	: pPlayerObject(nullptr), pAnimation(nullptr), stateType(StateType::None), pPlayerInput(nullptr)
{
	changeStateSubject = std::make_unique<StateChangeSubject>();

	pPlayerInput = GameInput::GetInstance();

	pCamera = &InSceneSystemManager::GetInstance()->GetMainCamera();
}

void PlayerActState_Base::Init(StateType _stateType, GameObject& _gameObject, StateChangeObserver& _changeObserver)
{
	stateType = _stateType;
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

void PlayerActState_Base::SetAnimation(CP_Animation& _pAnimation)
{
	pAnimation = &_pAnimation;
}

std::string PlayerActState_Base::StateTypeToStr(StateType _stateType)
{
	using enum StateType;
	switch (_stateType)
	{
	case Idle: return "Idle";

	case Move: return "Move";

	case TargetMove: return "TragetMove";

	case Jump: return "Jump";

	case GroundAttack1: return "Attack";

	default: break;
	}

	return std::string();
}

PlayerActState_Base::StateType PlayerActState_Base::GetActStateType() const
{
	return stateType;
}

nlohmann::json PlayerActState_Base::Save()
{
	return nlohmann::json();
}

void PlayerActState_Base::Load(const nlohmann::json& _data)
{
}

void PlayerActState_Base::ChangeState(StateType _nextState)
{
	changeStateSubject->NotifyAll(static_cast<int>(_nextState));
}

DirectX::SimpleMath::Vector2 PlayerActState_Base::InputValue()
{
	return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
}
