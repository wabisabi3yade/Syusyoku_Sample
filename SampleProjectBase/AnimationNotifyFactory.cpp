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
	Init();	// 初期化処理
}

std::unique_ptr<AnimationNotify_Base> AnimationNotifyFactory::Create(const std::string& _notifyName)
{
#ifdef _DEBUG
	if (!animNotifyList.contains(_notifyName))
	{
		HASHI_DEBUG_LOG(_notifyName + "が見つけられませんでした");
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
	// イベント追加
	ResisterNotify<ANE_DebugLog>();

	// ステート追加
	ResisterNotify<ANS_DebugLog>();
}
