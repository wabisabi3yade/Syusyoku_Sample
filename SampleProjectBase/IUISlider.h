#pragma once

namespace HashiTaku
{
	/// @brief UIのスライダーのインターフェース
	class IUISlider
	{

	public:
		IUISlider() = default;
		virtual ~IUISlider() = default;


		/// @brief 現在の値を割合でセット(0.0～1.0)
		/// @param _curRatio 現在の割合
		virtual void SetCurrentRatio(float _curRatio) = 0;

		/// @brief 現在の値をセット
		/// @param _curVal 現在の値
		virtual void SetCurrentValue(float _curVal) = 0;

		/// @brief 最大値をセット
		/// @param _maxVal 最大値
		virtual void SetMaxValue(float _maxVal) = 0;

		/// @brief 最小値をセット
		/// @param _minVal 最小値
		virtual void SetMinValue(float _minVal) = 0;
	};
}