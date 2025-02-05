#include "pch.h"
#include "TriggerTransCondition.h"

namespace HashiTaku
{
    TriggerTransCondition::TriggerTransCondition(const std::string& _parameterName)
        : TransCondition_Base(_parameterName, AnimParam::TypeKind::Trigger)
    {
    }

    bool TriggerTransCondition::IsCondition(AnimParam::conditionValType _checkValue) const
    {
        TriggerType* pTrigger = std::get_if<TriggerType>(&_checkValue);
        assert(pTrigger && "トリガータイプでありません");

        return pTrigger->GetIsTrigger();
    }

    json TriggerTransCondition::Save()
    {
        return json();
    }

    void TriggerTransCondition::Load(const json& _data)
    {
    }
}