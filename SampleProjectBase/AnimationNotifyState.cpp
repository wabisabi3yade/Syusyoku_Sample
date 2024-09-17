#include "pch.h"
#include "AnimationNotifyState.h"

AnimationNotifyState::AnimationNotifyState()
	:startEventRatio(0.0f), endEventRatio(1.0f)
{
}

void AnimationNotifyState::Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop)
{
	if (_isLoop)
	{
		if (_lastPlayingRatio < startEventRatio)
		{
			Begin();
			End();
		}
	}
	else
	{
		// 範囲内でないなら終了する
		if (_curPlayingRatio > startEventRatio)
		{
			if (_lastPlayingRatio < startEventRatio)
				Begin();

			if (_curPlayingRatio < endEventRatio)
				Tick();
		}

		if (_lastPlayingRatio < endEventRatio && _curPlayingRatio > endEventRatio)
			End();
	}
}

void AnimationNotifyState::SetStartRatio(float _startRatio)
{
	// 終了よりも後には置かない
	startEventRatio = std::clamp(_startRatio, 0.0f, endEventRatio);
}

void AnimationNotifyState::SetEndRatio(float _endRatio)
{
	// 開始よりも先には置かない
	endEventRatio = std::clamp(_endRatio, startEventRatio, 1.0f);
}
