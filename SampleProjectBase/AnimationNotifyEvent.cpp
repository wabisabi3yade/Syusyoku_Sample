#include "pch.h"
#include "AnimationNotifyEvent.h"

void AnimationNotifyEvent::SetEventRatio(float _addRatio)
{
	_addRatio = std::clamp(_addRatio, 0.0f, 1.0f);	

}

nlohmann::json AnimationNotifyEvent::Save()
{
	auto data = AnimationNotify_Base::Save();
	bool isUseFrame = UseFrame();
	data["useFrame"] = isUseFrame;

	if (isUseFrame)
	{
		data["eventFrame"] = eventFrame;
	}
	else
	{
		data["eventRatio"] = eventRatio;
	}
	
	return data;
}

void AnimationNotifyEvent::Load(const nlohmann::json& _data)
{
	AnimationNotify_Base::Load(_data);
	
	bool isUseFrame = false;
	HashiTaku::LoadJsonBoolean("useFrame", isUseFrame, _data);
	if (isUseFrame)
	{
		HashiTaku::LoadJsonUnsigned("eventFrame", eventFrame, _data);
		eventRatio = ConvertFrameToRatio(eventFrame);
	}
	else
	{
		HashiTaku::LoadJsonFloat("eventRatio", eventRatio, _data);
	}
}

void AnimationNotifyEvent::ImGuiDebug()
{
	if (UseFrame())
	{
		int imInt = static_cast<int>(eventFrame);
		ImGui::SliderInt("eventF", &imInt, 0, GetAnimFrameCnt());
		eventFrame = static_cast<u_int>(imInt);
		eventRatio = ConvertFrameToRatio(eventFrame);
	}
	else
	{
		AnimationNotify_Base::ImGuiDebug();
		ImGui::SliderFloat("eventR", &eventRatio, 0.0f, 1.0f);

	}
}

AnimationNotifyEvent::AnimationNotifyEvent(NotifyType _notifyType) : AnimationNotify_Base(_notifyType), eventRatio(0.0f), eventFrame(0)
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
		// 前回と現在の割合範囲に入っているなら処理
		if (eventRatio < _lastPlayingRatio) return;
		if (eventRatio > _curPlayingRatio) return;

		OnEvent();
	}
}

void AnimationNotifyEvent::Update(const u_int _lastPlayFrame, const u_int _curPlayingFrame, bool _isLoop)
{
	if (!GetIsActive()) return;

	if (_isLoop)
	{
		if (eventFrame > _lastPlayFrame || eventFrame < _curPlayingFrame)
			OnEvent();
	}
	else
	{
		// 前回と現在の割合範囲に入っているなら処理
		if (eventFrame < _lastPlayFrame) return;
		if (eventFrame > _curPlayingFrame) return;

		OnEvent();
	}
}
