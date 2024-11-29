#pragma once
#include "Component.h"
#include "IUISlider.h"

class CP_UIRenderer;

/// @brief UIのスライダー処理を行うクラス
class CP_UISlider : public Component, public IUISlider
{
	/// @brief UI描画
	CP_UIRenderer* pUiRenderer;

	/// @brief 
	float startPos;

	/// @brief 現在の値
	float currentValue;

	/// @brief 最大値
	float maxValue;

	/// @brief 最小値
	float minValue;

	/// @brief スライダーの全体の長さ
	float sliderAllLength;
public:
	CP_UISlider();
	~CP_UISlider() {}

	void Init() override;

	/// @brief 現在の値を割合でセット(0.0～1.0)
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

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief スライダー画像に適用する
	void ApplySlider();

	void ImGuiDebug() override;
};

