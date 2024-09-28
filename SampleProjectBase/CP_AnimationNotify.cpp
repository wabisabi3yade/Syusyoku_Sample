#include "pch.h"
#include "CP_AnimationNotify.h"
#include "GameObject.h"
#include "AnimationController.h"

CP_AnimationNotify::CP_AnimationNotify()
	: pAnimController(nullptr), curRatio(0.0f), lastRatio(-Mathf::smallValue), isLoop(false)
{
}

void CP_AnimationNotify::ImGuiSetting()
{
	ImGui::SliderFloat("current", &curRatio, 0.0f, 1.0f);
	ImGui::Text(std::to_string(lastRatio).c_str());

	ImGuiGroupSetting();

	if (ImGui::Button("Update"))
		UpdateNotifyGroups();
}

void CP_AnimationNotify::Init()
{
#ifdef EDIT
	pFactory = std::make_unique<AnimationNotifyFactory>();
#endif // EDIT
}

void CP_AnimationNotify::Start()
{
	CP_Animation* pAnim = gameObject->GetComponent<CP_PlayerAnimation>();
	
	if (pAnimController == nullptr)
	{
		HASHI_DEBUG_LOG(gameObject->GetName() + "にAnimationコンポーネントがついていません");
		SetEnable(false);
	}
	else
	{
		UpdateNotifyGroups();	// アニメーションと同期させる
	}
}


void CP_AnimationNotify::LateUpdate()
{
	AnimationController* pAnimController = GetAnimController();

	// 現在のアニメーションを更新
	UpdateCurGroup();

	// 現在の割合更新
	UpdateRatio();

	// イベント通知更新
	NotifyUpdate();

	// 前の割合更新
	UpdateLastRatio();
}

bool CP_AnimationNotify::IsCanUpdate()
{
	AnimationController* pAnimController = GetAnimController();
#ifdef EDIT
	if (!pAnimController) return false;
#endif // EDIT
	if (!pAnimController->GetIsPlay()) return false;	// 再生中じゃないなら

	return true;
}

void CP_AnimationNotify::UpdateRatio()
{
	AnimationController* pAnimController = GetAnimController();
	curRatio = pAnimController->GetPlayingRatio();
	isLoop = false;

	if (curRatio < lastRatio)
		isLoop = true;
}

void CP_AnimationNotify::NotifyUpdate()
{
	for (auto& pNotify : pCurrentGroup->pNotifys)
	{
		pNotify->Update(lastRatio, curRatio, isLoop);
	}
}

void CP_AnimationNotify::LinkCurGroup()
{
	AnimationNode_Base* pCurConNode = pAnimController->GetCurrentNode();
	if (pCurrentGroup->pAnimNode != pCurConNode)	// 現在のイベント通知のノードと違うなら
	{
		UpdateCurGroup();	// 現在のノードを更新する
	}
}

void CP_AnimationNotify::UpdateCurGroup()
{
	AnimationNode_Base* pControllerCur = GetAnimController()->GetCurrentNode();

	// 現在のノードのイベント通知を探す
	auto itr = std::find_if(notifyGroups.begin(), notifyGroups.end(),
		[&](const std::unique_ptr<NotifyGroup>& _group)
		{
			return pControllerCur == _group->pAnimNode;
		});

	assert(itr != notifyGroups.end() && "対応グループが見つかりません");
	pCurrentGroup = (*itr).get();
}

void CP_AnimationNotify::UpdateLastRatio()
{
	lastRatio = curRatio;
}

void CP_AnimationNotify::UpdateNotifyGroups()
{
	AnimationController* pAnimController = GetAnimController();
	if (!pAnimController) return;

	// コントローラー内にあるノードに更新する
	// 元々あるものはそのまま、コントローラー内にない通知イベントは削除
	std::list<const AnimationNode_Base*> pCreateNodes;
	pAnimController->GetNodeArray(pCreateNodes);

	// すでにある通知イベントは新規作成しない
	for (auto& notify : notifyGroups)
	{
		bool isDelete = true;
		for (auto& pNode : pCreateNodes)
		{
			// あるなら
			if (pNode == notify->pAnimNode)
			{
				isDelete = false;
				pCreateNodes.remove(pNode);	// 作成するリストからも削除
				break;
			}
		}

		if (isDelete)	// コントローラー内にないなら
		{
			notifyGroups.remove(notify);	// 削除
		}
	}


	for (auto& pNode : pCreateNodes)	// 新しく生成するノード
	{
		std::unique_ptr<NotifyGroup> pNewGroup = std::make_unique<NotifyGroup>();
		pNewGroup->pAnimNode = pNode;
		notifyGroups.push_back(std::move(pNewGroup));
	}
}

void CP_AnimationNotify::CreateNotify(NotifyGroup& _addGroup, const std::string& _typeName)
{
#ifdef EDIT
	std::unique_ptr<AnimationNotify_Base> pCreate = pFactory->Create(_typeName);
	pCreate->Init(*gameObject);
	_addGroup.pNotifys.push_back(std::move(pCreate));
#endif // EDIT
}

AnimationController* CP_AnimationNotify::GetAnimController()
{
	CP_Animation* pAnim = gameObject->GetComponent<CP_PlayerAnimation>();
	if (!pAnim)
	{
		HASHI_DEBUG_LOG("Animationコンポーネントがありません");
		return nullptr;
	}

	pAnimController = pAnim->GetAnimationController();
	assert(pAnimController && "AnimationControllerがありません");

	return pAnimController;
}

void CP_AnimationNotify::ImGuiGroupSetting()
{
#ifdef EDIT
	for (auto& group : notifyGroups)
	{
		std::string nodeName = group->pAnimNode->GetNodeName();
		if (!ImGuiMethod::TreeNode(nodeName.c_str())) continue;

		// イベント通知の編集
		for (auto& n : group->pNotifys)
		{
			std::string notifyName = n->GetNotifyName();
			if (!ImGuiMethod::TreeNode(notifyName.c_str())) continue;
			n->ImGuiCall();
			ImGui::TreePop();
		}

		// 新しく生成
		std::unique_ptr<AnimationNotify_Base> pNotify;
		if (pFactory->ImGuiCombo(pNotify))
		{
			group->pNotifys.push_back(std::move(pNotify));
		}

		ImGui::TreePop();
	}

#endif // EDIT
}
