#pragma once
#include "TransCondition_Base.h"
#include "AnimationParameterType.h"

class TransConditionCreater
{
public:
	static std::unique_ptr<TransCondition_Base> Create(conditionValType& _parameter);
};

