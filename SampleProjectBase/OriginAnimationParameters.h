#pragma once
#include "AnimationParameters.h"

/// @brief 各オブジェクトへ渡すためのコピー元となるアニメーションパラメータ
class OriginAnimationParameters : public AnimationParameters
{
	/// @brief パラメータリスト
	std::unordered_map<std::string, HashiTaku::AnimParam::conditionValType> animParameters;

	/// @brief パラメータ削除時の通知サブジェクト
	std::unique_ptr<HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>> pAnimParamSubject;
public:
	OriginAnimationParameters();
	~OriginAnimationParameters() {}


};

