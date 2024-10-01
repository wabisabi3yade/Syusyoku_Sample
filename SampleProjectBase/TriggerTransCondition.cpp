#include "pch.h"
#include "TriggerTransCondition.h"

TriggerTransCondition::TriggerTransCondition(const TriggerType& _parameter, const std::string& _parameterName)
    : TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Trigger), pParameter(&_parameter)
{
}

bool TriggerTransCondition::IsCondition() const
{
    return pParameter->GetIsTrigger();
}

nlohmann::json TriggerTransCondition::Save()
{
    return nlohmann::json();
}

void TriggerTransCondition::Load(const nlohmann::json& _data)
{
}
