#pragma once
#include "TransCondition_Base.h"
#include "AnimationParameterType.h"

namespace HashiTaku
{
	/// @brief 条件を作成するクラス
	class TransConditionCreater
	{
	public:
		/// @brief 遷移条件を作成する
		/// @param _parameterValue 遷移条件パラメータ
		/// @param _parameterName パラメータ名
		/// @return 作成した遷移条件インスタンス
		static std::unique_ptr<TransCondition_Base> Create(const AnimParam::conditionValType& _parameterValue, const std::string& _parameterName);
	};
}