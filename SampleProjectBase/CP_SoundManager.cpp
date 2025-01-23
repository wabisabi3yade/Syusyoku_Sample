#include "pch.h"
#include "CP_SoundManager.h"

namespace HashiTaku
{
	int CP_SoundManager::PlaySE(const PlaySoundParameter& _seParameter, const DXSimp::Vector3& _soundPos)
	{
		return pDXSoundManager->Play(_seParameter, _soundPos, volumeTimes);
	}

	json CP_SoundManager::Save()
	{
		json data = SingletonComponent::Save();

		data["volumeTimes"] = volumeTimes;

		return data;
	}

	void CP_SoundManager::Load(const json& _data)
	{
		SingletonComponent::Load(_data);

		LoadJsonFloat("volumeTimes", volumeTimes, _data);
	}

	void CP_SoundManager::ImGuiDebug()
	{
		SingletonComponent::ImGuiDebug();

		ImGui::DragFloat("volume", &volumeTimes, 0.01f, 0.0f, 10.0f);
	}

	void CP_SoundManager::Awake()
	{
		SingletonComponent::Awake();

		// DirectXのサウンドマネジャーを取得
		pDXSoundManager = DXSoundManager::GetInstance();
	}

	void CP_SoundManager::OnDestroy()
	{
		SingletonComponent::OnDestroy();

		// BGMを再生しているなら停止させる
		if (curPlayBGMId != -1)
		{
			pDXSoundManager->StopSound(curPlayBGMId);
		}
	}

	int CP_SoundManager::PlayBGM(const PlaySoundParameter& _bgmParameter)
	{
		// BGMが再生されているならそれを止める
		if (curPlayBGMId != -1)
		{
			pDXSoundManager->StopSound(curPlayBGMId);
		}

		curPlayBGMId = pDXSoundManager->Play(_bgmParameter, DXSimp::Vector3::Zero, volumeTimes);
		return curPlayBGMId;
	}
}

