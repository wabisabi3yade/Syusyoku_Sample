#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;
CP_Camera* PlayerActState_Base::pCamera = nullptr;


constexpr float CAN_ACTION_STICKINPUT(0.7f);	// アクションできる左スティックの入力量
constexpr float INPUT_VECTOR_DOT(0.6f);	// スティックの方向の内積でこれ以上一致していないなら反応しない

namespace DXSimp = DirectX::SimpleMath;

PlayerActState_Base::PlayerActState_Base() :
	pActionController(nullptr), stateType(PlayerState::None), 
	cancelPlayState(PlayerState::None),	targetLookRotateSpeed(40.0f),
	isAttackInput(false), isTargetLookAtEnemy(false)
{
	pPlayerInput = &InSceneSystemManager::GetInstance()->GetInput();
	pCamera = &InSceneSystemManager::GetInstance()->GetMainCamera();
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

	// キャンセルによる遷移変更
	CancelTransitionUpdate();

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
	GetRB().SetVelocity(DXSimp::Vector3::Zero);

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
	// 共通行動
	CommmonCheckTransition();
}

void PlayerActState_Base::ParameterClear()
{
	isAttackInput = false;
	cancelPlayState = PlayerState::None;
}

void PlayerActState_Base::CheckInputUpdate()
{
	if (!pActionController->GetCanInput()) return;

	// ローリングボタンを押す　かつ　左スティックの傾きが足りる
	if (IsRollingInput())
	{
		cancelPlayState = PlayerState::Rolling;
	}

	// 前突進攻撃
	else if (IsSpecialAtkInput(InputVector::Forward))
	{
		cancelPlayState = PlayerState::SpecialAtkHi;
	}

	// 攻撃
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		isAttackInput = true;
	}
}

void PlayerActState_Base::ChangeState(PlayerState _nextState)
{
	pActionController->ChangeState(_nextState);
}

void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
{
	isTargetLookAtEnemy = _isLook;
}

CP_RigidBody& PlayerActState_Base::GetRB()
{
	return *pActionController->GetRB();
}

Transform& PlayerActState_Base::GetTransform()
{
	return pActionController->GetPlayer().GetTransform();
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

bool PlayerActState_Base::IsInputVector(InputVector _checkVector) const
{
	// 入力値を取得
	DXSimp::Vector2 inputVec = GetInputLeftStick();
	if (inputVec.Length() < CAN_ACTION_STICKINPUT) return false;	// 傾きが十分でないなら

	// カメラから見た入力とする
	const Transform& camTrans = pCamera->GetTransform();
	DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() + 
									inputVec.y * camTrans.Forward();
	inputVec = { inputVecByCam.x, inputVecByCam.z };
	inputVec.Normalize();


	// 向きを取得
	const DXSimp::Vector3& playerFwd = pActionController->GetPlayer().GetTransform().Forward();
	DXSimp::Vector2 forwardXZ = { playerFwd.x, playerFwd.z };
	forwardXZ.Normalize();

	// スティックの方向が一致しているか見る
	float dot = inputVec.Dot(forwardXZ);

	HASHI_DEBUG_LOG(std::to_string(dot));
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

bool PlayerActState_Base::IsSpecialAtkInput(InputVector _inputVecter) const
{
	// ボタン入力
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack)) return false;

	// ターゲットした瞬間は出ないようにする
	if (!pActionController->GetIsTargeting() || 
		!pActionController->GetIsPrevTargeting()) return false;

	if (!IsInputVector(_inputVecter)) return false;

	return true;
}

bool PlayerActState_Base::GetCanAttack() const
{
	// キャンセルできないなら
	if (!pActionController->GetIsCanCancel()) return false;

	// 期間中にボタン入力されていたら
	if (!isAttackInput) return false;

	return true;
}

void PlayerActState_Base::ImGuiDebug()
{
}

bool PlayerActState_Base::ImGuiComboPlayerState(const std::string& _caption, PlayerState& _currentState)
{
#ifdef EDIT

	// コンポボックスで変更
	std::string curStateStr = std::string(magic_enum::enum_name(_currentState));
	bool isChange = ImGuiMethod::ComboBox(_caption, curStateStr, playerStateNameList);

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

void PlayerActState_Base::CommmonCheckTransition()
{
}

void PlayerActState_Base::CancelTransitionUpdate()
{
	//キャンセルフラグ
	bool canCancel = pActionController->GetIsCanCancel();
	if (!canCancel) return;

	// キャンセルするアクションがないなら
	if (cancelPlayState == PlayerState::None) return;

	// キャンセルの動きに変更
	ChangeState(cancelPlayState);
}