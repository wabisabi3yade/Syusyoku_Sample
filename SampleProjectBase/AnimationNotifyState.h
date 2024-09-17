#pragma once
#include "AnimationNotify_Base.h"

/// @brief アニメーションの通知
class AnimationNotifyState : public AnimationNotify_Base
{
	/// @brief 開始イベント割合
	float startEventRatio;

	/// @brief 終了イベント割合
	float endEventRatio;
public:
	AnimationNotifyState();
	~AnimationNotifyState() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// 開始時のイベント割合をセットする
	void SetStartRatio(float _startRatio);

	// 終了時のイベント割合をセットする
	void SetEndRatio(float _endRatio);

	virtual std::string ClassNameToStr() override = 0;
protected:
	/// @brief イベント開始処理
	virtual void Begin() = 0;

	/// @brief イベント間の更新処理
	virtual void Tick() = 0;

	/// @brief イベント終了処理
	virtual void End() = 0;
};

