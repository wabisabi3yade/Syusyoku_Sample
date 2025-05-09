#pragma once
#include "Component.h"
#include "IUISlider.h"

namespace HashiTaku
{
	/// @brief 体力ゲージなどを表示するUI
	class CP_Gage : public Component, public IUISlider
	{
		enum class BackState
		{
			NotUpdate,	// 更新しない
			Wait,	// 待機
			Move,	// 移動
		};
		
		/// @brief 正面スライダーオブジェクト名
		std::string frontObjName;

		/// @brief 後ろスライダーオブジェクト名
		std::string backObjName;

		/// @brief 正面のスライダー
		IUISlider* pFrontSlider;

		/// @brief 後ろのスライダー
		IUISlider* pBackSlider;

		/// @brief 後ろスライダーの移動イージング
		EaseKind backMoveEase;

		/// @brief 後ろスライダーの状態
		BackState backState;

		/// @brief 現在値
		float currentValue;

		/// @brief 最大値
		float maxValue;

		/// @brief 最低値
		float minValue;

		/// @brief 減ったときの後ろスライダーの値
		float backValueOnDecade;

		/// @brief 後ろスライダーの移動時間
		float backMoveTime;

		/// @brief 後ろスライダーが動き始めるまでの時間
		float backWaitTime;

		/// @brief 値が減ってからの経過時間
		float decadeElapsedTime;

		/// @brief 後ろスライダー移動の経過時間
		float backMoveElapsedTime;
	public:
		CP_Gage();
		~CP_Gage() {}

		/// @brief 現在の値を割合でセット(0.0～1.0)
		/// @param _curRatio 現在の割合
		void SetCurrentRatio(float _curRatio) override;

		/// @brief 現在の値をセット
		/// @param _curVal 現在の値
		void SetCurrentValue(float _curVal) override;

		/// @brief 最大値をセット
		/// @param _maxVal 最大値
		void SetMaxValue(float _maxVal) override;

		/// @brief 最小値をセット
		/// @param _minVal 最小値
		void SetMinValue(float _minVal) override;

		/// @brief 現在の値を取得
		/// @return 現在値
		float GetCurentValue() const;

		json Save() override;

		void Load(const json& _data) override;

	private:
		/// @brief 開始
		void Start() override;

		/// @brief 更新
		void Update() override;

		/// @brief 後ろスライダーの更新処理
		void BackSliderUpdate();

		/// @brief スライダーがセットされているか取得
		/// @return セットされているか？
		bool GetIsSettingSlider() const;

		void ImGuiDebug() override;

		/// @brief スライダーを取得する
		/// @param _objName オブジェクト名
		/// @return スライダー
		IUISlider* GetSlider(const std::string& _objName);
	};
}


