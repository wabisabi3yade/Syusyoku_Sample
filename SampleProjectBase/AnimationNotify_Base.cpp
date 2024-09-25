#include "pch.h"
#include "AnimationNotify_Base.h"

AnimationNotify_Base::AnimationNotify_Base()
	: isActive(true), notifyName(""), pGameObject(nullptr)
{
}

void AnimationNotify_Base::Init(GameObject& _pGameObject)
{
	pGameObject = &_pGameObject;
}

void AnimationNotify_Base::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

void AnimationNotify_Base::SetNotifyName(const std::string& _notifyName)
{
	notifyName = _notifyName;
}

bool AnimationNotify_Base::GetIsActive() const
{
	return isActive;
}

std::string AnimationNotify_Base::GetNotifyName() const
{
	return notifyName;
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
	ImGuiMethod::EditableText(notifyName, isEditing);
#endif // EDIT
}
