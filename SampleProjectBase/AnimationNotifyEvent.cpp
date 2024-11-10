#include "pch.h"
#include "AnimationNotifyEvent.h"

void AnimationNotifyEvent::SetEventRatio(float _addRatio)
{
	_addRatio = std::clamp(_addRatio, 0.0f, 1.0f);	
}

nlohmann::json AnimationNotifyEvent::Save()
{
	auto data = AnimationNotify_Base::Save();
	data["eventRatio"] = eventRatio;
	return data;
}

void AnimationNotifyEvent::Load(const nlohmann::json& _data)
{
	AnimationNotify_Base::Load(_data);
	HashiTaku::LoadJsonFloat("eventRatio", eventRatio, _data);
}

void AnimationNotifyEvent::ImGuiDebug()
{
	AnimationNotify_Base::ImGuiDebug();
	ImGui::SliderFloat("event", &eventRatio, 0.0f, 1.0f);
}

AnimationNotifyEvent::AnimationNotifyEvent(NotifyType _notifyType) : AnimationNotify_Base(_notifyType), eventRatio(0.0f)
{
}

void AnimationNotifyEvent::Update(const float _lastPlayingRatio, const float _curPlayingRatio, const bool _isLoop)
{
	if (!GetIsActive()) return;

	if (_isLoop)
	{
		if (eventRatio > _lastPlayingRatio || eventRatio < _curPlayingRatio)
			OnEvent();
	}
	else
	{
		// ‘O‰ñ‚ÆŒ»Ý‚ÌŠ„‡”ÍˆÍ‚É“ü‚Á‚Ä‚¢‚é‚È‚çˆ—
		if (eventRatio < _lastPlayingRatio) return;
		if (eventRatio > _curPlayingRatio) return;

		OnEvent();
	}
}
