#include "pch.h"
#include "AnimationNotifyEvent.h"

void AnimationNotifyEvent::SetEventRatio(float _addRatio)
{
	_addRatio = std::clamp(_addRatio, 0.0f, 1.0f);	
}

AnimationNotifyEvent::AnimationNotifyEvent() : eventRatio(0.0f)
{
}

void AnimationNotifyEvent::Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop)
{
	if (_isLoop)
	{
		if (eventRatio > _lastPlayingRatio)
			OnEvent();
	}
	else
	{
		// �O��ƌ��݂̊����͈͂ɓ����Ă���Ȃ珈��
		if (eventRatio < _lastPlayingRatio) return;
		if (eventRatio > _curPlayingRatio) return;

		OnEvent();
	}
}
