#pragma once
#include "IAssetCreater.h"
#include "Material.h"

namespace HashiTaku
{
	/// @brief �}�e���A���쐬�N���X
	class MaterialCreater
	{
	public:
		MaterialCreater() {}
		~MaterialCreater() {}

		static Material& CraeteAsset(const std::string& _assetName);
	};
}