#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

CP_Camera* PlayerActState_Base::pCamera = nullptr;

PlayerActState_Base::PlayerActState_Base()
	: pPlayer(nullptr), pAnimation(nullptr), stateType(PlayerState::None), pPlayerInput(nullptr)
{
	changeStateSubject = std::make_unique<StateChangeSubject>();

	pPlayerInput = GameInput::GetInstance();

	pCamera = &InSceneSystemManager::GetInstance()->GetMainCamera();
}

void PlayerActState_Base::Init(PlayerState _stateType, CP_Player& _player, StateChangeObserver& _changeObserver)
{
	stateType = _stateType;
	pPlayer = &_player;
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
