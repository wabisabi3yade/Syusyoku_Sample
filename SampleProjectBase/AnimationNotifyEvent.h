#pragma once
#include "AnimationNotify_Base.h"

/// @brief アニメーションの通知イベントの基底クラス
class AnimationNotifyEvent : public AnimationNotify_Base
{
	/// @brief イベント通知をするアニメーションの割合
	float eventRatio;
public:
	AnimationNotifyEvent();
	virtual ~AnimationNotifyEvent() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// イベントの割合をセット
	void SetEventRatio(float _ratio);

	virtual std::string ClassNameToStr() override = 0;
protected:
	/// @brief イベント起動時処理
	virtual void OnEvent() = 0;
};

