#include "pch.h"
#include "TransConditionCreater.h"

#include "BoolTransCondition.h"
#include "IntTransCondition.h"
#include "FloatTransCondition.h"
#include "TriggerTransCondition.h"

using namespace HashiTaku;

std::unique_ptr<TransCondition_Base> TransConditionCreater::Create(const AnimParam::conditionValType& _parameterValue, const std::string& _parameterName)
{
	std::unique_ptr<TransCondition_Base> pCondition;

	// •Ï”‚ÌŒ^–¼‚©‚çì¬‚·‚éğŒƒNƒ‰ƒX‚ğ•ÏX‚·‚é

	if (const bool* pBool = std::get_if<bool>(&_parameterValue))	// bool
		pCondition = std::make_unique<BoolTransCondition>(_parameterName);

	else if (const int* pInt = std::get_if<int>(&_parameterValue))	// int
		pCondition = std::make_unique<IntTransCondition>(_parameterName);

	else if (const float *pFloat = std::get_if<float>(&_parameterValue))	// float
		pCondition = std::make_unique<FloatTransCondition>(_parameterName);

	else if (const TriggerType* pTrigger = std::get_if<TriggerType>(&_parameterValue))	// float
		pCondition = std::make_unique<TriggerTransCondition>(_parameterName);

	return std::move(pCondition);
}
