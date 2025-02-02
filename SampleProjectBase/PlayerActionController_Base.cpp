#include "pch.h"
#include "PlayerActionController_Base.h"
#include "PlayerAction.h"

#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// アニメーションコントローラ内のプレイヤー名
	constexpr auto STATEANIM_PARAMNAME{ "state" };

	// None状態のID(統一させる)
	constexpr int STATE_NONE_ID(99);

	PlayerActionController_Base::PlayerActionController_Base(PlayerAction& _pAction, CP_Player& _player, const std::string& _controllerName) :
		CharacterActionController(_player, _controllerName),
		place(ActionPlace::Ground),
		reserveCancelState(STATE_NONE_ID),
		curFlameCancelState(STATE_NONE_ID)
	{
		pAction = &_pAction;
		pInput = &InSceneSystemManager::GetInstance()->GetInput();
	}

	void PlayerActionController_Base::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
	{
		CharacterActionController::Init(_pAnimation, _pRigidBody);
	}

	void PlayerActionController_Base::Update()
	{
		// 予約フラグをリセット
		ResetReservedFlag();

		CharacterActionController::Update();

		// 予約状態からステート変更する
		ChangeStateFromReserve();
	}

	void PlayerActionController_Base::OnEnd()
	{
		pCurrentNode->OnEnd();
	}

	void PlayerActionController_Base::ResetReserveState()
	{
		reserveCancelState = STATE_NONE_ID;
	}

	void PlayerActionController_Base::ResetReservedFlag()
	{
		curFlameCancelState = STATE_NONE_ID;
	}

	void PlayerActionController_Base::SetReserveState(int _setState)
	{
		if (!stateNodeList.contains(_setState)) return;

		// 移動キャンセルは他のキャンセルがないときだけ行う
		CancelType cancelType = GetPlayerState(_setState)->GetCancelType();
		if (reserveCancelState != STATE_NONE_ID && cancelType == CancelType::Move)
			return;

		// 同時押し対策
		// 既に今フレームで予約しているなら
		if (curFlameCancelState != STATE_NONE_ID)
		{
			// 予約している側の優先度取得
			int reservedPri = GetStatePriority(curFlameCancelState);
			// セットする側の優先度取得
			int setPri = GetStatePriority(_setState);

			if (setPri < reservedPri) return;	// 予約してる側の方が上なら終了
		}

		curFlameCancelState = _setState;
	}

	PlayerActState_Base* PlayerActionController_Base::GetPlayerState(int _playerStateId)
	{
		StateNode_Base* pState = GetNode(_playerStateId);
		if (!pState) return nullptr;

		return static_cast<PlayerActState_Base*>(pState);
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

		// 予約状態をリセット
		ResetReserveState();

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

	int PlayerActionController_Base::GetStatePriority(int _stateId) const
	{
		const PlayerActState_Base* pReserve =
			static_cast<PlayerActState_Base*>(stateNodeList.at(_stateId).get());
		if (!pReserve) return -99;	// 無ければ

		return pReserve->GetPriority();
	}

	void PlayerActionController_Base::ChangeStateFromReserve()
	{
		// 今フレームで予約されたならそれを新しい予約状態とする
		if (curFlameCancelState != STATE_NONE_ID)
		{
			reserveCancelState = curFlameCancelState;
		}

		// 予約した状態のキャンセルから遷移できる状態なら遷移を行う
		if (reserveCancelState == STATE_NONE_ID) return;	// 予約されていないなら

		bool canTransition = false;	// 遷移できるかフラグ
		switch (GetPlayerState(reserveCancelState)->GetCancelType())
		{
		case CancelType::Action:
			canTransition = GetCanAction();
			break;
		case CancelType::Attack:
			canTransition = GetCanAttack();
			break;
		case CancelType::Move:
			canTransition = GetCanMove();
			break;

		default:
			HASHI_DEBUG_LOG("キャンセルタイプが判別できません");
			break;
		}

		if (canTransition)	// 遷移する
			ChangeState(reserveCancelState);
	}
}