#include "pch.h"
#include "PlayerActionController_Base.h"
#include "PlayerAction.h"

#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr auto STATEANIM_PARAMNAME{ "state" };

	// None��Ԃ�ID(���ꂳ����)
	constexpr int STATE_NONE_ID(99);

	PlayerActionController_Base::PlayerActionController_Base(PlayerAction& _pAction, CP_Player& _player, const std::string& _controllerName) :
		CharacterActionController(_player, _controllerName),
		place(ActionPlace::Ground),
		reserveCancelState(STATE_NONE_ID),
		curFlameCancelState(STATE_NONE_ID)
	{
		pAction = &_pAction;
		pInput = &InSceneSystemManager::GetInstance()->GetInput();
	}

	void PlayerActionController_Base::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
	{
		CharacterActionController::Init(_pAnimation, _pRigidBody);
	}

	void PlayerActionController_Base::Update()
	{
		// �\��t���O�����Z�b�g
		ResetReservedFlag();

		CharacterActionController::Update();

		// �\���Ԃ���X�e�[�g�ύX����
		ChangeStateFromReserve();
	}

	void PlayerActionController_Base::OnEnd()
	{
		pCurrentNode->OnEnd();
	}

	void PlayerActionController_Base::ResetReserveState()
	{
		reserveCancelState = STATE_NONE_ID;
	}

	void PlayerActionController_Base::ResetReservedFlag()
	{
		curFlameCancelState = STATE_NONE_ID;
	}

	void PlayerActionController_Base::SetReserveState(int _setState)
	{
		if (!stateNodeList.contains(_setState)) return;

		// �ړ��L�����Z���͑��̃L�����Z�����Ȃ��Ƃ������s��
		CancelType cancelType = GetPlayerState(_setState)->GetCancelType();
		if (reserveCancelState != STATE_NONE_ID && cancelType == CancelType::Move)
			return;

		// ���������΍�
		// ���ɍ��t���[���ŗ\�񂵂Ă���Ȃ�
		if (curFlameCancelState != STATE_NONE_ID)
		{
			// �\�񂵂Ă��鑤�̗D��x�擾
			int reservedPri = GetStatePriority(curFlameCancelState);
			// �Z�b�g���鑤�̗D��x�擾
			int setPri = GetStatePriority(_setState);

			if (setPri < reservedPri) return;	// �\�񂵂Ă鑤�̕�����Ȃ�I��
		}

		curFlameCancelState = _setState;
	}

	PlayerActState_Base* PlayerActionController_Base::GetPlayerState(int _playerStateId)
	{
		StateNode_Base* pState = GetNode(_playerStateId);
		if (!pState) return nullptr;

		return static_cast<PlayerActState_Base*>(pState);
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
		// �X�e�[�g�}�V���ŕύX
		bool isSuccess = StateMachine_Base::ChangeNode(static_cast<int>(_nextActionState), _isForce);
		if (!isSuccess) return false;

		// �A�j���[�V��������state�ϐ����ύX
		pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));

		// �\���Ԃ����Z�b�g
		ResetReserveState();

		return true;
	}

	bool PlayerActionController_Base::GetCanUpdate()
	{
		if (!pRigidBody)
		{
			assert(!"rigidBody��ݒ肵�Ă�������");
			return false;
		}

		return true;
	}

	int PlayerActionController_Base::GetStatePriority(int _stateId) const
	{
		const PlayerActState_Base* pReserve =
			static_cast<PlayerActState_Base*>(stateNodeList.at(_stateId).get());
		if (!pReserve) return -99;	// �������

		return pReserve->GetPriority();
	}

	void PlayerActionController_Base::ChangeStateFromReserve()
	{
		// ���t���[���ŗ\�񂳂ꂽ�Ȃ炻���V�����\���ԂƂ���
		if (curFlameCancelState != STATE_NONE_ID)
		{
			reserveCancelState = curFlameCancelState;
		}

		// �\�񂵂���Ԃ̃L�����Z������J�ڂł����ԂȂ�J�ڂ��s��
		if (reserveCancelState == STATE_NONE_ID) return;	// �\�񂳂�Ă��Ȃ��Ȃ�

		bool canTransition = false;	// �J�ڂł��邩�t���O
		switch (GetPlayerState(reserveCancelState)->GetCancelType())
		{
		case CancelType::Action:
			canTransition = GetCanAction();
			break;
		case CancelType::Attack:
			canTransition = GetCanAttack();
			break;
		case CancelType::Move:
			canTransition = GetCanMove();
			break;

		default:
			HASHI_DEBUG_LOG("�L�����Z���^�C�v�����ʂł��܂���");
			break;
		}

		if (canTransition)	// �J�ڂ���
			ChangeState(reserveCancelState);
	}
}