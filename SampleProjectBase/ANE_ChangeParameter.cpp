#include "pch.h"
#include "ANE_ChangeParameter.h"

ANE_ChangeParameter::ANE_ChangeParameter()
	: AnimationNotifyEvent(NotifyType::ANE_ChangeParameter),
	pAnimationParameters(nullptr)
{
}

void ANE_ChangeParameter::SetAnimationParameters(AnimationParameters& _animatiionParameters)
{
	pAnimationParameters = &_animatiionParameters;
}

std::unique_ptr<AnimationNotify_Base> ANE_ChangeParameter::Clone()
{
	return std::make_unique<ANE_ChangeParameter>(*this);
}

nlohmann::json ANE_ChangeParameter::Save()
{
	return nlohmann::json();
}

void ANE_ChangeParameter::Load(const nlohmann::json& _data)
{
}

void ANE_ChangeParameter::OnEvent()
{
}

void ANE_ChangeParameter::ImGuiDebug()
{
}
