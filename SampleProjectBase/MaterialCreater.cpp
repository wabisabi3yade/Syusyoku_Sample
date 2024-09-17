#include "pch.h"
#include "MaterialCreater.h"
#include "AssetSetter.h"
#include "AssetGetter.h"

Material& MaterialCreater::CraeteAsset(const std::string& _assetName)
{
	if (AssetSetter::CheckImport<Material>(_assetName))	// ���ɂ���Ȃ炻���Ԃ�
		return *AssetGetter::GetAsset<Material>(_assetName);

	std::unique_ptr<Material> pCreateMaterial = std::make_unique<Material>();
	return *AssetSetter::SetAsset<Material>(_assetName, std::move(pCreateMaterial));
}
