#include "pch.h"
#include "AnimationNotify_Base.h"

AnimationNotify_Base::AnimationNotify_Base()
	: isActive(true), notifyName(""), notifyType(NotifyType::None)
{
}

AnimationNotify_Base::AnimationNotify_Base(NotifyType _notifyType)
	: isActive(true), notifyName(""), notifyType(_notifyType)
{
}

void AnimationNotify_Base::Init()
{
}

void AnimationNotify_Base::OnTerminalCall()
{
	if (!isActive) return;
	OnTerminal();
}

void AnimationNotify_Base::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

void AnimationNotify_Base::SetNotifyName(const std::string& _notifyName)
{
	notifyName = _notifyName;
}

std::string AnimationNotify_Base::GetTypeToStr(NotifyType _notifyType)
{
	switch (_notifyType)
	{
	case AnimationNotify_Base::NotifyType::ANE_DebugLog:
		return "ANE_DebugLog";
		break;
	case AnimationNotify_Base::NotifyType::ANS_DebugLog:
		return "ANS_DebugLog";
		break;
	case AnimationNotify_Base::NotifyType::ANE_ChangeParameter:
		return "ANE_ChangeParameter";
		break;
	case AnimationNotify_Base::NotifyType::ANS_ChangeParameter:
		return "ANS_ChangeParameter";
		break;
	default:
		break;
	}

	return "";
}

bool AnimationNotify_Base::GetIsActive() const
{
	return isActive;
}

std::string AnimationNotify_Base::GetNotifyName() const
{
	return notifyName;
}

AnimationNotify_Base::NotifyType AnimationNotify_Base::GetNotifyType() const
{
	return notifyType;
}

nlohmann::json AnimationNotify_Base::Save()
{
	nlohmann::json data;
	data["isActive"] = isActive;
	data["notifyName"] = notifyName;
	return data;
}

void AnimationNotify_Base::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonBoolean("isActive", isActive, _data);
	HashiTaku::LoadJsonString("notifyName", notifyName, _data);
}

void AnimationNotify_Base::ImGuiSetting()
{
#ifdef EDIT
	ImGui::Checkbox("isActive", &isActive);
	ImGuiMethod::EditableText("##anb", notifyName, isEditing);
#endif // EDIT
}
