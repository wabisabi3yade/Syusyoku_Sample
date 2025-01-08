#include "pch.h"
#include "CP_SoundManager.h"

namespace HashiTaku
{
	int CP_SoundManager::PlaySE(const PlaySoundParameter& _seParameter, const DXSimp::Vector3& _soundPos)
	{
		return pDXSoundManager->Play(_seParameter, _soundPos);
	}

	void CP_SoundManager::Awake()
	{
		SingletonComponent::Awake();

		pDXSoundManager = DXSoundManager::GetInstance();
	}

	void CP_SoundManager::OnDestroy()
	{
		SingletonComponent::OnDestroy();

		if (curPlayBGMId != -1)
		{
			pDXSoundManager->StopSound(curPlayBGMId);
		}
	}

	int CP_SoundManager::PlayBGM(const PlaySoundParameter& _bgmParameter)
	{
		// BGM‚ªÄ¶‚³‚ê‚Ä‚¢‚é‚È‚ç‚»‚ê‚ðŽ~‚ß‚é
		if (curPlayBGMId != -1)
		{
			pDXSoundManager->StopSound(curPlayBGMId);
		}

		curPlayBGMId = pDXSoundManager->Play(_bgmParameter);

		return curPlayBGMId;
	}
}

