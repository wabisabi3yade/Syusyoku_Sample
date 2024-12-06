#pragma once
#include "AssetPath_Base.h"

class AssetLoader;
/// @brief ���ʉ���Ȃ̃A�Z�b�g(XTKAudio�ɑΉ�)
class SoundAsset : public AssetPath_Base
{
	friend class AssetLoader;

	/// @brief ���[�h���č쐬�������f�[�^
	std::unique_ptr<DirectX::SoundEffect>  pSoundData;

public:
	SoundAsset();
	~SoundAsset() {}

	/// @brief ���f�[�^���擾����
	/// @return ���f�[�^
	DirectX::SoundEffect* GetSoundData() const;
private:
	/// @brief ���[�h���ꂽ���f�[�^���Z�b�g(move�œn��)
	/// @param _pSetData ���f�[�^
	void SetSoundData(std::unique_ptr<DirectX::SoundEffect> _pSetData);
};

