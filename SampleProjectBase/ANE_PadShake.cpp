#include "pch.h"
#include "ANE_PadShake.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	ANE_PadShake::ANE_PadShake() : 
		AnimationNotifyEvent(NotifyType::ANE_PadShake),
		padShakePower(0.4f),
		padShakeTime(0.2f)
	{
	}
	std::unique_ptr<AnimationNotify_Base> ANE_PadShake::Clone()
	{
		return std::make_unique<ANE_PadShake>(*this);
	}

	json ANE_PadShake::Save()
	{
		json data = AnimationNotifyEvent::Save();

		data["padPower"] = padShakePower;
		data["padTime"] = padShakeTime;

		return data;
	}

	void ANE_PadShake::Load(const json& _data)
	{
		AnimationNotifyEvent::Load(_data);

		LoadJsonFloat("padPower", padShakePower, _data);
		LoadJsonFloat("padTime", padShakeTime, _data);
	}

	void ANE_PadShake::OnEvent()
	{
		InSceneSystemManager::GetInstance()->GetInput().BeginVibration(padShakePower, padShakeTime);
	}

	void ANE_PadShake::ImGuiDebug()
	{
		AnimationNotifyEvent::ImGuiDebug();
		ImGui::DragFloat("Power", &padShakePower, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Time", &padShakeTime, 0.1f, 0.0f, 100.0f);
	}
}