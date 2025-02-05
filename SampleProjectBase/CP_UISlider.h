#pragma once
#include "CP_UIRenderer.h"
#include "IUISlider.h"

namespace HashiTaku
{
	class CP_UIRenderer;

	/// @brief UIのスライダー処理を行うクラス
	class CP_UISlider : public CP_UIRenderer, public IUISlider
	{
		/// @brief スライダー画像の中心座標
		DXSimp::Vector3 sliderCenterPos;

		/// @brief スライダー画像のスケール
		DXSimp::Vector2 sliderScale;

		/// @brief 現在の値
		float currentValue;

		/// @brief 最大値
		float maxValue;

		/// @brief 最小値
		float minValue;
	public:
		CP_UISlider();
		~CP_UISlider() {}

		void Init() override;
		void OnChangeScale() override;
		void OnChangePosition() override;
		void OnChangeRotation() override;

		/// @brief 現在の値を割合でセット(0.0〜1.0)
		/// @param _curRatio 現在の割合
		void SetCurrentRatio(float _curRatio);

		/// @brief 現在の値をセット
		/// @param _curVal 現在の値
		void SetCurrentValue(float _curVal);

		/// @brief 最大値をセット
		/// @param _maxVal 最大値
		void SetMaxValue(float _maxVal);

		/// @brief 最小値をセット
		/// @param _minVal 最小値
		void SetMinValue(float _minVal);

		json Save() override;
		void Load(const json& _data) override;

	private:
		/// @brief スライダー画像に適用する
		void ApplySlider();

		/// @brief ポリゴン再生性
		void ReCreatePolygon() override;

		void ImGuiDebug() override;
	};
}