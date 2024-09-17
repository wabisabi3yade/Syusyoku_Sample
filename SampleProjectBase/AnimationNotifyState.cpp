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
		// �͈͓��łȂ��Ȃ�I������
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
	// �I��������ɂ͒u���Ȃ�
	startEventRatio = std::clamp(_startRatio, 0.0f, endEventRatio);
}

void AnimationNotifyState::SetEndRatio(float _endRatio)
{
	// �J�n������ɂ͒u���Ȃ�
	endEventRatio = std::clamp(_endRatio, startEventRatio, 1.0f);
}
