#include "pch.h"
#include "PlayerActionController.h"
#include "CP_Animation.h"
#include "InSceneSystemManager.h"
#include "CP_BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerRollingMove.h"
#include "PlayerGuardState.h"
#include "PlayerAttackState.h"
#include "PlayerGroundAttack.h"
#include "PlayerRushAttack.h"
#include "PlayerDamageState.h"

using PlayerState = PlayerActState_Base::PlayerState;

PlayerActionController::PlayerActionController(CP_Player& _player) :
	CharacterActionController(_player, "playerAction"),
	pBattleManager(nullptr),
	pIsCanCancel(nullptr),
	pTargetObject(nullptr),
	pTargetCamera(nullptr),
	pIsCanCombAtk(nullptr),
	pDamageAtkInfo(nullptr),
	damageElapsedFrame(0),
	extensionParryFrame(4),
	isTargeting(false),
	prevIsTargeting(false),
	isParryChecking(false)
{
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pInput = &pInsceneSysytem->GetInput();

	// �s���N���X�𐶐�
	using enum PlayerState;
	CreateState<PlayerIdleState>(Idle);
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerTargetMove>(TargetMove);
	CreateState<PlayerRollingMove>(Rolling);
	CreateState<PlayerDamageState>(Damage_S);
	CreateState<PlayerDamageState>(Damage_L);
	CreateState<PlayerRollingMove>(Rolling);
	CreateState<PlayerGuardState>(Guard);
	CreateState<PlayerGroundAttack>(Attack11);
	CreateState<PlayerGroundAttack>(Attack12);
	CreateState<PlayerGroundAttack>(Attack13);
	CreateState<PlayerGroundAttack>(Attack14);
	CreateState<PlayerRushAttack>(SpecialAtkHi);
	CreateState<PlayerGroundAttack>(SpecialAtkGuard);

	// �f�t�H���g��Ԃ��Z�b�g
	SetDefaultNode(static_cast<int>(Idle));
}

PlayerActionController::~PlayerActionController()
{
	// �^�[�Q�b�g���鑤�ɍ폜����悤�ɒʒm
	if (pTargetObject)
		pTargetObject->RemoveTargeter(*this);
}
void PlayerActionController::Init(CP_Animation* _animationController,
	CP_RigidBody* _pRigidBody, IObjectTargeter* _pTargetCam)
{
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pBattleManager = CP_BattleManager::GetInstance();
	pCamera = &pInsceneSysytem->GetMainCamera();
	pTargetCamera = _pTargetCam;

	// �X�e�[�g�}�V�����ʊJ�n����
	CharacterActionController::Init(_animationController, _pRigidBody);

	// �A�j���[�V�����p�����[�^�̃A�h���X���擾
	pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);

	// ��s���͂̃A�h���X���擾
	pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);

	// �R���r�l�[�V�����U��k�\�A�h���X���擾
	pIsCanCombAtk = pAnimation->GetParameterPointer<bool>(COMBATK_PARAMNAME);
}

bool PlayerActionController::GetCanUpdate()
{
	if (!pRigidBody)
	{
		assert(!"rigidBody��ݒ肵�Ă�������");
		return false;
	}

	return true;
}

void PlayerActionController::Update()
{
#ifdef EDIT
	if (!GetCanUpdate()) return;
#endif // EDIT

	StateMachine_Base::Update();

	// �^�[�Q�b�e�B���O�̍X�V
	UpdateTargeting();
}

void PlayerActionController::CheckParryUpdate()
{
	if (!isParryChecking) return;

	damageElapsedFrame++;
	// �p���B�m�F�I��������
	if (damageElapsedFrame > extensionParryFrame)
	{
		damageElapsedFrame = 0;
		isParryChecking = false;

		assert(pDamageAtkInfo && "�G����̃_���[�W��񂪂���܂���");

		// �m�b�N��ԂɈڍs
		ChangeKnockState(*pDamageAtkInfo, atkEnemyPos);
	}
}

void PlayerActionController::OnBeginTargeting()
{
	if (!pBattleManager) return;

	// �G���X�g���擾����
	auto& enemyList = pBattleManager->GetEnemyList();
	u_int enemyCnt = static_cast<u_int>(enemyList.size());

	if (enemyCnt == 0) return;

	// ���G�������̂ł�悤�ɂ���Ȃ�ǂ̃^�[�Q�b�g�ɂ��邩�̏����������ɏ���
	// ����̓{�X1�̂Ȃ̂ł���������
	ITargetAccepter* pTargetEnemy = *enemyList.begin();

	SetTargetObject(*pTargetEnemy);

	if (pTargetCamera)	// �J�����ɂ��`����
		pTargetCamera->SetTargetObject(*pTargetEnemy);
}

void PlayerActionController::OnEndTargeting()
{
	if (pTargetCamera)	// �J�����ɂ��`����
	{
		pTargetCamera->RemoveNotify(*pTargetObject);
		pTargetObject->RemoveTargeter(*pTargetCamera);
	}

	// �^�[�Q�b�g�ŎQ�Ƃ���̂���߂�
	pTargetObject->RemoveTargeter(*this);
	pTargetObject = nullptr;
}

void PlayerActionController::UpdateTargeting()
{
	// �{�^�������ꂽ��^�[�Q�b�g��
	isTargeting = pInput->GetButton(GameInput::ButtonType::Player_RockOn);

	// �A�j���[�V�����p�����[�^�ɂ�����
	pAnimation->SetBool(TARGET_PARAMNAME, isTargeting);

	if (isTargeting && !prevIsTargeting)	// �^�[�Q�b�g�����u��
		OnBeginTargeting();
	else if (!isTargeting && prevIsTargeting)	// �^�[�Q�b�g�����̏u��
		OnEndTargeting();

	// ���t���[���ׂ̈ɍX�V
	prevIsTargeting = isTargeting;
}

bool PlayerActionController::ChangeState(const PlayerState& _nextActionState, bool _isForce)
{
	// �X�e�[�g�}�V���ŕύX
	bool isSuccess = StateMachine_Base::ChangeNode(static_cast<int>(_nextActionState), _isForce);
	if (!isSuccess) return false;

	// �A�j���[�V��������state�ϐ����ύX
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));

	return true;
}

void PlayerActionController::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	// �p���B�ł�����ڍs�̏����͍s��Ȃ�
	if (OnDamageParryCheck()) return;

	// �m�b�N��ԂɈڍs
	ChangeKnockState(_atkInfo, _attackerPos);
}

bool PlayerActionController::GetIsTargeting() const
{
	return isTargeting;
}

bool PlayerActionController::GetIsPrevTargeting() const
{
	return prevIsTargeting;
}

bool PlayerActionController::GetIsCanCancel() const
{
	return *pIsCanCancel;
}

bool PlayerActionController::GetCanInput() const
{
	return *pIsCanInput;
}

bool PlayerActionController::GetCanCombAtk() const
{
	return *pIsCanCombAtk;
}

CP_Player& PlayerActionController::GetPlayer()
{
	return static_cast<CP_Player&>(*pCharacter);
}

CP_Camera& PlayerActionController::GetCamera()
{
	return *pCamera;
}

PlayerActState_Base& PlayerActionController::CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase)
{
	return static_cast<PlayerActState_Base&>(_stateNodeBase);
}

bool PlayerActionController::OnDamageParryCheck()
{
	// ���p���B�ł��邩�m�F����
	// ���K�[�h��ԂłȂ��Ȃ�
	PlayerState currentState = static_cast<PlayerState>(currentStateKey);
	if (currentState != PlayerState::Guard) return false;

	// �p���B�ł���t���[�������m�F
	PlayerGuardState& guardState = static_cast<PlayerGuardState&>(*pCurrentNode);
	if (!guardState.GetCanParry()) return false;

	// �p���B����
	guardState.OnParry();
	return true;
}

void PlayerActionController::ChangeKnockState(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	using enum HashiTaku::AttackInformation::AttackLevel;
	using PlayerState = PlayerState;

	// �m�b�N�̃x�N�g�������߂�
	DirectX::SimpleMath::Vector3 knockVec =
		GetPlayer().GetTransform().GetPosition() - _attackerPos;
	knockVec.y = 0.0f; knockVec.Normalize();

	// �_���[�W�̃��x���ł̂����胂�[�V������ύX
	switch (_atkInfo.GetAttackLevel())
	{

		// ���̂�����
	case Low:
	case Mid:
	{
		// �m�b�N�̃x�N�g�����Z�b�g
		auto& damageNode = stateNodeList[static_cast<int>(PlayerState::Damage_S)];
		static_cast<PlayerDamageState&>(*damageNode).SetKnockVec(knockVec);
	}

	ChangeState(PlayerState::Damage_S, true);
	break;

	// ��̂�����
	case High:
	case SuperHigh:
	{
		// �m�b�N�̃x�N�g�����Z�b�g
		auto& damageNode = stateNodeList[static_cast<int>(PlayerState::Damage_L)];
		static_cast<PlayerDamageState&>(*damageNode).SetKnockVec(knockVec);
	}

	ChangeState(PlayerState::Damage_L, true);
	break;

	default:
		break;
	}
}

void PlayerActionController::ImGuiDebug()
{
	ImGuiMethod::Text("isTargeting", isTargeting);
	ImGui::DragInt("CanParryFrame", &extensionParryFrame, 1.f, 0, 1000);

	CharacterActionController::ImGuiDebug();
}

PlayerActState_Base* PlayerActionController::GetCurrentAction()
{
	return static_cast<PlayerActState_Base*>(pCurrentNode);
}

std::string PlayerActionController::GetStateStr(int _stateId)
{
	PlayerState state =
		static_cast<PlayerState>(_stateId);

	std::string str = std::string(magic_enum::enum_name<PlayerState>(state));

	return str;
}

int PlayerActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<PlayerState>(_stateName);
	if (!state.has_value())
	{
		assert("����ɏ�Ԃ��擾�ł��܂���");
		return -1;
	}

	return static_cast<int>(state.value());
}

ITargetAccepter* PlayerActionController::GetTargetObject()
{
	return pTargetObject;
}

void PlayerActionController::SetTargetObject(ITargetAccepter& _targetObject)
{
	pTargetObject = &_targetObject;
	pTargetObject->SetTargeter(*this);
}

void PlayerActionController::RemoveNotify(const ITargetAccepter& _deathTargetObj)
{
	// ���񂾃^�[�Q�b�g�悪���݌��Ă���I�u�W�F�N�g��������Q�Ƃ�����
	if (!pTargetObject) return;
	if (pTargetObject != &_deathTargetObj) return;

	pTargetObject = nullptr;
}

