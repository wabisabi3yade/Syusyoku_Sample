#include "pch.h"
#include "CP_AnimationNotify.h"
#include "GameObject.h"

CP_AnimationNotify::CP_AnimationNotify()
	: pAnimation(nullptr), curRatio(0.0f), lastRatio(-Mathf::smallValue), isLoop(false)
{
}

void CP_AnimationNotify::ImGuiSetting()
{
	ImGui::SliderFloat("current", &curRatio, 0.0f, 1.0f);
	ImGui::Text(std::to_string(lastRatio).c_str());

	if (ImGui::Button("Update"))
		UpdateNotifyGroups();
}

void CP_AnimationNotify::Init()
{
	/*UpdateNotifyGroups();*/
}

void CP_AnimationNotify::Start()
{
	pAnimation = gameObject->GetComponent<CP_Animation>();
	if (pAnimation == nullptr)
	{
		HASHI_DEBUG_LOG(gameObject->GetName() + "にAnimationコンポーネントがついていません");
		SetEnable(false);
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
	//if (!pAnimController->GetIsPlay()) return;	// 再生中じゃないなら
	if (!pAnimation->GetIsEnable()) return false;

	return true;
}

void CP_AnimationNotify::UpdateRatio()
{
	AnimationController* pAnimController = GetAnimController();
	curRatio = pAnimController->GetPlayingRatio();

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

void CP_AnimationNotify::UpdateCurGroup()
{
	AnimationNode_Base* pControllerCur = GetAnimController()->GetCurrentNode();
	if (pCurrentGroup->pAnimNode == pControllerCur) return;	// 同じなら

	// グループ内から探す
	auto itr = std::find_if(notifyGroups.begin(), notifyGroups.end(),
		[&](const NotifyGroup& _group)
		{
			return pControllerCur == _group.pAnimNode;
		});

	assert(itr != notifyGroups.end() && "対応グループが見つかりません");
	pCurrentGroup = &(*itr);
}

void CP_AnimationNotify::UpdateLastRatio()
{
	lastRatio = curRatio;
}

void CP_AnimationNotify::UpdateNotifyGroups()
{
	if (!pAnimation)
		pAnimation = gameObject->GetComponent<CP_Animation>();

	AnimationController* pAnimController = GetAnimController();
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
	AnimationController* pAnimController = pAnimation->GetAnimationController();
	assert(pAnimController && "AnimationControllerがありません");

	return pAnimController;
}

void CP_AnimationNotify::ImGuiGroupSetting()
{
	for (auto& group : notifyGroups)
	{
		std::string nodeName = group.pAnimNode->GetNodeName();
		if (!ImGuiMethod::TreeNode(nodeName.c_str())) continue;
		for (auto& n : group.pNotifys)
		{
			std::string notifyName = n->GetNotifyName();
			if (!ImGuiMethod::TreeNode(notifyName.c_str())) continue;
			n->ImGuiCall();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
