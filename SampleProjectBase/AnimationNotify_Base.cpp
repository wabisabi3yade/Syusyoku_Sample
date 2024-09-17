#include "pch.h"
#include "AnimationNotify_Base.h"

AnimationNotify_Base::AnimationNotify_Base()
	: isActive(true)
{
}

void AnimationNotify_Base::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

bool AnimationNotify_Base::GetIsActive() const
{
	return isActive;
}

void AnimationNotify_Base::ImGuiSetting()
{
	ImGui::Checkbox("isActive", &isActive);
}
