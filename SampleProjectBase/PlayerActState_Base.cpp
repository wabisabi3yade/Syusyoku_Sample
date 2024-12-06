#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;

// �A�N�V�����ł��鍶�X�e�B�b�N�̓��͗�
constexpr float CAN_ACTION_STICKINPUT(0.7f);
// �X�e�B�b�N�̕����̓��ςł���ȏ��v���Ă��Ȃ��Ȃ甽�����Ȃ�
constexpr float INPUT_VECTOR_DOT(0.6f);
// �ړ��ŃL�����Z�����ł���X�e�B�b�N�̓��͗�
constexpr float CAN_MOVECANCEL_INPUT(0.3f);
// �L�����Z�����͂ŗ\�񂵂���Ԃ̗L���������ԁi������Ɨ\�񂵂���Ԃ̓��Z�b�g�����j
constexpr float CANCEL_RESERVE_VALIED_TIME(0.3f);

namespace DXSimp = DirectX::SimpleMath;

PlayerActState_Base::PlayerActState_Base() :
	pActionController(nullptr),
	stateType(PlayerState::None),
	cancelReserveState(PlayerState::None),
	atkReserveState(PlayerState::None),
	moveReserveState(PlayerState::None),
	placeElement(ActPlaceElement::Ground),
	targetLookRotateSpeed(40.0f),
	lastCancelReserveElapse(0.0f),
	isTargetLookAtEnemy(false)
{
	pPlayerInput = &InSceneSystemManager::GetInstance()->GetInput();
}

void PlayerActState_Base::Init(PlayerState _stateType, PlayerActionController& _actController)
{
	stateType = _stateType;
	pActionController = &_actController;
}

void PlayerActState_Base::OnStart()
{
	// ���̓t���O�̃N���A
	ParameterClear();

	OnStartBehavior();
}

void PlayerActState_Base::Update()
{
	// ���͍X�V
	CheckInputUpdate();

	// �^�[�Q�b�g�̕���������
	UpdateTargetLook();

	// �e�A�N�V�����X�V
	UpdateBehavior();

	// �X�e�[�g�J�ڂ��`�F�b�N
	TransitionCheckUpdate();
}

void PlayerActState_Base::OnEnd()
{
	// �O�̊����͂Ȃ���
	//GetRB().SetVelocity(DXSimp::Vector3::Zero);

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
	// �L�����Z���ɂ��J�ڕύX
	CancelTransitionUpdate();
}

void PlayerActState_Base::ParameterClear()
{
	cancelReserveState = PlayerState::None;
	atkReserveState = PlayerState::None;
	moveReserveState = PlayerState::None;
}

void PlayerActState_Base::CheckInputUpdate()
{
	if (!pActionController->GetCanInput()) return;	// ���͎󂯕t���Ă��Ȃ��Ȃ�

	// �K�[�h
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Guard))
	{
		atkReserveState = PlayerState::None;
		cancelReserveState = PlayerState::Guard;
	}
	// ���[�����O�{�^���������@���@���X�e�B�b�N�̌X���������
	else if (IsRollingInput())
	{
		atkReserveState = PlayerState::None;
		cancelReserveState = PlayerState::Rolling;
	}

	// �W�����v
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Jump))
	{
		atkReserveState = PlayerState::None;
		cancelReserveState = PlayerState::BeginJump;
	}

	// �O�ːi�U��
	else if (IsSpecialAtkInput(InputVector::Forward))
	{
		cancelReserveState = PlayerState::None;
		atkReserveState = PlayerState::SpecialAtkHi;
	}

	// �U��
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		cancelReserveState = PlayerState::None;
		atkReserveState = PlayerState::Attack11;
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
			PlayerState curState = pActionController->GetCurrentState();
			bool isTarget = pActionController->GetIsTargeting();
			// ������ނ̈ړ����ړ��͂��Ȃ��悤�ɂ���
			// �^�[�Q�b�g���Ȃ�^�[�Q�b�g�ړ�
			if(isTarget && curState != PlayerState::TargetMove)
				moveReserveState = PlayerState::TargetMove;
			else if (!isTarget && curState != PlayerState::Move)
				moveReserveState = PlayerState::Move;				
		}
	}
}

void PlayerActState_Base::ChangeState(PlayerState _nextState)
{
	pActionController->ChangeState(_nextState);
}

void PlayerActState_Base::ClearVelocity(bool _applyY)
{
	DXSimp::Vector3 set;
	if (!_applyY) // y�����f���������Ȃ��Ȃ�
	{
		set.y = GetRB().GetVelocity().y;
	}
	GetRB().SetVelocity(set);
}

void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
{
	isTargetLookAtEnemy = _isLook;
}

void PlayerActState_Base::SetInvicible(bool _isInvicible)
{
	GetPlayer().SetIsInvicible(_isInvicible);
}

CP_RigidBody& PlayerActState_Base::GetRB()
{
	return *pActionController->GetRB();
}

Transform& PlayerActState_Base::GetTransform()
{
	return GetPlayer().GetTransform();
}

CP_Animation* PlayerActState_Base::GetAnimation()
{
	return pActionController->GetAnimation();
}

CP_Player& PlayerActState_Base::GetPlayer()
{
	return pActionController->GetPlayer();
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

bool PlayerActState_Base::IsInputVector(InputVector _checkVector)
{
	// ���͒l���擾
	DXSimp::Vector2 inputVec = GetInputLeftStick();
	if (inputVec.Length() < CAN_ACTION_STICKINPUT) return false;	// �X�����\���łȂ��Ȃ�

	// �J�������猩�����͂Ƃ���
	const Transform& camTrans = pActionController->GetCamera().GetTransform();
	DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
		inputVec.y * camTrans.Forward();
	inputVec = { inputVecByCam.x, inputVecByCam.z };
	inputVec.Normalize();

	// �������擾
	DXSimp::Vector3 baseVec;

	// �^�[�Q�b�g���œG������Ȃ�@�G�̕����x�N�g������x�N�g����
	if (pActionController->GetIsTargeting() &&
		pActionController->GetTargetObject())
	{
		DXSimp::Vector3 targetObjVec =
			pActionController->GetTargetObject()->GetWorldPosByTargetObj() -
			GetTransform().GetPosition();

		targetObjVec.Normalize(baseVec);
	}
	else // �Ⴄ�Ȃ�@�v���C���[�̑O�x�N�g��
		baseVec = GetPlayer().GetTransform().Forward();

	DXSimp::Vector2 baseVecXZ = { baseVec.x, baseVec.z };
	baseVecXZ.Normalize();

	// �X�e�B�b�N�̕�������v���Ă��邩����
	float dot = inputVec.Dot(baseVecXZ);

	return dot > INPUT_VECTOR_DOT ? true : false;
}

bool PlayerActState_Base::IsRollingInput() const
{
	// �{�^������
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

	// ���[�����O�ł��鍶�X�e�B�b�N�̓��͂��ł��Ă��Ȃ�
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ACTION_STICKINPUT) return false;

	return true;
}

bool PlayerActState_Base::IsSpecialAtkInput(InputVector _inputVecter)
{
	// �{�^������
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack)) return false;

	// �^�[�Q�b�g���Ă��Ȃ��Ȃ�
	if (!pActionController->GetIsTargeting()) return false;

	if (!IsInputVector(_inputVecter)) return false;

	return true;
}

void PlayerActState_Base::ImGuiDebug()
{
}

bool PlayerActState_Base::ImGuiComboAttack(const std::string& _caption, PlayerState& _currentState)
{
#ifdef EDIT

	// �R���|�{�b�N�X�ŕύX
	std::string curStateStr = std::string(magic_enum::enum_name(_currentState));
	bool isChange = ImGuiMethod::ComboBox(_caption, curStateStr, playerCombAtkList);

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

void PlayerActState_Base::UpdateTargetLook()
{
	// �^�[�Q�b�g�̕������Ȃ��Ȃ�
	if (!isTargetLookAtEnemy) return;

	ITargetAccepter* pTargetObj = pActionController->GetTargetObject();
	if (!pTargetObj) return;	// �^�[�Q�b�g�����Ȃ��Ȃ�

	Transform& transform = pActionController->GetPlayer().GetTransform();

	// �^�[�Q�b�g�ւ̃x�N�g�������߂�
	const DXSimp::Vector3& playerPos = transform.GetPosition();
	const DXSimp::Vector3& targetPos = pTargetObj->GetWorldPosByTargetObj();
	DXSimp::Vector3 vector = targetPos - playerPos;
	vector.y = 0.0f; vector.Normalize();

	// ��]������
	DXSimp::Quaternion targetRot = Quat::RotateToVector(vector);
	DXSimp::Quaternion myRot = transform.GetRotation();
	myRot = DXSimp::Quaternion::Slerp(myRot, targetRot, targetLookRotateSpeed * DeltaTime());

	transform.SetRotation(myRot);
}

void PlayerActState_Base::CancelTransitionUpdate()
{
	//�L�����Z���t���O
	if (pActionController->GetIsCanCancel() && cancelReserveState != PlayerState::None)
	{
		// �L�����Z���̓����ɕύX
		ChangeState(cancelReserveState);
		return;
	}
	if (pActionController->GetCanAttack() && atkReserveState != PlayerState::None)
	{
		// �U���̓����ɕύX
		ChangeState(atkReserveState);
		return;
	}
	if (pActionController->GetCanMove() && moveReserveState != PlayerState::None)
	{
		// �ړ��̓����ɕύX
		ChangeState(moveReserveState);
		return;
	}
	
}