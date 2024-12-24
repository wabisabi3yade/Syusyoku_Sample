#pragma once

namespace HashiTaku
{
	/// @brief トランスフォームを必要とするインターフェース
	class ITransformSetter
	{

	public:
		ITransformSetter() = default;
		~ITransformSetter() = default;

		/// @brief トランスフォームのセッター
		/// @param _transform トランスフォーム
		virtual void SetTransform(const Transform& _transform) = 0;
	};
}