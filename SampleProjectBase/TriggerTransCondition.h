#pragma once
#include "TransCondition_Base.h"

namespace HashiTaku
{
	/// @brief ƒgƒŠƒK[Œ^•Ï”‚ğŒ©‚ÄA‘JˆÚğŒ’B¬‚µ‚Ä‚¢‚é‚©
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