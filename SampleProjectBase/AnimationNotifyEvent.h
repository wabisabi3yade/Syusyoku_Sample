#pragma once
#include "AnimationNotify_Base.h"

/// @brief アニメーションの通知イベントの基底クラス
class AnimationNotifyEvent : public AnimationNotify_Base
{
	/// @brief イベント通知をするアニメーションの割合
	float eventRatio;

	/// @brief イベント時のフレーム
	u_int eventFrame;
public:
	/// @brief コンストラクタ
	/// @param _notifyType 通知イベント種類
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

	/// @brief 始まったときの開始処理
	virtual void OnInitialize() {};

	/// @brief 次のアニメーションに切り替える前の終了処理
	virtual void OnTerminal() {};

	void ImGuiDebug() override;
};

