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
#include "PlayerGroundChargeAttack.h"
#include "PlayerDamageState.h"
#include "PlayerJumpState.h"

namespace HashiTaku
{
	PlayerGroundActionController::PlayerGroundActionController(
		PlayerAction& _pAction,
		CP_Player& _player) :
		PlayerActionController_Base(_pAction, _player, "playerGroundController")
	{
		place = ActionPlace::Ground;

		// 行動クラスを生成(下に行くほど優先度高め)
		using enum GroundState;
		CreateState<PlayerIdleState>(Idle, CancelType::None);

		CreateState<PlayerMoveState>(Move, CancelType::Move);
		CreateState<PlayerTargetMove>(TargetMove, CancelType::Move);

		CreateState<PlayerAttackState>(Attack11, CancelType::Attack);
		CreateState<PlayerAttackState>(Attack12, CancelType::Attack);
		CreateState<PlayerAttackState>(Attack13, CancelType::Attack);
		CreateState<PlayerAttackState>(Attack14, CancelType::Attack);
		CreateState<PlayerRushAttack>(SpecialAtkHi, CancelType::Attack);
		CreateState<PlayerAttackState>(SpecialAtkGuard, CancelType::Attack);
		CreateState<PlayerGroundChargeAttack>(ChargeAttack1, CancelType::Attack);

		CreateState<PlayerJumpState>(BeginJump, CancelType::Action);
		CreateState<PlayerGuardState>(Guard, CancelType::Action);
		CreateState<PlayerRollingMove>(Rolling, CancelType::Action);

		CreateState<PlayerDamageState>(Damage_S, CancelType::None);
		CreateState<PlayerDamageState>(Damage_L, CancelType::None);

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

	void PlayerGroundActionController::OnDamage(AttackInformation& _atkInfo,
		bool* _pAcceptDamage)
	{
		if (_pAcceptDamage)
			*_pAcceptDamage = false;

		// 各状態のダメージ処理を行い、ダメージ処理を行わないなら以降は処理しない
		if (!GetCurrentGroundState()->OnDamage(_atkInfo)) return;

		// ノック状態に移行
		ChangeKnockState(_atkInfo);

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

	PlayerGroundState* PlayerGroundActionController::GetCurrentGroundState()
	{
		// 現在の状態を取得する
		StateNode_Base* curNode = GetCurrentNode();
		if (!curNode) return nullptr;

		// キャストして地上行動にする
		return static_cast<PlayerGroundState*>(curNode);
	}

	PlayerGroundState::PlayerState PlayerGroundActionController::GetCurrentStateType() const
	{
		return static_cast<PlayerGroundState::PlayerState>(currentStateKey);
	}

	void PlayerGroundActionController::ChangeKnockState(const AttackInformation& _atkInfo)
	{
		using enum AttackInformation::AttackLevel;

		// デバッグ無敵ならノック状態に移行しない
		if (CP_Player::GetIsDebugInvincible()) return;

		// ノックのベクトルを求める
		DXSimp::Vector3 knockVec =
			GetMyTransform().GetPosition() - _atkInfo.GetAttackerWorldPos();
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
}