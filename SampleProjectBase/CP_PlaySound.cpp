#include "pch.h"
#include "CP_PlaySound.h"
#include "CP_SoundManager.h"

namespace HashiTaku
{
	CP_PlaySound::CP_PlaySound() : soundId(-1), isBGM(false)
	{
	}

	void CP_PlaySound::OnDestroy()
	{
		DXSoundManager::GetInstance()->StopSound(soundId);
	}

	json CP_PlaySound::Save()
	{
		auto data = Component::Save();

		data["isBGM"] = isBGM;
		data["sound"] = soundParameter.Save();

		return data;
	}

	void CP_PlaySound::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonBoolean("isBGM", isBGM, _data);

		json loadData;
		if (LoadJsonData("sound", loadData, _data))
		{
			soundParameter.Load(loadData);
		}
	}

	void CP_PlaySound::Start()
	{
		// ‰¹‚ðÄ¶‚·‚é
		CP_SoundManager* pSound = CP_SoundManager::GetInstance();
		if (!pSound) return;

		// BGM‚©SE‚Æ‚µ‚ÄÄ¶
		if (isBGM)
			soundId = pSound->PlayBGM(soundParameter);
		else
			soundId = pSound->PlaySE(soundParameter, GetTransform().GetPosition());
	}

	void CP_PlaySound::ImGuiDebug()
	{
		Component::ImGuiDebug();

		ImGui::Checkbox("IsBGM", &isBGM);

		soundParameter.ImGuiCall();
	}
}