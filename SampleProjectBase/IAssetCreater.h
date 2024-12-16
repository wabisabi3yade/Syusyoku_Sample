#pragma once
#include "AssetContacter.h"

namespace HashiTaku
{
	// �A�Z�b�g���쐬����C���^�[�t�F�[�X
	template<AssetConcept T>
	class IAssetCreater : public AssetContacter
	{
	public:
		IAssetCreater() {}
		virtual ~IAssetCreater() {}

		virtual T& CraeteAsset(const std::string& _assetName) = 0;
	};
}