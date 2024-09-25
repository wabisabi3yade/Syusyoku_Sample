#include "pch.h"
#include "TransConditionCreater.h"

#include "BoolTransCondition.h"
#include "IntTransCondition.h"
#include "FloatTransCondition.h"

std::unique_ptr<TransCondition_Base> TransConditionCreater::Create(conditionValType& _parameter)
{
	std::unique_ptr<TransCondition_Base> pCondition;

	// •Ï”‚ÌŒ^–¼‚©‚çì¬‚·‚éğŒƒNƒ‰ƒX‚ğ•ÏX‚·‚é

	if (bool* pBool = std::get_if<bool>(&_parameter))	// bool
		pCondition = std::make_unique<BoolTransCondition>(*pBool);

	else if (int* pInt = std::get_if<int>(&_parameter))	// int
		pCondition = std::make_unique<IntTransCondition>(*pInt);

	else if (float *pFloat = std::get_if<float>(&_parameter))	// float
		pCondition = std::make_unique<FloatTransCondition>(*pFloat);

	return std::move(pCondition);
}
