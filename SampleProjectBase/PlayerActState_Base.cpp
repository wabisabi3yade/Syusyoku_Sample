#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;
CP_Camera* PlayerActState_Base::pCamera = nullptr;


constexpr float ROLLING_SENKOINPUT_SEC(0.2f);	// ���[�����O�̐�s���͕b��
constexpr float CAN_ROLLING_STICKINPUT(0.3f);	// ���[�����O�ł��鍶�X�e�B�b�N�̓��͗�

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
	// ���̓t���O�̃N���A
	InputFlagClear();

	OnStartBehavior();
}

void PlayerActState_Base::Update()
{
	CheckInputUpdate();

	// �e�A�N�V�����X�V
	UpdateBehavior();

	// �X�e�[�g�J�ڂ��`�F�b�N
	TransitionCheckUpdate();
}

void PlayerActState_Base::OnEnd()
{
	// �O�̊����͂Ȃ���
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
	// ���ʍs��
	CommmonCheckTransition();
}

void PlayerActState_Base::InputFlagClear()
{
	isAttackInput = false;
}

void PlayerActState_Base::CheckInputUpdate()
{
	if (!pActionController->GetCanInput()) return;

	// �U��
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
	// �L�����Z���ł��Ȃ��Ȃ�
	if (!pActionController->GetIsCanCancel()) return false;

	// ���͎󂯕t���Ă��Ȃ��Ȃ�
	if (!pActionController->GetCanInput()) return false;

	// �{�^������
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

	// ���[�����O�ł��鍶�X�e�B�b�N�̓��͂��ł��Ă��Ȃ�
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ROLLING_STICKINPUT) return false;

	return true;
}

bool PlayerActState_Base::GetCanAttack() const
{
	// �L�����Z���ł��Ȃ��Ȃ�
	if (!pActionController->GetIsCanCancel()) return false;

	// ���Ԓ��Ƀ{�^�����͂���Ă�����
	if (!isAttackInput) return false;

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

void PlayerActState_Base::CommmonCheckTransition()
{
	// �L�����Z���s��������
	if (CheckCanCancelTransition()) return;
}

bool PlayerActState_Base::CheckCanCancelTransition()
{
	//�L�����Z���t���O
	bool canCancel = pActionController->GetIsCanCancel();
	if (!canCancel) return false;

	// �ォ��D�揇�ʂ������Ȃ�

	// ���[�����O
	if (CheckCanRolling())
	{
		ChangeState(PlayerState::Rolling);
		return true;
	}

	return false;
}

bool PlayerActState_Base::CheckCanRolling()
{
	// �{�^������
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling, ROLLING_SENKOINPUT_SEC)) 
		return false;

	// ���X�e�B�b�N�̌X��������Ȃ�
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ROLLING_STICKINPUT)
		return false;

	return true;
}
