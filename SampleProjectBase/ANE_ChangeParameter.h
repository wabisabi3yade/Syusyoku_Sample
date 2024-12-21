#pragma once
#include "AnimationNotifyEvent.h"
#include "IAnimParametersSetter.h"

namespace HashiTaku
{
	class ANE_ChangeParameter : public AnimationNotifyEvent, public IAnimParametersSetter
	{
		/// @brief 変更したいパラメータ名
		std::string changeParamName;

		/// @brief アニメーション内パラメータ
		AnimationParameters* pAnimationParameters;

		/// @brief イベントで代入する値
		bool setParam;
	public:
		ANE_ChangeParameter();
		~ANE_ChangeParameter() {}

		/// @brief アニメーションパラメータセット
		/// @param _animatiionParameters アニメーションパラメータ
		void SetAnimationParameters(AnimationParameters& _animatiionParameters) override;

		/// @brief クローン関数
		/// @return 生成した通知イベント
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		/// @brief イベント起動時処理
		void OnEvent() override;

		/// @brief  初期化
		void OnInitialize() override;

		/// @brief 終了
		void OnTerminal() override;

		void ImGuiDebug() override;
	};
}