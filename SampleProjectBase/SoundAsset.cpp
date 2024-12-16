#include "pch.h"
#include "SoundAsset.h"

namespace HashiTaku
{
	SoundAsset::SoundAsset()
	{
	}

	DX::SoundEffect* SoundAsset::GetSoundData() const
	{
		return pSoundData.get();
	}

	void SoundAsset::SetSoundData(std::unique_ptr<DirectX::SoundEffect> _pSetData)
	{
		pSoundData = std::move(_pSetData);
	}
}