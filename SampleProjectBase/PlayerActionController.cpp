#include "pch.h"
#include "PlayerActionController.h"
#include "CP_Animation.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerAttackState.h"

PlayerActionController::PlayerActionController(GameObject & _pPlayerObject) 
	: pCurrentState(nullptr), pAnimation(nullptr), pPlayerObject(&_pPlayerObject)
{
	// ��ԑJ�ڃI�u�U�[�o�[����
	pStateChangeObserver = std::make_unique<PlayerActChangeObserver>("StateChangeObserver", *this);

	// �s���N���X�𐶐�
	CreateState<PlayerIdleState>();
	CreateState<PlayerMoveState>();
	CreateState<PlayerTargetMove>();
	CreateState<PlayerAttackState>();

	// �f�t�H���g��Ԃ��Z�b�g
	DefaultState(PlayerActState_Base::StateType::Move);
}
void PlayerActionController::Begin(CP_Animation& _animationController)
{
	// �A�j���[�V�����R���g���[���[���e�X�e�[�g�ɓn��
	pAnimation = &_animationController;
	for (auto& actState : actionList)
	{
		actState.second->SetAnimation(_animationController);
	}	
}

void PlayerActionController::Update()
{
	pCurrentState->UpdateCall();
}

void PlayerActionController::ChangeState(PlayerActState_Base::StateType _nextState)
{
	// �ύX�O�A�N�V�����I������
	pCurrentState->OnEndCall();

	// �w�肵����ԂɑJ��
	pCurrentState = actionList[_nextState].get();

	// �A�j���[�V�����ϐ��̏�Ԃ��ύX����
	pAnimation->SetInt(STATE_PARAMNAME, static_cast<int>(_nextState));

	// �ύX��A�N�V������������
	pCurrentState->OnStartCall();

	HASHI_DEBUG_LOG(PlayerActState_Base::StateTypeToStr(_nextState) + "�ɑJ��");
}

void PlayerActionController::DefaultState(PlayerActState_Base::StateType _defaultState)
{
	pCurrentState = actionList[_defaultState].get();
}

void PlayerActionController::ImGuiSetting()
{
	if (!ImGuiMethod::TreeNode("Action")) return;

	std::string text = "NowState:" + PlayerActState_Base::StateTypeToStr(pCurrentState->GetActStateType());
	ImGui::Text(text.c_str());

	for (auto& pAct : actionList)	// �e�A�N�V�����̒���
		pAct.second->ImGuiCall();

	ImGui::TreePop();
}

nlohmann::json PlayerActionController::Save()
{
	nlohmann::json data;

	for (auto& act : actionList)
	{
		nlohmann::json actData;
		actData["type"] = act.first;
		actData["data"] = act.second->Save();
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

			actionList[state]->Load(actParam);
		}
	}
}
