#pragma once
#include "AssetPath_Base.h"

class AssetLoader;
/// @brief 効果音や曲のアセット(XTKAudioに対応)
class SoundAsset : public AssetPath_Base
{
	friend class AssetLoader;

	/// @brief ロードして作成した音データ
	std::unique_ptr<DirectX::SoundEffect>  pSoundData;

public:
	SoundAsset();
	~SoundAsset() {}

	/// @brief 音データを取得する
	/// @return 音データ
	DirectX::SoundEffect* GetSoundData() const;
private:
	/// @brief ロードされた音データをセット(moveで渡す)
	/// @param _pSetData 音データ
	void SetSoundData(std::unique_ptr<DirectX::SoundEffect> _pSetData);
};

