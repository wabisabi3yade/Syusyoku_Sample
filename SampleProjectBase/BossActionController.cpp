#include "pch.h"
#include "BossActionController.h"
#include "CP_Player.h"
#include "CP_Boss.h"
#include "CP_BattleManager.h"

#include "BossIdleState.h"
#include "BossGroundMove.h"
#include "BossDamageState.h"
#include "BossCombAttack.h"
#include "BossJumpAttack.h"

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace DXSimp = DirectX::SimpleMath;

// 状態を表すアニメーションパラメータ名
constexpr auto STATE_PARAM_NAME("state");
// のけぞりができるかアニメーションパラメータ名
constexpr auto CAN_KNOCK_PARAM_NAME("canKnock");

BossActionController::BossActionController(CP_Boss& _boss) :
	EnemyActionController(_boss, "bossActCon"), pPlayerObject(nullptr), pCanKnock(nullptr),
	defaultState(BossActState_Base::BossState::Idle)
{
	// 初期化
	disLengthList = { 0.0f };

	// ステート作成
	using enum BossActState_Base::BossState;
	using enum ActDistance;
	CreateState<BossIdleState>(Idle, {});
	CreateState<BossGroundMove>(Walk, { ActDistance::Mid });
	CreateState<BossGroundMove>(Run, { ActDistance::Far });

	CreateState<BossDamageState>(Damage_Small, {});

	CreateState<BossCombAttack>(CombAttack1, { ActDistance::Short });
	CreateState<BossJumpAttack>(JumpAttack, { ActDistance::Far });

	// デフォルトノード設定
	SetDefaultNode(static_cast<int>(defaultState));
}

void BossActionController::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
{
	EnemyActionController::Init(_pAnimation, _pRigidBody);

	// アニメーションパラメータから変数のポインタもらう
	GetAnimationParam();
}

void BossActionController::Update()
{
	// 更新できるか
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

void BossActionController::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos, bool* _pAcceptDamage)
{
	assert(pCurrentNode && "現在のノードが設定されていません");

	// 現在のノードにダメージ処理を伝える
	CastBossAct(pCurrentNode)->OnDamage();
}

void BossActionController::DebugDisplay()
{
#ifdef EDIT
	EnemyActionController::DebugDisplay();

	if (!pPlayerObject) return;

	// どの距離か表示する
	DXSimp::Vector3 bossPos = GetBoss().GetTransform().GetPosition();
	DXSimp::Vector3 playerPos = pPlayerObject->GetTransform().GetPosition();
	float dis = (bossPos - playerPos).Length();
	ActDistance curDis = JudgeActDistance(dis);

	// 線の色で表示
	DirectX::SimpleMath::Color lineColor = DXSimp::Color(1, 0, 0);
	switch (curDis)
	{
	case ActDistance::Mid:
		lineColor = DXSimp::Color(1, 1, 0);
		break;
	case ActDistance::Far:
		lineColor = DXSimp::Color(0, 1, 0);
		break;
	}

	Geometory::AddLine(bossPos, playerPos, lineColor);
#endif // EDIT
}

void BossActionController::SetPlayer(CP_Player& _playerObj)
{
	pPlayerObject = &_playerObj;
}

void BossActionController::SetAttackInfo(const HashiTaku::AttackInformation& _atkInfo)
{
	GetBoss().SetAttackInfo(_atkInfo);
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

float BossActionController::GetActDistanceLength(ActDistance _actDistance)
{
	return disLengthList[static_cast<u_int>(_actDistance)];
}

BossActionController::ActDistance BossActionController::JudgeActDistance(float _disLength) const
{
	u_int disCnt = static_cast<u_int>(ActDistance::MaxNum);
	ActDistance curDisType = static_cast<ActDistance>(disCnt - 1);	// 最長をセットしておく

	// どの距離か測る
	for (u_int d_i = 0; d_i < disCnt - 1; d_i++)
	{
		ActDistance disType = static_cast<ActDistance>(d_i);
		float dis = disLengthList[d_i];

		// 距離が範囲外なら
		if (dis < _disLength) continue;

		curDisType = disType;
		break;
	}

	return curDisType;
}

nlohmann::json BossActionController::Save()
{
	auto data = EnemyActionController::Save();
	data["defaultState"] = defaultState;

	// 距離をセーブ
	auto& disLengthDatas = data["disLengthList"];
	for (auto& disLength : disLengthList)
	{
		disLengthDatas.push_back(disLength);
	}
	return data;
}

void BossActionController::Load(const nlohmann::json& _data)
{
	EnemyActionController::Load(_data);

	bool isLoad = HashiTaku::LoadJsonEnum<BossActState_Base::BossState>(
		"defaultState",
		defaultState,
		_data);

	if (isLoad)
		SetDefaultNode(static_cast<int>(defaultState));

	// 距離をロード
	nlohmann::json disDatas;
	if (HashiTaku::LoadJsonDataArray("disLengthList", disDatas, _data))
	{
		u_int i = 0;
		for (auto& disData : disDatas)
		{
			disLengthList[i] = disData;
			i++;
		}
	}
}

BossActState_Base::BossState BossActionController::RandState(ActDistance _actDistance)
{
	// 距離のステートリストをもらう
	std::vector<BossActState_Base::BossState>* pActLists = &shortRangeActions;
	switch (_actDistance)
	{
	case ActDistance::Mid:
		pActLists = &midRangeActions;
		break;

	case ActDistance::Far:
		pActLists = &farRangeActions;
		break;
	}

	// 既に決まるような事象なら
	u_int stateCnt = static_cast<u_int>(pActLists->size());
	if (stateCnt == 0)
	{
		std::string disName = std::string(magic_enum::enum_name<ActDistance>(_actDistance));
		HASHI_DEBUG_LOG(disName + "のステートが一つもありません");
		return BossActState_Base::BossState::Run;
	}
	else if (stateCnt == 1)	// ステートが一つなら
	{
		return (*pActLists)[0];
	}

	// 確率でステートを選ぶ
	float rand = HashiTaku::Random::Range<float>(0.0f, 1.0f);
	float oneStateProbability = 1.0f / stateCnt;
	u_int arrayId = static_cast<u_int>(rand / oneStateProbability);

	return (*pActLists)[arrayId];
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
		return false;
	}
	if (!pAnimation)
	{
		return false;
	}
	if (!pRigidBody)
	{
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

	// 各距離の長さ
	if (ImGuiMethod::TreeNode("Distance"))
	{
		u_int disCnt = static_cast<u_int>(ActDistance::MaxNum);
		for (u_int d_i = 0; d_i < disCnt; d_i++)
		{
			std::string disName =
				std::string(magic_enum::enum_name<ActDistance>(static_cast<ActDistance>(d_i)));

			ImGui::DragFloat(disName.c_str(), &disLengthList[d_i], 0.01f, 0.0f, 1000.0f);
		}

		ImGui::TreePop();
	}

	// デフォルトを変更
	std::string stateName = GetStateStr(static_cast<int>(defaultState));
	if (ImGuiMethod::ComboBox("Default", stateName, stateNames))
		SetDefaultState(static_cast<BossActState_Base::BossState>(GetStateId(stateName)));
	EnemyActionController::ImGuiDebug();

	ImGui::PushID(9);
	// デバッグステート変更
	for (auto& node : stateNodeList)
	{
		if (ImGui::Button(GetStateStr(node.first).c_str()))
			ChangeState(static_cast<BossActState_Base::BossState>(node.first));
	}
	ImGui::PopID();
}

