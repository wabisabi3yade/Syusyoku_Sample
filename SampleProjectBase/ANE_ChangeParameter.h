#pragma once
#include "AnimationNotifyEvent.h"
#include "IAnimParametersSetter.h"

class ANE_ChangeParameter : public AnimationNotifyEvent, public IAnimParametersSetter
{
	/// @brief 変更したいパラメータ名
	std::string changeParamName;

	/// @brief アニメーション内パラメータ
	AnimationParameters* pAnimationParameters;
public:
	ANE_ChangeParameter();
	/// @brief コンストラクタ
	/// @param _animationParameters アニメーションパラメータリスト
	ANE_ChangeParameter(AnimationParameters& _animationParameters);
	~ANE_ChangeParameter() {}

	/// @brief アニメーションパラメータセット
	/// @param _animatiionParameters アニメーションパラメータ
	void SetAnimationParameters(AnimationParameters& _animatiionParameters) override;

	/// @brief クローン関数
	/// @return 生成した通知イベント
	std::unique_ptr<AnimationNotify_Base> Clone() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief イベント起動時処理
	void OnEvent() override;

	void ImGuiDebug() override;
};

