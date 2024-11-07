#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;
CP_Camera* PlayerActState_Base::pCamera = nullptr;


constexpr float ROLLING_SENKOINPUT_SEC(0.2f);	// ���[�����O�̐�s���͕b��
constexpr float CAN_ROLLING_STICKINPUT(0.3f);	// ���[�����O�ł��鍶�X�e�B�b�N�̓��͗�

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
	// �e�A�N�V�����X�V
	UpdateBehavior();

	// �X�e�[�g�J�ڂ��`�F�b�N
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
	// �L�����Z���ł��Ȃ��Ȃ�
	if (!pActionController->GetIsCanCancel()) return false;

	// �{�^������
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling, 
		ROLLING_SENKOINPUT_SEC)) return false;

	// ���[�����O�ł��鍶�X�e�B�b�N�̓��͂��ł��Ă��Ȃ�
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ROLLING_STICKINPUT) return false;

	return true;
}

bool PlayerActState_Base::ImGuiComboPlayerState(const std::string& _caption, PlayerState& _currentState)
{
#ifdef EDIT

	// �R���|�{�b�N�X�ŕύX
	std::string curStateStr = std::string(magic_enum::enum_name(_currentState));
	bool isChange = ImGuiMethod::ComboBox(_caption, curStateStr, playerStateNameList);

	if (isChange)
	{
		// �����񂩂�񋓌^
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
