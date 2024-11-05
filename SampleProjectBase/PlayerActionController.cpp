#include "pch.h"
#include "PlayerActionController.h"
#include "CP_Animation.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerAttackState.h"
#include "PlayerGroundAttack.h"
#include "PlayerRushAttack.h"

PlayerActionController::PlayerActionController(CP_Player& _player)
	: StateMachine_Base("playerAction"), pAnimation(nullptr), pPlayer(&_player)
{
	// 状態遷移オブザーバー生成
	pStateChangeObserver = std::make_unique<PlayerActChangeObserver>("StateChangeObserver", *this);

	// アニメーション変更オブザーバー生成
	pChangeAnimObserver = std::make_unique<PlayerChangeAnimObserver>(*this);

	// 行動クラスを生成
	using enum PlayerActState_Base::PlayerState;
	CreateState<PlayerIdleState>(Idle);
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerTargetMove>(TargetMove);
	CreateState<PlayerGroundAttack>(Attack11);
	CreateState<PlayerGroundAttack>(Attack12);
	CreateState<PlayerGroundAttack>(Attack13);
	CreateState<PlayerGroundAttack>(Attack14);
	CreateState<PlayerRushAttack>(SpecialAtkHi);

	// デフォルト状態をセット
	SetDefaultNode(Idle);
}
void PlayerActionController::Begin(CP_Animation& _animationController)
{
	// ステートマシン共通開始処理
	StateMachine_Base::Begin();

	// アニメーションコントローラーを各ステートに渡す
	pAnimation = &_animationController;
	for (auto& stateNode : stateNodeList)
	{
		PlayerActState_Base& playerAct = CastPlayerAct(*stateNode.second);
		playerAct.SetAnimation(_animationController);
	}
}

void PlayerActionController::Update()
{
	pCurrentNode->Update();
}

bool PlayerActionController::ChangeNode(const PlayerActState_Base::PlayerState& _nextActionState)
{
	// ステートマシンで変更
	bool isSuccess = StateMachine_Base::ChangeNode(_nextActionState);
	if (!isSuccess) return false;

	// アニメーション側のstate変数も変更
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));
	
	return true;
}

PlayerActState_Base& PlayerActionController::CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase)
{
	return static_cast<PlayerActState_Base&>(_stateNodeBase);
}

void PlayerActionController::ImGuiSetting()
{
	// 現在の状態表示
	std::string text = "NowState:" + std::string(magic_enum::enum_name(currentStateKey));
	ImGui::Text(text.c_str());

	for (auto& pAct : stateNodeList)	// 各アクションの調整
	{
		std::string stateStr = std::string(magic_enum::enum_name(pAct.first));
		if (ImGuiMethod::TreeNode(stateStr))
		{
			CastPlayerAct(*pAct.second).ImGuiCall();

			ImGui::TreePop();
		}
	}
}

PlayerActState_Base* PlayerActionController::GetCurrentAction()
{
	return static_cast<PlayerActState_Base*>(pCurrentNode);
}

PlayerChangeAnimObserver& PlayerActionController::GetChangeAnimObserver()
{
	return *pChangeAnimObserver;
}

nlohmann::json PlayerActionController::Save()
{
	nlohmann::json data;

	for (auto& node : stateNodeList)
	{
		nlohmann::json actData;
		// 文字列に変換
		actData["typeString"] = magic_enum::enum_name(node.first);
		actData["data"] = CastPlayerAct(*node.second).Save();
		data["actData"].push_back(actData);
	}

	return data;
}

void PlayerActionController::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	nlohmann::json actDataList;
	// ステートごとのパラメータをロードする
	if (LoadJsonDataArray("actData", actDataList, _data))
	{
		for (auto& actData : actDataList)
		{
			std::string stateString;
			if (!LoadJsonString("typeString", stateString , actData))
				continue;

			PlayerActState_Base::PlayerState playerState;
			// 文字列->StateType
			auto state = magic_enum::enum_cast<PlayerActState_Base::PlayerState>(stateString);
			if (state.has_value())
				playerState = state.value();
			else
				continue;

			nlohmann::json actParam;
			if (!LoadJsonData("data", actParam, actData))
				continue;

			if (stateNodeList.contains(playerState))
				CastPlayerAct(*stateNodeList[playerState]).Load(actParam);
		}
	}
}

PlayerChangeAnimObserver::PlayerChangeAnimObserver(PlayerActionController& _playerActCon)
	: ChangeAnimObserver("playerChangeAnimObserver"), pActionController(&_playerActCon)
{
}

void PlayerChangeAnimObserver::ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value)
{
	// 現在再生しているアクションにのみ通知する
	auto curAction = pActionController->GetCurrentAction();
	if (!curAction)
	{
		HASHI_DEBUG_LOG(GetObserverName() + 
			"再生されているアクションがないためアニメーション変更通知がおこなえません");

		return;
	}

	// 通知する
	curAction->OnAnimationEnd(*_value.pFromAnimNodeName, *_value.pToAnimNodeName);

	HASHI_DEBUG_LOG("from:" + *_value.pFromAnimNodeName + "\nto:" + *_value.pToAnimNodeName);
}
