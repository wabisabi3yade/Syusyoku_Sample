#include "pch.h"
#include "PlayerGroundState.h"
#include "PlayerGroundActionController.h"

// 移動でキャンセルができるスティックの入力量
constexpr float CAN_MOVECANCEL_INPUT(0.3f);

PlayerGroundState::PlayerGroundState() 
{
}

void PlayerGroundState::OnStart()
{
	PlayerActState_Base::OnStart();

	OnStartBehavior();
}

void PlayerGroundState::Update()
{
	InputUpdate();

	PlayerActState_Base::Update();

	UpdateBehavior();

	TransitionCheckUpdate();
}

void PlayerGroundState::OnEnd()
{
	PlayerActState_Base::OnEnd();

	OnEndBehavior();
}

nlohmann::json PlayerGroundState::Save()
{
	return PlayerActState_Base::Save();
}

void PlayerGroundState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);
}

void PlayerGroundState::TransitionCheckUpdate()
{
	using enum PlayerState;
	if (pActionController->GetCanAttack() && atkReserveState != static_cast<int>(None))
	{
		ChangeState(static_cast<PlayerState>(atkReserveState));
		return;
	}
	if (pActionController->GetCanAction() && actionReserveState != static_cast<int>(None))
	{
		ChangeState(static_cast<PlayerState>(actionReserveState));
		return;
	}
	if (pActionController->GetCanMove() && moveReserveState != static_cast<int>(None))
	{
		ChangeState(static_cast<PlayerState>(moveReserveState));
		return;
	}
}

void PlayerGroundState::ChangeState(PlayerState _nextState, bool _isForce)
{
	CastGroundController().ChangeGroundState(_nextState, _isForce);
}

PlayerGroundActionController& PlayerGroundState::CastGroundController()
{
	return static_cast<PlayerGroundActionController&>(*pActionController);
}

void PlayerGroundState::InputUpdate()
{
	if (!pActionController->GetCanInput()) return;	// 入力受け付けていないなら

	using enum PlayerState;
	// ガード
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Guard))
	{
		atkReserveState = static_cast<int>(None);
		actionReserveState = static_cast<int>(Guard);
	}
	// ローリングボタンを押す　かつ　左スティックの傾きが足りる
	else if (IsRollingInput())
	{
		atkReserveState = static_cast<int>(None);
		actionReserveState = static_cast<int>(Rolling);
	}
	// ジャンプ
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Jump))
	{
		atkReserveState = static_cast<int>(None);
		actionReserveState = static_cast<int>(BeginJump);
	}
	// 前突進攻撃
	else if (IsSpecialAtkInput(InputVector::Forward))
	{
		actionReserveState = static_cast<int>(None);
		atkReserveState = static_cast<int>(SpecialAtkHi);
	}
	// 攻撃
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		actionReserveState = static_cast<int>(None);
		atkReserveState = static_cast<int>(Attack11);
	}
	// 移動キャンセル
	else
	{
		// 入力量
		float inputMag = std::min(1.0f,
			pPlayerInput->GetValue(GameInput::ValueType::Player_Move).Length());

		// スティックの入力量が大きければ
		if (inputMag > CAN_MOVECANCEL_INPUT)
		{
			PlayerState curState = CastGroundController().GetCurrentState();
			bool isTarget = pActionController->GetIsTargeting();
			// 同じ種類の移動→移動はしないようにする
			// ターゲット時ならターゲット移動
			if (isTarget && curState != PlayerState::TargetMove)
				moveReserveState = static_cast<int>(TargetMove);
			else if (!isTarget && curState != PlayerState::Move)
				moveReserveState = static_cast<int>(Move);
		}
	}
}
