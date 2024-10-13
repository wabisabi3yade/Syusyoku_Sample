#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

CP_Camera* PlayerActState_Base::pCamera = nullptr;

PlayerActState_Base::PlayerActState_Base(StateType _stateType)
	: pPlayerObject(nullptr), pAnimation(nullptr), stateType(_stateType), pPlayerInput(nullptr)
{
	changeStateSubject = std::make_unique<HashiTaku::Subject<int>>();

	pPlayerInput = GameInput::GetInstance();

	pCamera = &InSceneSystemManager::GetInstance()->GetMainCamera();
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

	case Attack: return "Attack";

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

void PlayerActState_Base::ChangeState(StateType _changeState)
{
	changeStateSubject->NotifyAll(static_cast<int>(_changeState));
}

DirectX::SimpleMath::Vector2 PlayerActState_Base::InputValue()
{
	return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
}
