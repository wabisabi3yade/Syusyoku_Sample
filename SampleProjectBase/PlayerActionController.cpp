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

	// 行動クラスを生成
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

	// デフォルト状態をセット
	SetDefaultNode(static_cast<int>(Idle));
}

PlayerActionController::~PlayerActionController()
{
	// ターゲットする側に削除するように通知
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

	// ステートマシン共通開始処理
	CharacterActionController::Init(_animationController, _pRigidBody);

	// アニメーションパラメータのアドレスを取得
	pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);

	// 先行入力のアドレスを取得
	pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);

	// コンビネーション攻撃k可能アドレスを取得
	pIsCanCombAtk = pAnimation->GetParameterPointer<bool>(COMBATK_PARAMNAME);
}

bool PlayerActionController::GetCanUpdate()
{
	if (!pRigidBody)
	{
		assert(!"rigidBodyを設定してください");
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

	// ターゲッティングの更新
	UpdateTargeting();
}

void PlayerActionController::CheckParryUpdate()
{
	if (!isParryChecking) return;

	damageElapsedFrame++;
	// パリィ確認終了したら
	if (damageElapsedFrame > extensionParryFrame)
	{
		damageElapsedFrame = 0;
		isParryChecking = false;

		assert(pDamageAtkInfo && "敵からのダメージ情報がありません");

		// ノック状態に移行
		ChangeKnockState(*pDamageAtkInfo, atkEnemyPos);
	}
}

void PlayerActionController::OnBeginTargeting()
{
	if (!pBattleManager) return;

	// 敵リストを取得する
	auto& enemyList = pBattleManager->GetEnemyList();
	u_int enemyCnt = static_cast<u_int>(enemyList.size());

	if (enemyCnt == 0) return;

	// ↓敵が複数体でるようにするならどのターゲットにするかの処理をここに書く
	// 今回はボス1体なのでそいつを見る
	ITargetAccepter* pTargetEnemy = *enemyList.begin();

	SetTargetObject(*pTargetEnemy);

	if (pTargetCamera)	// カメラにも伝える
		pTargetCamera->SetTargetObject(*pTargetEnemy);
}

void PlayerActionController::OnEndTargeting()
{
	if (pTargetCamera)	// カメラにも伝える
	{
		pTargetCamera->RemoveNotify(*pTargetObject);
		pTargetObject->RemoveTargeter(*pTargetCamera);
	}

	// ターゲットで参照するのをやめる
	pTargetObject->RemoveTargeter(*this);
	pTargetObject = nullptr;
}

void PlayerActionController::UpdateTargeting()
{
	// ボタン押されたらターゲット中
	isTargeting = pInput->GetButton(GameInput::ButtonType::Player_RockOn);

	// アニメーションパラメータにも送る
	pAnimation->SetBool(TARGET_PARAMNAME, isTargeting);

	if (isTargeting && !prevIsTargeting)	// ターゲットした瞬間
		OnBeginTargeting();
	else if (!isTargeting && prevIsTargeting)	// ターゲット解除の瞬間
		OnEndTargeting();

	// 次フレームの為に更新
	prevIsTargeting = isTargeting;
}

bool PlayerActionController::ChangeState(const PlayerState& _nextActionState, bool _isForce)
{
	// ステートマシンで変更
	bool isSuccess = StateMachine_Base::ChangeNode(static_cast<int>(_nextActionState), _isForce);
	if (!isSuccess) return false;

	// アニメーション側のstate変数も変更
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));

	return true;
}

void PlayerActionController::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	// パリィできたら移行の処理は行わない
	if (OnDamageParryCheck()) return;

	// ノック状態に移行
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
	// 今パリィできるか確認する
	// 今ガード状態でないなら
	PlayerState currentState = static_cast<PlayerState>(currentStateKey);
	if (currentState != PlayerState::Guard) return false;

	// パリィできるフレーム内か確認
	PlayerGuardState& guardState = static_cast<PlayerGuardState&>(*pCurrentNode);
	if (!guardState.GetCanParry()) return false;

	// パリィ処理
	guardState.OnParry();
	return true;
}

void PlayerActionController::ChangeKnockState(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	using enum HashiTaku::AttackInformation::AttackLevel;
	using PlayerState = PlayerState;

	// ノックのベクトルを求める
	DirectX::SimpleMath::Vector3 knockVec =
		GetPlayer().GetTransform().GetPosition() - _attackerPos;
	knockVec.y = 0.0f; knockVec.Normalize();

	// ダメージのレベルでのけぞりモーションを変更
	switch (_atkInfo.GetAttackLevel())
	{

		// 小のけぞり
	case Low:
	case Mid:
	{
		// ノックのベクトルをセット
		auto& damageNode = stateNodeList[static_cast<int>(PlayerState::Damage_S)];
		static_cast<PlayerDamageState&>(*damageNode).SetKnockVec(knockVec);
	}

	ChangeState(PlayerState::Damage_S, true);
	break;

	// 大のけぞり
	case High:
	case SuperHigh:
	{
		// ノックのベクトルをセット
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
		assert("正常に状態を取得できません");
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
	// 死んだターゲット先が現在見ているオブジェクトだったら参照を消す
	if (!pTargetObject) return;
	if (pTargetObject != &_deathTargetObj) return;

	pTargetObject = nullptr;
}

