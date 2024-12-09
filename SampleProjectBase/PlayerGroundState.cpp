#include "pch.h"
#include "PlayerGroundState.h"
#include "PlayerGroundActionController.h"

// �ړ��ŃL�����Z�����ł���X�e�B�b�N�̓��͗�
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

nlohmann::json PlayerGroundState::Save()
{
	return PlayerActState_Base::Save();
}

void PlayerGroundState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);
}

void PlayerGroundState::TransitionCheckUpdate()
{
	using enum PlayerState;
	if (pActionController->GetCanAttack() && atkReserveState != static_cast<int>(None))
	{
		ChangeState(static_cast<PlayerState>(atkReserveState));
		return;
	}
	if (pActionController->GetCanAction() && actionReserveState != static_cast<int>(None))
	{
		ChangeState(static_cast<PlayerState>(actionReserveState));
		return;
	}
	if (pActionController->GetCanMove() && moveReserveState != static_cast<int>(None))
	{
		ChangeState(static_cast<PlayerState>(moveReserveState));
		return;
	}
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
	if (!pActionController->GetCanInput()) return;	// ���͎󂯕t���Ă��Ȃ��Ȃ�

	using enum PlayerState;
	// �K�[�h
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Guard))
	{
		atkReserveState = static_cast<int>(None);
		actionReserveState = static_cast<int>(Guard);
	}
	// ���[�����O�{�^���������@���@���X�e�B�b�N�̌X���������
	else if (IsRollingInput())
	{
		atkReserveState = static_cast<int>(None);
		actionReserveState = static_cast<int>(Rolling);
	}
	// �W�����v
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Jump))
	{
		atkReserveState = static_cast<int>(None);
		actionReserveState = static_cast<int>(BeginJump);
	}
	// �O�ːi�U��
	else if (IsSpecialAtkInput(InputVector::Forward))
	{
		actionReserveState = static_cast<int>(None);
		atkReserveState = static_cast<int>(SpecialAtkHi);
	}
	// �U��
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		actionReserveState = static_cast<int>(None);
		atkReserveState = static_cast<int>(Attack11);
	}
	// �ړ��L�����Z��
	else
	{
		// ���͗�
		float inputMag = std::min(1.0f,
			pPlayerInput->GetValue(GameInput::ValueType::Player_Move).Length());

		// �X�e�B�b�N�̓��͗ʂ��傫�����
		if (inputMag > CAN_MOVECANCEL_INPUT)
		{
			PlayerState curState = CastGroundController().GetCurrentState();
			bool isTarget = pActionController->GetIsTargeting();
			// ������ނ̈ړ����ړ��͂��Ȃ��悤�ɂ���
			// �^�[�Q�b�g���Ȃ�^�[�Q�b�g�ړ�
			if (isTarget && curState != PlayerState::TargetMove)
				moveReserveState = static_cast<int>(TargetMove);
			else if (!isTarget && curState != PlayerState::Move)
				moveReserveState = static_cast<int>(Move);
		}
	}
}
