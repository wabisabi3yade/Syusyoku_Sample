#include "pch.h"
#include "PlayerActionController_Base.h"
#include "PlayerAction.h"

#include "InSceneSystemManager.h"

// �A�j���[�V�����R���g���[�����̃v���C���[��
constexpr auto STATEANIM_PARAMNAME{ "state" };

// None��Ԃ�ID(���ꂳ����)
constexpr int STATE_NONE_ID(99);

PlayerActionController_Base::PlayerActionController_Base(PlayerAction& _pAction, CP_Player& _player, const std::string& _controllerName) :
	CharacterActionController(_player, _controllerName),
	place(ActionPlace::Ground)
{
	pAction = &_pAction;
	pInput = &InSceneSystemManager::GetInstance()->GetInput();

	// ������
	u_int cancelCnt = static_cast<u_int>(CancelType::MaxNum);
	for (u_int c_i = 0; c_i < cancelCnt; c_i++)
	{
		reserveCancelStates[c_i] = STATE_NONE_ID;
		isReservedCurFrame[c_i] = false;
	}
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
	// ������
	u_int cancelCnt = static_cast<u_int>(CancelType::MaxNum);
	for (u_int c_i = 0; c_i < cancelCnt; c_i++)
	{
		reserveCancelStates[c_i] = STATE_NONE_ID;
	}
}

void PlayerActionController_Base::ResetReservedFlag()
{
	// �t���O�����Z�b�g
	u_int cancelCnt = static_cast<u_int>(CancelType::MaxNum);
	for (u_int c_i = 0; c_i < cancelCnt; c_i++)
	{
		isReservedCurFrame[c_i] = false;
	}
}

void PlayerActionController_Base::SetReserveState(int _setState, CancelType _cancelState)
{
	if (!stateNodeList.contains(_setState)) return;

	int cancelId = static_cast<int>(_cancelState);

	// ���������΍�
	// ���ɍ��t���[���ŗ\�񂵂Ă���Ȃ�
	if (isReservedCurFrame[cancelId])
	{
		// �\�񂵂Ă��鑤�̗D��x�擾
		int reservedId = reserveCancelStates[cancelId];
		int reservedPri = GetStatePriority(reservedId);

		// �Z�b�g���鑤�̗D��x�擾
		int setPri = GetStatePriority(_setState);

		if (setPri < reservedPri) return;	// �\�񂵂Ă鑤�̕�����Ȃ�I��
	}

	reserveCancelStates[cancelId] = _setState;
	isReservedCurFrame[cancelId] = true;
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
	// �\���Ԃ���ύX����
	if (GetCanAction() &&
		reserveCancelStates[static_cast<u_int>(CancelType::Action)] != STATE_NONE_ID)
	{
		ChangeState(reserveCancelStates[static_cast<u_int>(CancelType::Action)]);
		return;
	}
	if (GetCanAttack() && 
		reserveCancelStates[static_cast<u_int>(CancelType::Attack)] != STATE_NONE_ID)
	{
		ChangeState(reserveCancelStates[static_cast<u_int>(CancelType::Attack)]);
		return;
	}
	if (GetCanMove() && 
		reserveCancelStates[static_cast<u_int>(CancelType::Move)] != STATE_NONE_ID)
	{
		ChangeState(reserveCancelStates[static_cast<u_int>(CancelType::Move)]);
		return;
	}
}
