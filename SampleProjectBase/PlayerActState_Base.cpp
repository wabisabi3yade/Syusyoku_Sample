#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;

// アクションできる左スティックの入力量
constexpr float CAN_ACTION_STICKINPUT(0.7f);
// スティックの方向の内積でこれ以上一致していないなら反応しない
constexpr float INPUT_VECTOR_DOT(0.6f);
// 移動でキャンセルができるスティックの入力量
constexpr float CAN_MOVECANCEL_INPUT(0.3f);
// キャンセル入力で予約した状態の有効期限時間（超えると予約した状態はリセットされる）
constexpr float CANCEL_RESERVE_VALIED_TIME(0.3f);

namespace DXSimp = DirectX::SimpleMath;

PlayerActState_Base::PlayerActState_Base() :
	pActionController(nullptr),
	stateType(PlayerState::None),
	cancelReserveState(PlayerState::None),
	atkReserveState(PlayerState::None),
	moveReserveState(PlayerState::None),
	placeElement(ActPlaceElement::Ground),
	targetLookRotateSpeed(40.0f),
	lastCancelReserveElapse(0.0f),
	isTargetLookAtEnemy(false)
{
	pPlayerInput = &InSceneSystemManager::GetInstance()->GetInput();
}

void PlayerActState_Base::Init(PlayerState _stateType, PlayerActionController& _actController)
{
	stateType = _stateType;
	pActionController = &_actController;
}

void PlayerActState_Base::OnStart()
{
	// 入力フラグのクリア
	ParameterClear();

	OnStartBehavior();
}

void PlayerActState_Base::Update()
{
	// 入力更新
	CheckInputUpdate();

	// ターゲットの方向を見る
	UpdateTargetLook();

	// 各アクション更新
	UpdateBehavior();

	// ステート遷移をチェック
	TransitionCheckUpdate();
}

void PlayerActState_Base::OnEnd()
{
	// 前の慣性はなくす
	//GetRB().SetVelocity(DXSimp::Vector3::Zero);

	OnEndBehavior();
}

PlayerActState_Base::PlayerState PlayerActState_Base::GetActStateType() const
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

void PlayerActState_Base::TransitionCheckUpdate()
{
	// キャンセルによる遷移変更
	CancelTransitionUpdate();
}

void PlayerActState_Base::ParameterClear()
{
	cancelReserveState = PlayerState::None;
	atkReserveState = PlayerState::None;
	moveReserveState = PlayerState::None;
}

void PlayerActState_Base::CheckInputUpdate()
{
	if (!pActionController->GetCanInput()) return;	// 入力受け付けていないなら

	// ガード
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Guard))
	{
		atkReserveState = PlayerState::None;
		cancelReserveState = PlayerState::Guard;
	}
	// ローリングボタンを押す　かつ　左スティックの傾きが足りる
	else if (IsRollingInput())
	{
		atkReserveState = PlayerState::None;
		cancelReserveState = PlayerState::Rolling;
	}

	// ジャンプ
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Jump))
	{
		atkReserveState = PlayerState::None;
		cancelReserveState = PlayerState::BeginJump;
	}

	// 前突進攻撃
	else if (IsSpecialAtkInput(InputVector::Forward))
	{
		cancelReserveState = PlayerState::None;
		atkReserveState = PlayerState::SpecialAtkHi;
	}

	// 攻撃
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		cancelReserveState = PlayerState::None;
		atkReserveState = PlayerState::Attack11;
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
			PlayerState curState = pActionController->GetCurrentState();
			bool isTarget = pActionController->GetIsTargeting();
			// 同じ種類の移動→移動はしないようにする
			// ターゲット時ならターゲット移動
			if(isTarget && curState != PlayerState::TargetMove)
				moveReserveState = PlayerState::TargetMove;
			else if (!isTarget && curState != PlayerState::Move)
				moveReserveState = PlayerState::Move;				
		}
	}
}

void PlayerActState_Base::ChangeState(PlayerState _nextState)
{
	pActionController->ChangeState(_nextState);
}

void PlayerActState_Base::ClearVelocity(bool _applyY)
{
	DXSimp::Vector3 set;
	if (!_applyY) // y軸反映させたくないなら
	{
		set.y = GetRB().GetVelocity().y;
	}
	GetRB().SetVelocity(set);
}

void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
{
	isTargetLookAtEnemy = _isLook;
}

void PlayerActState_Base::SetInvicible(bool _isInvicible)
{
	GetPlayer().SetIsInvicible(_isInvicible);
}

CP_RigidBody& PlayerActState_Base::GetRB()
{
	return *pActionController->GetRB();
}

Transform& PlayerActState_Base::GetTransform()
{
	return GetPlayer().GetTransform();
}

CP_Animation* PlayerActState_Base::GetAnimation()
{
	return pActionController->GetAnimation();
}

CP_Player& PlayerActState_Base::GetPlayer()
{
	return pActionController->GetPlayer();
}

float PlayerActState_Base::DeltaTime() const
{
	return pActionController->GetCharacter().DeltaTime();
}

float PlayerActState_Base::GetDeltaSpeed() const
{
	return pActionController->GetCharacter().GetDeltaSpeed();
}

DirectX::SimpleMath::Vector2 PlayerActState_Base::GetInputLeftStick() const
{
	return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
}

bool PlayerActState_Base::IsInputVector(InputVector _checkVector)
{
	// 入力値を取得
	DXSimp::Vector2 inputVec = GetInputLeftStick();
	if (inputVec.Length() < CAN_ACTION_STICKINPUT) return false;	// 傾きが十分でないなら

	// カメラから見た入力とする
	const Transform& camTrans = pActionController->GetCamera().GetTransform();
	DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
		inputVec.y * camTrans.Forward();
	inputVec = { inputVecByCam.x, inputVecByCam.z };
	inputVec.Normalize();

	// 向きを取得
	DXSimp::Vector3 baseVec;

	// ターゲット時で敵がいるなら　敵の方向ベクトルを基準ベクトルに
	if (pActionController->GetIsTargeting() &&
		pActionController->GetTargetObject())
	{
		DXSimp::Vector3 targetObjVec =
			pActionController->GetTargetObject()->GetWorldPosByTargetObj() -
			GetTransform().GetPosition();

		targetObjVec.Normalize(baseVec);
	}
	else // 違うなら　プレイヤーの前ベクトル
		baseVec = GetPlayer().GetTransform().Forward();

	DXSimp::Vector2 baseVecXZ = { baseVec.x, baseVec.z };
	baseVecXZ.Normalize();

	// スティックの方向が一致しているか見る
	float dot = inputVec.Dot(baseVecXZ);

	return dot > INPUT_VECTOR_DOT ? true : false;
}

bool PlayerActState_Base::IsRollingInput() const
{
	// ボタン入力
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

	// ローリングできる左スティックの入力ができていない
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ACTION_STICKINPUT) return false;

	return true;
}

bool PlayerActState_Base::IsSpecialAtkInput(InputVector _inputVecter)
{
	// ボタン入力
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack)) return false;

	// ターゲットしていないなら
	if (!pActionController->GetIsTargeting()) return false;

	if (!IsInputVector(_inputVecter)) return false;

	return true;
}

void PlayerActState_Base::ImGuiDebug()
{
}

bool PlayerActState_Base::ImGuiComboAttack(const std::string& _caption, PlayerState& _currentState)
{
#ifdef EDIT

	// コンポボックスで変更
	std::string curStateStr = std::string(magic_enum::enum_name(_currentState));
	bool isChange = ImGuiMethod::ComboBox(_caption, curStateStr, playerCombAtkList);

	if (isChange)
	{
		// 文字列から列挙型
		auto changeState = magic_enum::enum_cast<PlayerState>(curStateStr);
		if (changeState.has_value())
		{
			_currentState = changeState.value();
			return true;
		}
		else
			return false;
	}
#endif EDIT

	return false;
}

void PlayerActState_Base::UpdateTargetLook()
{
	// ターゲットの方向見ないなら
	if (!isTargetLookAtEnemy) return;

	ITargetAccepter* pTargetObj = pActionController->GetTargetObject();
	if (!pTargetObj) return;	// ターゲットがいないなら

	Transform& transform = pActionController->GetPlayer().GetTransform();

	// ターゲットへのベクトルを求める
	const DXSimp::Vector3& playerPos = transform.GetPosition();
	const DXSimp::Vector3& targetPos = pTargetObj->GetWorldPosByTargetObj();
	DXSimp::Vector3 vector = targetPos - playerPos;
	vector.y = 0.0f; vector.Normalize();

	// 回転させる
	DXSimp::Quaternion targetRot = Quat::RotateToVector(vector);
	DXSimp::Quaternion myRot = transform.GetRotation();
	myRot = DXSimp::Quaternion::Slerp(myRot, targetRot, targetLookRotateSpeed * DeltaTime());

	transform.SetRotation(myRot);
}

void PlayerActState_Base::CancelTransitionUpdate()
{
	//キャンセルフラグ
	if (pActionController->GetIsCanCancel() && cancelReserveState != PlayerState::None)
	{
		// キャンセルの動きに変更
		ChangeState(cancelReserveState);
		return;
	}
	if (pActionController->GetCanAttack() && atkReserveState != PlayerState::None)
	{
		// 攻撃の動きに変更
		ChangeState(atkReserveState);
		return;
	}
	if (pActionController->GetCanMove() && moveReserveState != PlayerState::None)
	{
		// 移動の動きに変更
		ChangeState(moveReserveState);
		return;
	}
	
}