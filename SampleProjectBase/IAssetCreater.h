#pragma once
#include "AssetContacter.h"

// �A�Z�b�g���쐬����C���^�[�t�F�[�X
template<HashiTaku::AssetConcept T>
class IAssetCreater : public AssetContacter
{
public:
	IAssetCreater() {}
	virtual ~IAssetCreater() {}

	virtual T& CraeteAsset(const std::string& _assetName) = 0;
};

