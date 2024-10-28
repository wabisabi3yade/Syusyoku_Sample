#include "pch.h"
#include "PlayerActionController.h"
#include "CP_Animation.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerAttackState.h"

PlayerActionController::PlayerActionController(GameObject& _pPlayerObject)
	: StateMachine_Base("playerAction"), pAnimation(nullptr), pPlayerObject(&_pPlayerObject)
{
	// ��ԑJ�ڃI�u�U�[�o�[����
	pStateChangeObserver = std::make_unique<PlayerActChangeObserver>("StateChangeObserver", *this);

	// �s���N���X�𐶐�
	using enum PlayerActState_Base::StateType;
	CreateState<PlayerIdleState>(Idle);
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerTargetMove>(TargetMove);
	CreateState<PlayerAttackState>(NormalAttack1);

	// �f�t�H���g��Ԃ��Z�b�g
	SetDefaultNode(Idle);
}
void PlayerActionController::Begin(CP_Animation& _animationController)
{
	// �X�e�[�g�}�V�����ʊJ�n����
	StateMachine_Base::Begin();

	// �A�j���[�V�����R���g���[���[���e�X�e�[�g�ɓn��
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

void PlayerActionController::ChangeNode(const PlayerActState_Base::StateType& _nextActionState)
{
	// �X�e�[�g�}�V���ŕύX
	StateMachine_Base::ChangeNode(_nextActionState);

	// �A�j���[�V��������state�ϐ����ύX
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));
}

PlayerActState_Base& PlayerActionController::CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase)
{
	return static_cast<PlayerActState_Base&>(_stateNodeBase);
}

void PlayerActionController::ImGuiSetting()
{
	if (!ImGuiMethod::TreeNode("Action")) return;

	std::string text = "NowState:" + PlayerActState_Base::StateTypeToStr(currentStateKey);
	ImGui::Text(text.c_str());

	for (auto& pAct : stateNodeList)	// �e�A�N�V�����̒���
		CastPlayerAct(*pAct.second).ImGuiCall();

	ImGui::TreePop();
}

nlohmann::json PlayerActionController::Save()
{
	nlohmann::json data;

	for (auto& node : stateNodeList)
	{
		nlohmann::json actData;
		actData["type"] = node.first;
		actData["data"] = CastPlayerAct(*node.second).Save();
		data["actData"].push_back(actData);
	}

	return data;
}

void PlayerActionController::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	nlohmann::json actDataList;
	if (LoadJsonDataArray("actData", actDataList, _data))
	{
		for (auto& actData : actDataList)
		{
			PlayerActState_Base::StateType state;
			if (!LoadJsonEnum<PlayerActState_Base::StateType>("type", state, actData))
				continue;


			nlohmann::json actParam;
			if (!LoadJsonData("data", actParam, actData))
				continue;
			else
			{
				int i = 0;
			}

			if (stateNodeList.contains(state))
				CastPlayerAct(*stateNodeList[state]).Load(actParam);
		}
	}
}
