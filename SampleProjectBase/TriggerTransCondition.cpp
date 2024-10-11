#include "pch.h"
#include "TriggerTransCondition.h"

TriggerTransCondition::TriggerTransCondition(const std::string& _parameterName)
    : TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Trigger)
{
}

bool TriggerTransCondition::IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const
{
    TriggerType* pTrigger = std::get_if<TriggerType>(&_checkValue);
    assert(pTrigger && "�g���K�[�^�C�v�ł���܂���");

    return pTrigger->GetIsTrigger();
}

nlohmann::json TriggerTransCondition::Save()
{
    return nlohmann::json();
}

void TriggerTransCondition::Load(const nlohmann::json& _data)
{
}
