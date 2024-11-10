#include "pch.h"
#include "BossActionController.h"
#include "CP_Player.h"
#include "CP_Boss.h"
#include "CP_BattleManager.h"

#include "BossIdleState.h"
#include "BossGroundMove.h"
#include "BossGroundAttack.h"

constexpr auto STATE_PARAM_NAME("state");	// 状態を表すアニメーションパラメータ名

BossActionController::BossActionController(CP_Boss& _boss)
	: EnemyActionController(_boss, "bossActCon"), pPlayerObject(nullptr)
{
	// ステート作成
	using enum BossActState_Base::BossState;
	CreateState<BossIdleState>(Idle);
	CreateState<BossGroundMove>(Walk);
	CreateState<BossGroundMove>(Run);
	CreateState<BossGroundAttack>(Attack1);

	// デフォルトノード設定
	SetDefaultNode(static_cast<int>(Run));
}

void BossActionController::Init(CP_Animation& _animationController)
{
	EnemyActionController::Init(_animationController);
}

void BossActionController::Update()
{
	if (!IsCanBossUpdate()) return;

	EnemyActionController::Update();
}

bool BossActionController::ChangeState(BossActState_Base::BossState _nextState)
{
	if (!ChangeNode(static_cast<int>(_nextState))) return false;
		
	// アニメーションにも渡す
	pAnimation->SetInt(STATE_PARAM_NAME, currentStateKey);
	return true;
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

nlohmann::json BossActionController::Save()
{
	nlohmann::json data;

	for (auto& node : stateNodeList)
	{

		BossActState_Base::BossState bossState = static_cast<BossActState_Base::BossState>(node.first);
		nlohmann::json actData;
		// 文字列に変換
		actData["typeString"] = magic_enum::enum_name(bossState);
		actData["data"] = static_cast<BossActState_Base&>(*node.second).Save();
		data["actData"].push_back(actData);
	}

	return data;
}

void BossActionController::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	nlohmann::json actDataList;
	// ステートごとのパラメータをロードする
	if (LoadJsonDataArray("actData", actDataList, _data))
	{
		for (auto& actData : actDataList)
		{
			std::string stateString;
			if (!LoadJsonString("typeString", stateString, actData))
				continue;

			BossActState_Base::BossState bossState;
			// 文字列->StateType
			auto state = magic_enum::enum_cast<BossActState_Base::BossState>(stateString);
			if (state.has_value())
				bossState = state.value();
			else
				continue;

			nlohmann::json actParam;
			if (!LoadJsonData("data", actParam, actData))
				continue;

			int stateId = static_cast<int>(bossState);
			if (stateNodeList.contains(stateId))
				static_cast<BossActState_Base&>(*stateNodeList[stateId]).Load(actParam);
		}
	}
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

void BossActionController::ImGuiDebug()
{
	EnemyActionController::ImGuiDebug();

	// 各ステート
	for (auto& node : stateNodeList)
	{
		BossActState_Base::BossState state = static_cast<BossActState_Base::BossState>(node.first);
		if (ImGuiMethod::TreeNode(std::string(magic_enum::enum_name(state))))
		{
			static_cast<BossActState_Base&>(*node.second).ImGuiCall();

			ImGui::TreePop();
		}
	}
}

