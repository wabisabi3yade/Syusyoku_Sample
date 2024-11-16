#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;
CP_Camera* PlayerActState_Base::pCamera = nullptr;


constexpr float ROLLING_SENKOINPUT_SEC(0.2f);	// ローリングの先行入力秒数
constexpr float CAN_ROLLING_STICKINPUT(0.3f);	// ローリングできる左スティックの入力量

namespace DX = DirectX::SimpleMath;

PlayerActState_Base::PlayerActState_Base()
	: pActionController(nullptr), stateType(PlayerState::None), isAttackInput(false)
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
	InputFlagClear();

	OnStartBehavior();
}

void PlayerActState_Base::Update()
{
	CheckInputUpdate();

	// 各アクション更新
	UpdateBehavior();

	// ステート遷移をチェック
	TransitionCheckUpdate();
}

void PlayerActState_Base::OnEnd()
{
	// 前の慣性はなくす
	GetRB().SetVelocity(DX::Vector3::Zero);

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

void PlayerActState_Base::InputFlagClear()
{
	isAttackInput = false;
}

void PlayerActState_Base::CheckInputUpdate()
{
	if (!pActionController->GetCanInput()) return;

	// 攻撃
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		isAttackInput = true;
	}
}

void PlayerActState_Base::ChangeState(PlayerState _nextState)
{
	pActionController->ChangeState(_nextState);
}

CP_RigidBody& PlayerActState_Base::GetRB()
{
	return *pActionController->GetRB();
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

bool PlayerActState_Base::GetCanRolling() const
{
	// キャンセルできないなら
	if (!pActionController->GetIsCanCancel()) return false;

	// 入力受け付けていないなら
	if (!pActionController->GetCanInput()) return false;

	// ボタン入力
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

	// ローリングできる左スティックの入力ができていない
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ROLLING_STICKINPUT) return false;

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

void PlayerActState_Base::CommmonCheckTransition()
{
	// キャンセル行動を見る
	if (CheckCanCancelTransition()) return;
}

bool PlayerActState_Base::CheckCanCancelTransition()
{
	//キャンセルフラグ
	bool canCancel = pActionController->GetIsCanCancel();
	if (!canCancel) return false;

	// 上から優先順位が高くなる

	// ローリング
	if (CheckCanRolling())
	{
		ChangeState(PlayerState::Rolling);
		return true;
	}

	return false;
}

bool PlayerActState_Base::CheckCanRolling()
{
	// ボタン入力
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling, ROLLING_SENKOINPUT_SEC)) 
		return false;

	// 左スティックの傾きが足りない
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ROLLING_STICKINPUT)
		return false;

	return true;
}
