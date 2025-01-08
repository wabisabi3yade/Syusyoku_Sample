#pragma once

namespace HashiTaku
{
	/// @brief 深度書き込みを行いたい側のインターフェース
	class IApplyShadowDepth
	{
	public:
		IApplyShadowDepth() = default;
		virtual ~IApplyShadowDepth() = default;

		/// @brief 深度書き込みを行う
		virtual void WriteDepth() = 0;
	};
}
