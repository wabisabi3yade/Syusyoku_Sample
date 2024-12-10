#include "pch.h"
#include "PlayerActionController_Base.h"
#include "PlayerAction.h"

#include "InSceneSystemManager.h"

// アニメーションコントローラ内のプレイヤー名
constexpr auto STATEANIM_PARAMNAME{ "state" };

// None状態のID(統一させる)
constexpr int STATE_NONE_ID(99);

PlayerActionController_Base::PlayerActionController_Base(PlayerAction& _pAction, CP_Player& _player, const std::string& _controllerName) :
	CharacterActionController(_player, _controllerName),
	place(ActionPlace::Ground)
{
	pAction = &_pAction;
	pInput = &InSceneSystemManager::GetInstance()->GetInput();

	// 初期化
	u_int cancelCnt = static_cast<u_int>(CancelType::MaxNum);
	for (u_int c_i = 0; c_i < cancelCnt; c_i++)
	{
		reserveCancelStates[c_i] = STATE_NONE_ID;
		isReservedCurFrame[c_i] = false;
	}
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
	// 初期化
	u_int cancelCnt = static_cast<u_int>(CancelType::MaxNum);
	for (u_int c_i = 0; c_i < cancelCnt; c_i++)
	{
		reserveCancelStates[c_i] = STATE_NONE_ID;
	}
}

void PlayerActionController_Base::ResetReservedFlag()
{
	// フラグをリセット
	u_int cancelCnt = static_cast<u_int>(CancelType::MaxNum);
	for (u_int c_i = 0; c_i < cancelCnt; c_i++)
	{
		isReservedCurFrame[c_i] = false;
	}
}

void PlayerActionController_Base::SetReserveState(int _setState, CancelType _cancelState)
{
	if (!stateNodeList.contains(_setState)) return;

	int cancelId = static_cast<int>(_cancelState);

	// 同時押し対策
	// 既に今フレームで予約しているなら
	if (isReservedCurFrame[cancelId])
	{
		// 予約している側の優先度取得
		int reservedId = reserveCancelStates[cancelId];
		int reservedPri = GetStatePriority(reservedId);

		// セットする側の優先度取得
		int setPri = GetStatePriority(_setState);

		if (setPri < reservedPri) return;	// 予約してる側の方が上なら終了
	}

	reserveCancelStates[cancelId] = _setState;
	isReservedCurFrame[cancelId] = true;
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
	// 予約状態から変更する
	if (GetCanAction() &&
		reserveCancelStates[static_cast<u_int>(CancelType::Action)] != STATE_NONE_ID)
	{
		ChangeState(reserveCancelStates[static_cast<u_int>(CancelType::Action)]);
		return;
	}
	if (GetCanAttack() && 
		reserveCancelStates[static_cast<u_int>(CancelType::Attack)] != STATE_NONE_ID)
	{
		ChangeState(reserveCancelStates[static_cast<u_int>(CancelType::Attack)]);
		return;
	}
	if (GetCanMove() && 
		reserveCancelStates[static_cast<u_int>(CancelType::Move)] != STATE_NONE_ID)
	{
		ChangeState(reserveCancelStates[static_cast<u_int>(CancelType::Move)]);
		return;
	}
}
