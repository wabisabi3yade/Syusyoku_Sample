#include "pch.h"
#include "BossActionController.h"
#include "CP_Player.h"
#include "CP_Boss.h"
#include "CP_BattleManager.h"

#include "BossIdleState.h"
#include "BossGroundMove.h"
#include "BossGroundAttack.h"
#include "BossDamageState.h"

// 状態を表すアニメーションパラメータ名
constexpr auto STATE_PARAM_NAME("state");	
// のけぞりができるかアニメーションパラメータ名
constexpr auto CAN_KNOCK_PARAM_NAME("canKnock");	

BossActionController::BossActionController(CP_Boss& _boss): 
	EnemyActionController(_boss, "bossActCon"), pPlayerObject(nullptr), pCanKnock(nullptr),
	defaultState(BossActState_Base::BossState::Idle)
{
	// ステート作成
	using enum BossActState_Base::BossState;
	CreateState<BossIdleState>(Idle);
	CreateState<BossGroundMove>(Walk);
	CreateState<BossGroundMove>(Run);
	CreateState<BossGroundAttack>(Attack1);
	CreateState<BossDamageState>(Damage_Small);

	// デフォルトノード設定
	SetDefaultNode(static_cast<int>(defaultState));
}

void BossActionController::Init(CP_Animation* _pAnimation)
{
	EnemyActionController::Init(_pAnimation);

	// アニメーションパラメータから変数のポインタもらう
	GetAnimationParam();
}

void BossActionController::Update()
{
	if (!IsCanBossUpdate()) return;

	EnemyActionController::Update();
}

bool BossActionController::ChangeState(BossActState_Base::BossState _nextState, bool _isForce)
{
	if (!ChangeNode(static_cast<int>(_nextState), _isForce)) return false;
		
	// アニメーションにも渡す
	pAnimation->SetInt(STATE_PARAM_NAME, currentStateKey);
	return true;
}

void BossActionController::OnDamage()
{
	assert(pCurrentNode && "現在のノードが設定されていません");

	// 現在のノードにダメージ処理を伝える
	CastBossAct(pCurrentNode)->OnDamage();
}

void BossActionController::SetPlayer(CP_Player& _playerObj)
{
	pPlayerObject = &_playerObj;
}

CP_Boss& BossActionController::GetBoss()
{
	return static_cast<CP_Boss&>(GetCharacter());
}

CP_Player& BossActionController::GetPlayer()
{
	assert(pPlayerObject && "プレイヤーが設定されていません");

	return *pPlayerObject;
}

const bool BossActionController::GetCanKnock() const
{
	if (!pCanKnock) return false;

	return *pCanKnock;
}

nlohmann::json BossActionController::Save()
{
	auto data = EnemyActionController::Save();
	data["defaultState"] = defaultState;
	return data;
}

void BossActionController::Load(const nlohmann::json& _data)
{
	EnemyActionController::Load(_data);

	HashiTaku::LoadJsonEnum<BossActState_Base::BossState>(
		"defaultState", 
		defaultState,
		_data);
}

void BossActionController::GetAnimationParam()
{
	// のけぞりできるか？
	pCanKnock = pAnimation->GetParameterPointer<bool>(CAN_KNOCK_PARAM_NAME);
}

bool BossActionController::IsCanBossUpdate()
{
#ifdef EDIT
	if (!pPlayerObject)
	{
		assert(!"プレイヤーセットしてください");
		return false;
	}
	if (!pAnimation)
	{
		assert(!"アニメーションセットしてください");
		return false;
	}
#endif

	return true;
}

BossActState_Base* BossActionController::CastBossAct(HashiTaku::StateNode_Base* _pBaseNode)
{
	if (!_pBaseNode) return nullptr;

	return static_cast<BossActState_Base*>(_pBaseNode);
}

void BossActionController::SetDefaultState(BossActState_Base::BossState _defaultState)
{
	defaultState = _defaultState;
	SetDefaultNode(static_cast<int>(defaultState));
}

std::string BossActionController::GetStateStr(int _stateId)
{
	BossActState_Base::BossState state =
		static_cast<BossActState_Base::BossState>(_stateId);

	return std::string(magic_enum::enum_name<BossActState_Base::BossState>(state));
}

int BossActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<BossActState_Base::BossState>(_stateName);
	if (!state.has_value())
	{
		assert("正常に状態を取得できません");
		return -1;
	}

	return static_cast<int>(state.value());
}

void BossActionController::ImGuiDebug()
{
	// 名前取得
	std::vector<std::string> stateNames;
	for (auto& node : stateNodeList)
	{
		stateNames.push_back(GetStateStr(node.first));
	}

	// デフォルトを変更
	std::string stateName = GetStateStr(static_cast<int>(defaultState));
	if (ImGuiMethod::ComboBox("Default", stateName, stateNames))
		SetDefaultState(static_cast<BossActState_Base::BossState>(GetStateId(stateName)));

	EnemyActionController::ImGuiDebug();
}

