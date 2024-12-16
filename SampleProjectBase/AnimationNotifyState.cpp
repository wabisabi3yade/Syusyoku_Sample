#include "pch.h"
#include "AnimationNotifyState.h"

namespace HashiTaku
{
	AnimationNotifyState::AnimationNotifyState(NotifyType _notifyType) :
		AnimationNotify_Base(_notifyType),
		startEventRatio(0.0f),
		endEventRatio(0.0f),
		startEventFrame(0),
		endEventFrame(0),
		curPlayRatio(0.0f),
		lastPlayRatio(-Mathf::smallValue)
	{
	}

	void AnimationNotifyState::Update(const float _lastPlayingRatio, const float _curPlayingRatio, const bool _isLoop)
	{
		if (!GetIsActive()) return;

		lastPlayRatio = _lastPlayingRatio;
		curPlayRatio = _curPlayingRatio;

		if (_isLoop)
		{
			if (_lastPlayingRatio > startEventRatio)
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

	nlohmann::json AnimationNotifyState::Save()
	{
		bool isUseFrame = UseFrame();
		auto data = AnimationNotify_Base::Save();
		data["useFrame"] = isUseFrame;

		if (isUseFrame)	// �t���[���Œ�������Ȃ�
		{
			data["startFrame"] = startEventFrame;
			data["endFrame"] = endEventFrame;
		}
		else
		{
			data["start"] = startEventRatio;
			data["end"] = endEventRatio;
		}

		return data;
	}

	void AnimationNotifyState::Load(const nlohmann::json& _data)
	{
		AnimationNotify_Base::Load(_data);

		bool isUseFrame = false;
		LoadJsonBoolean("useFrame", isUseFrame, _data);

		if (isUseFrame)
		{
			LoadJsonUnsigned("startFrame", startEventFrame, _data);
			LoadJsonUnsigned("endFrame", endEventFrame, _data);
			startEventRatio = ConvertFrameToRatio(startEventFrame);
			endEventRatio = ConvertFrameToRatio(endEventFrame);
		}
		else
		{
			LoadJsonFloat("start", startEventRatio, _data);
			LoadJsonFloat("end", endEventRatio, _data);
		}

	}

	float AnimationNotifyState::GetStartEventRatio() const
	{
		return startEventRatio;
	}

	float AnimationNotifyState::GetEndEventRatio() const
	{
		return endEventRatio;
	}

	float AnimationNotifyState::GetCurrentRatio() const
	{
		return curPlayRatio;
	}

	float AnimationNotifyState::GetLastRatio() const
	{
		return lastPlayRatio;
	}

	void AnimationNotifyState::ImGuiDebug()
	{
		AnimationNotify_Base::ImGuiDebug();

		if (UseFrame())
		{
			// �͈�
			ImGuiMethod::PushItemSmallWidth();

			// �J�n����
			int imInt = static_cast<int>(startEventFrame);
			ImGui::DragInt("start", &imInt, 1, 0, endEventFrame);
			startEventFrame = static_cast<u_int>(imInt);
			// �����ɂ�
			startEventRatio = ConvertFrameToRatio(startEventFrame);

			ImGui::SameLine();

			// �I������
			imInt = static_cast<int>(endEventFrame);
			ImGui::DragInt("end", &imInt, 1, startEventFrame, GetAnimFrameCnt());
			endEventFrame = static_cast<u_int>(imInt);
			endEventRatio = ConvertFrameToRatio(endEventFrame);

			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::DragFloatRange2("event", &startEventRatio, &endEventRatio, 0.001f, 0.0f, 1.0f);
		}

	}
}
