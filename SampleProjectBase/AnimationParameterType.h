#pragma once

namespace HashiTaku
{
	namespace AnimParam	// アニメーションパラメータ
	{
		/// @brief 遷移条件に使用する変数の型名
		using conditionValType = std::variant<bool, float, int>;

		// コンボボックスで作成する
		enum class TypeKind
		{
			Bool,
			Int,
			Float
		};
	}
}
