#pragma once
#include "TriggerType.h"

namespace HashiTaku
{
	namespace AnimParam	// アニメーションパラメータ
	{
		/// @brief 遷移条件に使用する変数の型名
		using conditionValType = std::variant<bool, float, int, TriggerType>;

		template<typename T>
		concept AnimParamConcept = std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, TriggerType>;

		// コンボボックスで作成する
		enum class TypeKind
		{
			Bool,
			Int,
			Float,
			Trigger
		};
	}
}
