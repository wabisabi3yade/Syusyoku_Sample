#include "pch.h"
#include "AssetSaveLoad.h"
#include "AssetCollection.h"
#include "AssetLoader.h"
#include "AnimControllerCreater.h"

#include "Texture.h"
#include "Material.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"
#include "VisualEffect.h"
#include "AnimationData.h"

namespace HashiTaku
{
	constexpr auto ASSET_ELEMENT = "assets";
	constexpr auto TEX_ELEMENT = "textures";
	constexpr auto MOD_ELEMENT = "models";
	constexpr auto MAT_ELEMENT = "materials";
	constexpr auto VFX_ELEMENT = "vfx";
	constexpr auto ANIM_ELEMENT = "animations";
	constexpr auto ANIMCON_ELEMENT = "animController";

	void AssetSaveLoader::Save()
	{
		// シーンデータ
		json saveData;

		auto& assetData = saveData[ASSET_ELEMENT];

		// テクスチャ
		auto& texData = assetData[TEX_ELEMENT];
		for (auto& tex : pAssetCollection->textureAssets)
		{
			if (tex.second->GetIsSave())	// セーブするなら
				texData.push_back(tex.second->Save());
		}

		// モデル
		auto& modelDatas = assetData[MOD_ELEMENT];
		for (auto& model : pAssetCollection->modelAssets)
		{
			if (model.second->GetIsSave())
				modelDatas.push_back(model.second->Save());
		}

		// マテリアル
		auto& materialData = assetData[MAT_ELEMENT];
		for (auto& material : pAssetCollection->materialAssets)
		{
			if (material.second->GetIsSave())
				materialData.push_back(material.second->Save());
		}

		// エフェクト
		auto& vfxData = assetData[VFX_ELEMENT];
		for (auto& vfx : pAssetCollection->vfxAssets)
		{
			if (vfx.second->GetIsSave())
				vfxData.push_back(vfx.second->Save());
		}

		// アニメーション
		auto& animData = assetData[ANIM_ELEMENT];
		for (auto& anim : pAssetCollection->animationAssets)
		{
			if (anim.second->GetIsSave())
				animData.push_back(anim.second->Save());
		}

		// アニメーションコントローラ
		auto& animConData = assetData[ANIMCON_ELEMENT];
		for (auto& animCon : pAssetCollection->animControllerAssets)
		{
			if (animCon.second->GetIsSave())
				animConData.push_back(animCon.second->Save());
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

		const json loadData = json::parse(f);

		// テクスチャ
		CreateTexture(loadData);

		// マテリアル
		CreateMaterial(loadData);

		// メッシュ
		CreateMesh(loadData);

		// エフェクトを作成
		CreateVFX(loadData);

		// アニメーション
		CreateAnimaton(loadData);

		//// アニメーションコントローラー
		CreateAnimController(loadData);

		// アセットをロードする
		LoadAsset(loadData);
	}

	void AssetSaveLoader::CreateTexture(const json& _loadData)
	{
		if (!IsJsonContains(_loadData, TEX_ELEMENT)) return;

		for (auto& data : _loadData[TEX_ELEMENT])
		{
			std::string path;
			LoadJsonString("pathName", path, data);

			AssetLoader::TextureLoad(path);
		}
	}

	void AssetSaveLoader::CreateMaterial(const json& _loadData)
	{
		if (!IsJsonContains(_loadData, MAT_ELEMENT)) return;

		// 名前を取得し、アセット生成
		for (auto& data : _loadData[MAT_ELEMENT])
		{
			std::unique_ptr<Material> pCreate = std::make_unique<Material>();
			std::string name;
			LoadJsonString("assetName", name, data);

			pAssetCollection->SetAsset<Material>(name, std::move(pCreate));
		}
	}

	void AssetSaveLoader::CreateVFX(const json& _loadData)
	{
		if (!IsJsonContains(_loadData, VFX_ELEMENT)) return;

		// 名前を取得し、アセット生成
		for (auto& data : _loadData[VFX_ELEMENT])
		{
			std::string path;
			if (!LoadJsonString("pathName", path, data)) continue;

			float loadScale = 1.0f;
			if (!LoadJsonFloat("loadScale", loadScale, data)) return;

			// エフェクトをロード
			AssetLoader::VFXLoadForEffekseer(path, loadScale);
		}
	}

	void AssetSaveLoader::CreateMesh(const json& _loadData)
	{
		if (!IsJsonContains(_loadData, MOD_ELEMENT)) return;

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

			AssetLoader::ModelLoad(path, scale, true, isRightHand, isGetSize);
		}
	}

	void AssetSaveLoader::CreateAnimaton(const json& _loadData)
	{
		if (!IsJsonContains(_loadData, ANIM_ELEMENT)) return;

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

	void AssetSaveLoader::CreateAnimController(const json& _loadData)
	{
		if (!IsJsonContains(_loadData, ANIMCON_ELEMENT)) return;

		AnimControllerCreater animConCreater;

		// 名前を取得し、アセット生成
		for (auto& data : _loadData[ANIMCON_ELEMENT])
		{
			std::unique_ptr<AnimationController> pCreate;
			;
			std::string name;
			LoadJsonString("assetName", name, data);

			animConCreater.CraeteAsset(name);
		}
	}

	void AssetSaveLoader::LoadAsset(const json& _loadData)
	{
		json assetDatas;

		if (LoadJsonDataArray(TEX_ELEMENT, assetDatas, _loadData))
		{
			for (const auto& asset : assetDatas)
			{
				std::string assetName;
				LoadJsonString("assetName", assetName, asset);

				Asset_Base* pAsset = pAssetCollection->GetAsset<Texture>(assetName);

				if (pAsset)
					pAsset->Load(asset);
			}
		}

		if (LoadJsonDataArray(MAT_ELEMENT, assetDatas, _loadData))
		{
			for (const auto& asset : assetDatas)
			{
				std::string assetName;
				LoadJsonString("assetName", assetName, asset);

				Asset_Base* pAsset = pAssetCollection->GetAsset<Material>(assetName);

				if (pAsset)
					pAsset->Load(asset);
			}
		}

		if (LoadJsonDataArray(MOD_ELEMENT, assetDatas, _loadData))
		{
			for (const auto& asset : assetDatas)
			{
				std::string assetName;
				LoadJsonString("assetName", assetName, asset);

				Asset_Base* pAsset = pAssetCollection->GetAsset<Mesh_Group>(assetName);

				if (pAsset)
					pAsset->Load(asset);
			}
		}

		if (LoadJsonDataArray(VFX_ELEMENT, assetDatas, _loadData))
		{
			for (const auto& asset : assetDatas)
			{
				std::string assetName;
				LoadJsonString("assetName", assetName, asset);

				Asset_Base* pAsset = pAssetCollection->GetAsset<VisualEffect>(assetName);

				if (pAsset)
					pAsset->Load(asset);
			}
		}

		if (LoadJsonDataArray(ANIM_ELEMENT, assetDatas, _loadData))
		{
			for (const auto& asset : assetDatas)
			{
				std::string assetName;
				LoadJsonString("assetName", assetName, asset);

				Asset_Base* pAsset = pAssetCollection->GetAsset<AnimationData>(assetName);

				if (pAsset)
					pAsset->Load(asset);
			}
		}

		if (LoadJsonDataArray(ANIMCON_ELEMENT, assetDatas, _loadData))
		{
			for (const auto& asset : assetDatas)
			{
				std::string assetName;
				LoadJsonString("assetName", assetName, asset);

				Asset_Base* pAsset = pAssetCollection->GetAsset<AnimationController>(assetName);

				if (pAsset)
					pAsset->Load(asset);
			}
		}
	}


	std::string AssetSaveLoader::SaveFilePath()
	{
		std::string path = "assets/data/asset/collection.json";

		return path;
	}
}