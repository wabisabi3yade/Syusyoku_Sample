#include "pch.h"
#include "ANE_PlaySound.h"
#include "CP_SoundManager.h"

namespace HashiTaku
{
	ANE_PlaySound::ANE_PlaySound() : 
		AnimationNotifyEvent(NotifyType::ANE_PlaySound)
	{
	}
	
	std::unique_ptr<AnimationNotify_Base> ANE_PlaySound::Clone()
	{
		return std::make_unique<ANE_PlaySound>(*this);
	}

	json ANE_PlaySound::Save()
	{
		auto data = AnimationNotifyEvent::Save();

		data["parameter"] = soundParameter.Save();

		return data;
	}

	void ANE_PlaySound::Load(const json& _data)
	{
		AnimationNotifyEvent::Load(_data);

		json soundData;
		if (LoadJsonData("parameter", soundData, _data))
		{
			soundParameter.Load(soundData);
		}
	}

	void ANE_PlaySound::OnEvent()
	{
		// Ä¶‚·‚é
		CP_SoundManager* pSound = CP_SoundManager::GetInstance();
		if (!pSound) return;

		pSound->PlaySE(soundParameter);
	}

	void ANE_PlaySound::ImGuiDebug()
	{
		AnimationNotifyEvent::ImGuiDebug();

		soundParameter.ImGuiCall();
	}
}

