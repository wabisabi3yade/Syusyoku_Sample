#include "pch.h"
#include "AssetSaveLoad.h"

#include "AssetCollection.h"
#include "AssetLoader.h"

#include "Texture.h"
#include "Material.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"
#include "AnimationData.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

const std::string ASSET_ELEMENT = "assets";
const std::string TEX_ELEMENT = "textures";
const std::string MOD_ELEMENT = "models";
const std::string MAT_ELEMENT = "materials";
const std::string ANIM_ELEMENT = "animations";

void AssetSaveLoader::Save()
{
	// シーンデータ
	nlohmann::json saveData;

	auto& assetData = saveData[ASSET_ELEMENT];

	auto& texData = assetData[TEX_ELEMENT];
	for (auto& tex : pAssetCollection->textureAssets)
	{
		if (tex.second->GetIsSave())	// セーブするなら
			texData.push_back(tex.second->Save());
	}

	auto& modelDatas = assetData[MOD_ELEMENT];
	for (auto& model : pAssetCollection->modelAssets)
	{
		if (model.second->GetIsSave())
			modelDatas.push_back(model.second->Save());
	}

	auto& materialData = assetData[MAT_ELEMENT];
	for (auto& material : pAssetCollection->materialAssets)
	{
		if (material.second->GetIsSave())
			materialData.push_back(material.second->Save());
	}

	auto& animData = assetData[ANIM_ELEMENT];
	for (auto& anim : pAssetCollection->animationAssets)
	{
		if (anim.second->GetIsSave())
			animData.push_back(anim.second->Save());
	}

	std::ofstream f(SaveFilePath());

	auto str = assetData.dump(4);
	auto len = str.length();
	f.write(str.c_str(), len);
}

void AssetSaveLoader::Load()
{
	using namespace HashiTaku;

	// ファイルからjsonを読み込み
	std::ifstream f(SaveFilePath());

	if (!f.is_open())
	{
		HASHI_DEBUG_LOG("アセットセーブファイルが開けませんでした");
		return;
	}

	const nlohmann::json loadData = nlohmann::json::parse(f);

	// テクスチャ
	CreateTexture(loadData);

	// マテリアル
	CreateMaterial(loadData);

	// メッシュ
	CreateMesh(loadData);

	// アニメーション
	CreateAnimaton(loadData);

	// アセットをロードする
	LoadAsset(loadData);
}

void AssetSaveLoader::CreateTexture(const nlohmann::json& _loadData)
{
	for (auto& data : _loadData[TEX_ELEMENT])
	{
		std::string path;
		LoadJsonString("pathName", path, data);
		
		AssetLoader::TextureLoad(path);
	}
}

void AssetSaveLoader::CreateMaterial(const nlohmann::json& _loadData)
{
	// 名前を取得し、アセット生成
	for (auto& data : _loadData[MAT_ELEMENT])
	{
		std::unique_ptr<Material> pCreate = std::make_unique<Material>();
		std::string name;
		LoadJsonString("assetName", name, data);

		pAssetCollection->SetAsset<Material>(name, std::move(pCreate));
	}
}

void AssetSaveLoader::CreateMesh(const nlohmann::json& _loadData)
{
	for (auto& data : _loadData[MOD_ELEMENT])
	{
		std::string path;
		LoadJsonString("pathName", path, data);

		float scale = 1.0f;
		LoadJsonFloat("loadScale", scale, data);

		bool isFlipY = false;
		LoadJsonBoolean("flipY", isFlipY, data);

		bool isGetSize = false;
		LoadJsonBoolean("getSize", isGetSize, data);

		bool isRightHand = false;
		LoadJsonBoolean("rightHand", isRightHand, data);

		AssetLoader::ModelLoad(path, scale, true , isRightHand, isGetSize);
	}
}

void AssetSaveLoader::CreateAnimaton(const nlohmann::json& _loadData)
{
	for (auto& data : _loadData[ANIM_ELEMENT])
	{
		std::string path;
		LoadJsonString("pathName", path, data);

		std::string boneName;
		LoadJsonString("boneListName", boneName, data);

		bool isRightHand = false;
		LoadJsonBoolean("rightHand", isRightHand, data);

		AssetLoader::AnimationLoad(path, boneName, isRightHand);
	}
}

void AssetSaveLoader::LoadAsset(const nlohmann::json& _loadData)
{
	for (const auto& asset : _loadData[TEX_ELEMENT])
	{
		std::string assetName;
		LoadJsonString("assetName", assetName, asset);

		Asset_Base* pAsset = pAssetCollection->GetAsset<Texture>(assetName);
		pAsset->Load(asset);
	}

	for (const auto& asset : _loadData[MAT_ELEMENT])
	{
		std::string assetName;
		LoadJsonString("assetName", assetName, asset);

		Asset_Base* pAsset = pAssetCollection->GetAsset<Material>(assetName);
		pAsset->Load(asset);
	}

	for (const auto& asset : _loadData[MOD_ELEMENT])
	{
		std::string assetName;
		LoadJsonString("assetName", assetName, asset);

		Asset_Base* pAsset = pAssetCollection->GetAsset<Mesh_Group>(assetName);
		pAsset->Load(asset);
	}

	for (const auto& asset : _loadData[ANIM_ELEMENT])
	{
		std::string assetName;
		LoadJsonString("assetName", assetName, asset);

		Asset_Base* pAsset = pAssetCollection->GetAsset<AnimationData>(assetName);
	
		pAsset->Load(asset);
	}
}


std::string AssetSaveLoader::SaveFilePath()
{
	std::string path = "assets/data/asset/collection.json";

	return path;
}
