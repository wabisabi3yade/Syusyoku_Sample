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

	// 行動クラスを生成(下に行くほど優先度高め)
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

	// デフォルト状態をセット
	SetDefaultNode(static_cast<int>(Idle));
}

PlayerGroundActionController::~PlayerGroundActionController()
{
}

void PlayerGroundActionController::Init(CP_Animation* _animationController,
	CP_RigidBody* _pRigidBody)
{
	// プレイヤーアクション共通開始処理
	PlayerActionController_Base::Init(_animationController, _pRigidBody);
}

bool PlayerGroundActionController::GetCanUpdate()
{
	if (!pRigidBody)
	{
		assert(!"rigidBodyを設定してください");
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

	// パリィできたら移行の処理は行わない
	if (OnDamageParryCheck(_attackerPos)) return;

	// ノック状態に移行
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
	// 今パリィできるか確認する
	// 今ガード状態でないなら
	GroundState currentState = static_cast<GroundState>(currentStateKey);
	if (currentState != GroundState::Guard) return false;

	// パリィできるフレーム内か確認
	PlayerGuardState& guardState = static_cast<PlayerGuardState&>(*pCurrentNode);
	if (!guardState.GetCanParry(_enemyPos)) return false;

	// パリィ処理
	guardState.OnParry();
	return true;
}

void PlayerGroundActionController::ChangeKnockState(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	using enum HashiTaku::AttackInformation::AttackLevel;

	// ノックのベクトルを求める
	DirectX::SimpleMath::Vector3 knockVec =
		GetMyTransform().GetPosition() - _attackerPos;
	knockVec.y = 0.0f; knockVec.Normalize();

	// ダメージのレベルでのけぞりモーションを変更
	switch (_atkInfo.GetAttackLevel())
	{

		// 小のけぞり
	case Low:
	case Mid:
	{
		// ノックのベクトルをセット
		auto& damageNode = stateNodeList[static_cast<int>(GroundState::Damage_S)];
		static_cast<PlayerDamageState&>(*damageNode).SetKnockVec(knockVec);
	}

	ChangeGroundState(GroundState::Damage_S, true);
	break;

	// 大のけぞり
	case High:
	case SuperHigh:
	{
		// ノックのベクトルをセット
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
		assert("正常に状態を取得できません");
		return -1;
	}

	return static_cast<int>(state.value());
}
