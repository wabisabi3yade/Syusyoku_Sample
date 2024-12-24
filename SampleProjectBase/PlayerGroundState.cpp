#include "pch.h"
#include "PlayerGroundState.h"
#include "PlayerGroundActionController.h"

namespace HashiTaku
{
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

	json PlayerGroundState::Save()
	{
		return PlayerActState_Base::Save();
	}

	void PlayerGroundState::Load(const json& _data)
	{
		PlayerActState_Base::Load(_data);
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

		// �A�N�V�����L�����Z��
		// �K�[�h
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Guard))
		{
			pActionController->SetReserveState(static_cast<int>(Guard));
		}
		// ���[�����O�{�^���������@���@���X�e�B�b�N�̌X���������
		if (IsRollingInput())
		{
			pActionController->SetReserveState(static_cast<int>(Rolling));
		}
		// �W�����v
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Jump))
		{
			pActionController->SetReserveState(static_cast<int>(BeginJump));
		}

		// �U���L�����Z��
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_ChargeAttack))
		{
			pActionController->SetReserveState(static_cast<int>(ChargeAttack1));
		}
		// �O�ːi�U��
		if (IsSpecialAtkInput(InputVector::Forward))
		{
			pActionController->SetReserveState(static_cast<int>(SpecialAtkHi));
		}
		// �U��
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
		{
			pActionController->SetReserveState(static_cast<int>(Attack11));
		}

		// �ړ��L�����Z��
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
			{
				pActionController->SetReserveState(static_cast<int>(TargetMove));
			}
			else if (!isTarget && curState != PlayerState::Move)
			{
				pActionController->SetReserveState(static_cast<int>(Move));
			}
		}
	}
}