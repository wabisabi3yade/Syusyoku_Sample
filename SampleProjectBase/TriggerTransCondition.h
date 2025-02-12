#pragma once
#include "TransCondition_Base.h"

namespace HashiTaku
{
	/// @brief トリガー型変数を見て、遷移条件達成しているか
	class TriggerTransCondition : public TransCondition_Base
	{
	public:
		TriggerTransCondition(const std::string& _parameterName);
		~TriggerTransCondition() {}

		bool IsCondition(AnimParam::conditionValType _checkValue)const override;

		json Save() override;
		void Load(const json& _data) override;
	};
}