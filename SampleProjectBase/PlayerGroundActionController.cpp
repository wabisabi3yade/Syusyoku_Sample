#include "pch.h"
#include "PlayerGroundActionController.h"
#include "CP_Player.h"
#include "PlayerAction.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerRollingMove.h"
#include "PlayerGuardState.h"
#include "PlayerRushAttack.h"
#include "PlayerDamageState.h"
#include "PlayerJumpState.h"

namespace DXSimp = DirectX::SimpleMath;

PlayerGroundActionController::PlayerGroundActionController(
	PlayerAction& _pAction,
	CP_Player& _player) :
	PlayerActionController_Base(_pAction, _player, "playerGroundController")
{
	place = ActionPlace::Ground;

	// �s���N���X�𐶐�(���ɍs���قǗD��x����)
	using enum GroundState;
	CreateState<PlayerIdleState>(Idle);

	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerTargetMove>(TargetMove);

	CreateState<PlayerAttackState>(Attack11);
	CreateState<PlayerAttackState>(Attack12);
	CreateState<PlayerAttackState>(Attack13);
	CreateState<PlayerAttackState>(Attack14);
	CreateState<PlayerRushAttack>(SpecialAtkHi);
	CreateState<PlayerAttackState>(SpecialAtkGuard);

	CreateState<PlayerJumpState>(BeginJump);
	CreateState<PlayerGuardState>(Guard);
	CreateState<PlayerRollingMove>(Rolling);

	CreateState<PlayerDamageState>(Damage_S);
	CreateState<PlayerDamageState>(Damage_L);

	// �f�t�H���g��Ԃ��Z�b�g
	SetDefaultNode(static_cast<int>(Idle));
}

PlayerGroundActionController::~PlayerGroundActionController()
{
}

void PlayerGroundActionController::Init(CP_Animation* _animationController,
	CP_RigidBody* _pRigidBody)
{
	// �v���C���[�A�N�V�������ʊJ�n����
	PlayerActionController_Base::Init(_animationController, _pRigidBody);
}

bool PlayerGroundActionController::GetCanUpdate()
{
	if (!pRigidBody)
	{
		assert(!"rigidBody��ݒ肵�Ă�������");
		return false;
	}

	return true;
}

void PlayerGroundActionController::Update()
{
	PlayerActionController_Base::Update();
}

bool PlayerGroundActionController::ChangeGroundState(const GroundState& _nextActionState, bool _isForce)
{
	return ChangeState(static_cast<int>(_nextActionState), _isForce);
}

void PlayerGroundActionController::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos, bool* _pAcceptDamage)
{
	if (_pAcceptDamage)
		*_pAcceptDamage = false;

	// �p���B�ł�����ڍs�̏����͍s��Ȃ�
	if (OnDamageParryCheck(_attackerPos)) return;

	// �m�b�N��ԂɈڍs
	ChangeKnockState(_atkInfo, _attackerPos);

	if (_pAcceptDamage)
		*_pAcceptDamage = true;
}

bool PlayerGroundActionController::GetIsTargeting() const
{
	return pAction->GetIsTargeting();
}

CP_Camera& PlayerGroundActionController::GetCamera()
{
	return pAction->GetCamera();
}

PlayerGroundState::PlayerState PlayerGroundActionController::GetCurrentState() const
{
	return static_cast<PlayerGroundState::PlayerState>(currentStateKey);
}

bool PlayerGroundActionController::OnDamageParryCheck(const DirectX::SimpleMath::Vector3& _enemyPos)
{
	// ���p���B�ł��邩�m�F����
	// ���K�[�h��ԂłȂ��Ȃ�
	GroundState currentState = static_cast<GroundState>(currentStateKey);
	if (currentState != GroundState::Guard) return false;

	// �p���B�ł���t���[�������m�F
	PlayerGuardState& guardState = static_cast<PlayerGuardState&>(*pCurrentNode);
	if (!guardState.GetCanParry(_enemyPos)) return false;

	// �p���B����
	guardState.OnParry();
	return true;
}

void PlayerGroundActionController::ChangeKnockState(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	using enum HashiTaku::AttackInformation::AttackLevel;

	// �m�b�N�̃x�N�g�������߂�
	DirectX::SimpleMath::Vector3 knockVec =
		GetMyTransform().GetPosition() - _attackerPos;
	knockVec.y = 0.0f; knockVec.Normalize();

	// �_���[�W�̃��x���ł̂����胂�[�V������ύX
	switch (_atkInfo.GetAttackLevel())
	{

		// ���̂�����
	case Low:
	case Mid:
	{
		// �m�b�N�̃x�N�g�����Z�b�g
		auto& damageNode = stateNodeList[static_cast<int>(GroundState::Damage_S)];
		static_cast<PlayerDamageState&>(*damageNode).SetKnockVec(knockVec);
	}

	ChangeGroundState(GroundState::Damage_S, true);
	break;

	// ��̂�����
	case High:
	case SuperHigh:
	{
		// �m�b�N�̃x�N�g�����Z�b�g
		auto& damageNode = stateNodeList[static_cast<int>(GroundState::Damage_L)];
		static_cast<PlayerDamageState&>(*damageNode).SetKnockVec(knockVec);
	}

	ChangeGroundState(GroundState::Damage_L, true);
	break;

	default:
		break;
	}
}

void PlayerGroundActionController::ImGuiDebug()
{
	CharacterActionController::ImGuiDebug();
}

std::string PlayerGroundActionController::GetStateStr(int _stateId)
{
	GroundState state =
		static_cast<GroundState>(_stateId);

	std::string str = std::string(magic_enum::enum_name<GroundState>(state));

	return str;
}

int PlayerGroundActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<GroundState>(_stateName);
	if (!state.has_value())
	{
		assert("����ɏ�Ԃ��擾�ł��܂���");
		return -1;
	}

	return static_cast<int>(state.value());
}
