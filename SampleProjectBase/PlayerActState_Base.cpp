#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController.h"
#include "InSceneSystemManager.h"

GameInput* PlayerActState_Base::pPlayerInput = nullptr;
CP_Camera* PlayerActState_Base::pCamera = nullptr;


constexpr float CAN_ACTION_STICKINPUT(0.7f);	// �A�N�V�����ł��鍶�X�e�B�b�N�̓��͗�
constexpr float INPUT_VECTOR_DOT(0.6f);	// �X�e�B�b�N�̕����̓��ςł���ȏ��v���Ă��Ȃ��Ȃ甽�����Ȃ�

namespace DXSimp = DirectX::SimpleMath;

PlayerActState_Base::PlayerActState_Base() :
	pActionController(nullptr), stateType(PlayerState::None), 
	cancelPlayState(PlayerState::None),	targetLookRotateSpeed(40.0f),
	isAttackInput(false), isTargetLookAtEnemy(false)
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
	ParameterClear();

	OnStartBehavior();
}

void PlayerActState_Base::Update()
{
	// ���͍X�V
	CheckInputUpdate();

	// �L�����Z���ɂ��J�ڕύX
	CancelTransitionUpdate();

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
	GetRB().SetVelocity(DXSimp::Vector3::Zero);

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

void PlayerActState_Base::ParameterClear()
{
	isAttackInput = false;
	cancelPlayState = PlayerState::None;
}

void PlayerActState_Base::CheckInputUpdate()
{
	if (!pActionController->GetCanInput()) return;

	// ���[�����O�{�^���������@���@���X�e�B�b�N�̌X���������
	if (IsRollingInput())
	{
		cancelPlayState = PlayerState::Rolling;
	}

	// �O�ːi�U��
	else if (IsSpecialAtkInput(InputVector::Forward))
	{
		cancelPlayState = PlayerState::SpecialAtkHi;
	}

	// �U��
	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		isAttackInput = true;
	}
}

void PlayerActState_Base::ChangeState(PlayerState _nextState)
{
	pActionController->ChangeState(_nextState);
}

void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
{
	isTargetLookAtEnemy = _isLook;
}

CP_RigidBody& PlayerActState_Base::GetRB()
{
	return *pActionController->GetRB();
}

Transform& PlayerActState_Base::GetTransform()
{
	return pActionController->GetPlayer().GetTransform();
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

bool PlayerActState_Base::IsInputVector(InputVector _checkVector) const
{
	// ���͒l���擾
	DXSimp::Vector2 inputVec = GetInputLeftStick();
	if (inputVec.Length() < CAN_ACTION_STICKINPUT) return false;	// �X�����\���łȂ��Ȃ�

	// �J�������猩�����͂Ƃ���
	const Transform& camTrans = pCamera->GetTransform();
	DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() + 
									inputVec.y * camTrans.Forward();
	inputVec = { inputVecByCam.x, inputVecByCam.z };
	inputVec.Normalize();


	// �������擾
	const DXSimp::Vector3& playerFwd = pActionController->GetPlayer().GetTransform().Forward();
	DXSimp::Vector2 forwardXZ = { playerFwd.x, playerFwd.z };
	forwardXZ.Normalize();

	// �X�e�B�b�N�̕�������v���Ă��邩����
	float dot = inputVec.Dot(forwardXZ);

	HASHI_DEBUG_LOG(std::to_string(dot));
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

bool PlayerActState_Base::IsSpecialAtkInput(InputVector _inputVecter) const
{
	// �{�^������
	if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack)) return false;

	// �^�[�Q�b�g�����u�Ԃ͏o�Ȃ��悤�ɂ���
	if (!pActionController->GetIsTargeting() || 
		!pActionController->GetIsPrevTargeting()) return false;

	if (!IsInputVector(_inputVecter)) return false;

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

void PlayerActState_Base::ImGuiDebug()
{
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

void PlayerActState_Base::CommmonCheckTransition()
{
}

void PlayerActState_Base::CancelTransitionUpdate()
{
	//�L�����Z���t���O
	bool canCancel = pActionController->GetIsCanCancel();
	if (!canCancel) return;

	// �L�����Z������A�N�V�������Ȃ��Ȃ�
	if (cancelPlayState == PlayerState::None) return;

	// �L�����Z���̓����ɕύX
	ChangeState(cancelPlayState);
}