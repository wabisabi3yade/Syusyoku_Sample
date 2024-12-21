#include "pch.h"
#include "PlayerGroundState.h"
#include "PlayerGroundActionController.h"

namespace HashiTaku
{
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

	json PlayerGroundState::Save()
	{
		return PlayerActState_Base::Save();
	}

	void PlayerGroundState::Load(const json& _data)
	{
		PlayerActState_Base::Load(_data);
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

		// アクションキャンセル
		// ガード
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Guard))
		{
			pActionController->SetReserveState(static_cast<int>(Guard));
		}
		// ローリングボタンを押す　かつ　左スティックの傾きが足りる
		if (IsRollingInput())
		{
			pActionController->SetReserveState(static_cast<int>(Rolling));
		}
		// ジャンプ
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Jump))
		{
			pActionController->SetReserveState(static_cast<int>(BeginJump));
		}

		// 攻撃キャンセル
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_ChargeAttack))
		{
			pActionController->SetReserveState(static_cast<int>(ChargeAttack1));
		}
		// 前突進攻撃
		if (IsSpecialAtkInput(InputVector::Forward))
		{
			pActionController->SetReserveState(static_cast<int>(SpecialAtkHi));
		}
		// 攻撃
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
		{
			pActionController->SetReserveState(static_cast<int>(Attack11));
		}

		// 移動キャンセル
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
			{
				pActionController->SetReserveState(static_cast<int>(TargetMove));
			}
			else if (!isTarget && curState != PlayerState::Move)
			{
				pActionController->SetReserveState(static_cast<int>(Move));
			}
		}
	}
}