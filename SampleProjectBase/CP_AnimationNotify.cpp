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
		HASHI_DEBUG_LOG(gameObject->GetName() + "��Animation�R���|�[�l���g�����Ă��܂���");
		SetEnable(false);
	}
	else
	{
		UpdateNotifyGroups();	// �A�j���[�V�����Ɠ���������
	}
}


void CP_AnimationNotify::LateUpdate()
{
	AnimationController* pAnimController = GetAnimController();

	// ���݂̃A�j���[�V�������X�V
	UpdateCurGroup();

	// ���݂̊����X�V
	UpdateRatio();

	// �C�x���g�ʒm�X�V
	NotifyUpdate();

	// �O�̊����X�V
	UpdateLastRatio();
}

bool CP_AnimationNotify::IsCanUpdate()
{
	AnimationController* pAnimController = GetAnimController();
#ifdef EDIT
	if (!pAnimController) return false;
#endif // EDIT
	if (!pAnimController->GetIsPlay()) return false;	// �Đ�������Ȃ��Ȃ�

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
	if (pCurrentGroup->pAnimNode != pCurConNode)	// ���݂̃C�x���g�ʒm�̃m�[�h�ƈႤ�Ȃ�
	{
		UpdateCurGroup();	// ���݂̃m�[�h���X�V����
	}
}

void CP_AnimationNotify::UpdateCurGroup()
{
	AnimationNode_Base* pControllerCur = GetAnimController()->GetCurrentNode();

	// ���݂̃m�[�h�̃C�x���g�ʒm��T��
	auto itr = std::find_if(notifyGroups.begin(), notifyGroups.end(),
		[&](const std::unique_ptr<NotifyGroup>& _group)
		{
			return pControllerCur == _group->pAnimNode;
		});

	assert(itr != notifyGroups.end() && "�Ή��O���[�v��������܂���");
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

	// �R���g���[���[���ɂ���m�[�h�ɍX�V����
	// ���X������̂͂��̂܂܁A�R���g���[���[���ɂȂ��ʒm�C�x���g�͍폜
	std::list<const AnimationNode_Base*> pCreateNodes;
	pAnimController->GetNodeArray(pCreateNodes);

	// ���łɂ���ʒm�C�x���g�͐V�K�쐬���Ȃ�
	for (auto& notify : notifyGroups)
	{
		bool isDelete = true;
		for (auto& pNode : pCreateNodes)
		{
			// ����Ȃ�
			if (pNode == notify->pAnimNode)
			{
				isDelete = false;
				pCreateNodes.remove(pNode);	// �쐬���郊�X�g������폜
				break;
			}
		}

		if (isDelete)	// �R���g���[���[���ɂȂ��Ȃ�
		{
			notifyGroups.remove(notify);	// �폜
		}
	}


	for (auto& pNode : pCreateNodes)	// �V������������m�[�h
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
		HASHI_DEBUG_LOG("Animation�R���|�[�l���g������܂���");
		return nullptr;
	}

	pAnimController = pAnim->GetAnimationController();
	assert(pAnimController && "AnimationController������܂���");

	return pAnimController;
}

void CP_AnimationNotify::ImGuiGroupSetting()
{
#ifdef EDIT
	for (auto& group : notifyGroups)
	{
		std::string nodeName = group->pAnimNode->GetNodeName();
		if (!ImGuiMethod::TreeNode(nodeName.c_str())) continue;

		// �C�x���g�ʒm�̕ҏW
		for (auto& n : group->pNotifys)
		{
			std::string notifyName = n->GetNotifyName();
			if (!ImGuiMethod::TreeNode(notifyName.c_str())) continue;
			n->ImGuiCall();
			ImGui::TreePop();
		}

		// �V��������
		std::unique_ptr<AnimationNotify_Base> pNotify;
		if (pFactory->ImGuiCombo(pNotify))
		{
			group->pNotifys.push_back(std::move(pNotify));
		}

		ImGui::TreePop();
	}

#endif // EDIT
}
