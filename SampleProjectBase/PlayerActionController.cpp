#include "pch.h"
#include "PlayerActionController.h"
#include "CP_Animation.h"
#include "InSceneSystemManager.h"
#include "CP_BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerRollingMove.h"
#include "PlayerAttackState.h"
#include "PlayerGroundAttack.h"
#include "PlayerRushAttack.h"

PlayerActionController::PlayerActionController(CP_Player& _player) :
	CharacterActionController(_player, "playerAction"), pBattleManager(nullptr),
	pIsCanCancel(nullptr), pTargetObject(nullptr), pIsCanCombAtk(nullptr), 
	isTargeting(false), prevIsTargeting(false)
{
	pInput = &InSceneSystemManager::GetInstance()->GetInput();

	// 行動クラスを生成
	using enum PlayerActState_Base::PlayerState;
	CreateState<PlayerIdleState>(Idle);
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerTargetMove>(TargetMove);
	CreateState<PlayerRollingMove>(Rolling);
	CreateState<PlayerGroundAttack>(Attack11);
	CreateState<PlayerGroundAttack>(Attack12);
	CreateState<PlayerGroundAttack>(Attack13);
	CreateState<PlayerGroundAttack>(Attack14);
	CreateState<PlayerRushAttack>(SpecialAtkHi);

	// デフォルト状態をセット
	SetDefaultNode(static_cast<int>(Idle));
}
void PlayerActionController::Init(CP_Animation* _animationController,
	CP_RigidBody* _pRigidBody)
{
	pBattleManager = CP_BattleManager::GetInstance();

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

	pCurrentNode->Update();

	UpdateTargeting();
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

	SetTargetObject(*(*enemyList.begin()));
}

void PlayerActionController::OnEndTargeting()
{
	// ターゲットで参照するのをやめる
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

bool PlayerActionController::ChangeState(const PlayerActState_Base::PlayerState& _nextActionState)
{
	// ステートマシンで変更
	bool isSuccess = StateMachine_Base::ChangeNode(static_cast<int>(_nextActionState));
	if (!isSuccess) return false;

	// アニメーション側のstate変数も変更
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));

	return true;
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

PlayerActState_Base& PlayerActionController::CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase)
{
	return static_cast<PlayerActState_Base&>(_stateNodeBase);
}

void PlayerActionController::ImGuiDebug()
{
	ImGuiMethod::Text("isTargeting", isTargeting);

	CharacterActionController::ImGuiDebug();
}

PlayerActState_Base* PlayerActionController::GetCurrentAction()
{
	return static_cast<PlayerActState_Base*>(pCurrentNode);
}

std::string PlayerActionController::GetStateStr(int _stateId)
{
	PlayerActState_Base::PlayerState state =
		static_cast<PlayerActState_Base::PlayerState>(_stateId);

	return std::string(magic_enum::enum_name<PlayerActState_Base::PlayerState>(state));
}

int PlayerActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<PlayerActState_Base::PlayerState>(_stateName);
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
}

void PlayerActionController::UpdateDeathNotify(const ITargetAccepter& _deathTargetObj)
{
	// 死んだターゲット先が現在見ているオブジェクトだったら参照を消す
	if (!pTargetObject) return;
	if (pTargetObject != &_deathTargetObj) return;

	pTargetObject = nullptr;
}

