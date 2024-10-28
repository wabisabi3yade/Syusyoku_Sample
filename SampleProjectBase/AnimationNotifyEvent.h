#pragma once
#include "AnimationNotify_Base.h"

/// @brief アニメーションの通知イベントの基底クラス
class AnimationNotifyEvent : public AnimationNotify_Base
{
	/// @brief イベント通知をするアニメーションの割合
	float eventRatio;
public:
	AnimationNotifyEvent(NotifyType _notifyType);
	virtual ~AnimationNotifyEvent() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// イベントの割合をセット
	void SetEventRatio(float _ratio);

	/// @brief クローン関数
	/// @return 生成した通知イベント
	virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief イベント起動時処理
	virtual void OnEvent() = 0;

	void ImGuiSetting() override;
};

