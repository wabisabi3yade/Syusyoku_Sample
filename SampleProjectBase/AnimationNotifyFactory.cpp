#include "pch.h"
#include "AnimationNotifyFactory.h"
#include "AnimNotifyDefine.h"

std::unique_ptr<AnimationNotify_Base> AnimationNotifyFactory::ImGuiCombo()
{
#ifdef EDIT
	std::vector<const std::string*> nameList = GetNotifyNames();
	if (nameList.empty()) return nullptr;

	std::unique_ptr<AnimationNotify_Base> pCreateNotify;

	if (ImGui::Button("+"))
	{
		pCreateNotify = Create(selectName);

	}
	
	ImGui::SameLine();
	ImGuiMethod::ComboBox("AddNotify", selectName, nameList);

	return std::move(pCreateNotify);
#endif // EDIT

	return nullptr;
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

	return animNotifyList[_notifyName]->Create();
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
