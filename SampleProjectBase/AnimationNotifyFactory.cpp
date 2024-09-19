#include "pch.h"
#include "AnimationNotifyFactory.h"
#include "AnimNotifyDefine.h"

bool AnimationNotifyFactory::ImGuiCombo(std::unique_ptr<AnimationNotify_Base>& _pCreateNotify)
{
#ifdef EDIT
	std::vector<const std::string*> nameList = GetNotifyNames();
	if (nameList.empty()) return false;

	if (ImGui::Button("+"))
	{
		_pCreateNotify = Create(selectName);
		return true;
	}
	
	ImGui::SameLine();
	ImGuiMethod::ComboBox("AddNotify", selectName, nameList);
#endif // EDIT

	return false;
}

AnimationNotifyFactory::AnimationNotifyFactory()
{
	Init();	// ����������
}

std::unique_ptr<AnimationNotify_Base> AnimationNotifyFactory::Create(const std::string& _notifyName)
{
#ifdef _DEBUG
	if (!animNotifyList.contains(_notifyName))
	{
		HASHI_DEBUG_LOG(_notifyName + "���������܂���ł���");
		return nullptr;
	}
#endif // _DEBUG

	std::unique_ptr<AnimationNotify_Base> pCreate = animNotifyList[_notifyName]->Create();
	pCreate->SetNotifyName(_notifyName);
	return std::move(pCreate);
}

std::vector<const std::string*> AnimationNotifyFactory::GetNotifyNames() const
{
	std::vector<const std::string*> nameList;

	for (auto& notify : animNotifyList)
	{
		nameList.push_back(&notify.first);
	}

	return nameList;
}

void AnimationNotifyFactory::Init()
{
	// �C�x���g�ǉ�
	ResisterNotify<ANE_DebugLog>();

	// �X�e�[�g�ǉ�
	ResisterNotify<ANS_DebugLog>();
}
