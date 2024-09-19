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
	// イベント追加
	ResisterNotify<ANE_DebugLog>();

	// ステート追加
	ResisterNotify<ANS_DebugLog>();
}
