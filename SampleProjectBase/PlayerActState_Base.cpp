#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;
CP_Camera* PlayerActState_Base::pCamera = nullptr;


constexpr float ROLLING_SENKOINPUT_SEC(0.2f);	// ローリングの先行入力秒数
constexpr float CAN_ROLLING_STICKINPUT(0.3f);	// ローリングできる左スティックの入力量

PlayerActState_Base::PlayerActState_Base()
	: pActionController(nullptr), pAnimation(nullptr), stateType(PlayerState::None)
{
	changeStateSubject = std::make_unique<StateChangeSubject>();

	pPlayerInput = GameInput::GetInstance();
	pCamera = &InSceneSystemManager::GetInstance()->GetMainCamera();
}

void PlayerActState_Base::Init(PlayerState _stateType, PlayerActionController& _actController, StateChangeObserver& _changeObserver)
{
	stateType = _stateType;
	pActionController = &_actController;
	changeStateSubject->AddObserver(_changeObserver);
}

void PlayerActState_Base::OnStart()
{
	OnStartBehavior();
}

void PlayerActState_Base::Update()
{
	// 各アクション更新
	UpdateBehavior();

	// ステート遷移をチェック
	TransitionCheckUpdate();
}

void PlayerActState_Base::OnEnd()
{
	OnEndBehavior();
}


void PlayerActState_Base::SetAnimation(CP_Animation& _pAnimation)
{
	pAnimation = &_pAnimation;
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

void PlayerActState_Base::ChangeState(PlayerState _nextState)
{
	changeStateSubject->NotifyAll(static_cast<int>(_nextState));
}

DirectX::SimpleMath::Vector2 PlayerActState_Base::GetInputLeftStick() const
{
	return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
}

bool PlayerActState_Base::GetCanRolling() const
{
	// キャンセルできないなら
	if (!pActionController->GetIsCanCancel()) return false;

	// ボタン入力
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling, 
		ROLLING_SENKOINPUT_SEC)) return false;

	// ローリングできる左スティックの入力ができていない
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ROLLING_STICKINPUT) return false;

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
