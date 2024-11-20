#pragma once
#include "AnimationNotify_Base.h"

/// @brief アニメーションの通知
class AnimationNotifyState : public AnimationNotify_Base
{
	/// @brief 開始イベント割合
	float startEventRatio;
	/// @brief 開始イベントフレーム
	u_int startEventFrame;

	/// @brief 終了イベント割合
	float endEventRatio;
	/// @brief 終了イベントフレーム
	u_int endEventFrame;
public:

	/// @brief コンストラクタ
	/// @param _notifyType 通知イベント種類
	AnimationNotifyState(NotifyType _notifyType);
	virtual ~AnimationNotifyState() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// 開始時のイベント割合をセットする
	void SetStartRatio(float _startRatio);

	// 終了時のイベント割合をセットする
	void SetEndRatio(float _endRatio);

	/// @brief クローン関数
	/// @return 生成した通知イベント
	virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief イベント開始処理
	virtual void Begin() = 0;

	/// @brief イベント間の更新処理
	virtual void Tick() = 0;

	/// @brief イベント終了処理
	virtual void End() = 0;

	void ImGuiDebug() override;
};

