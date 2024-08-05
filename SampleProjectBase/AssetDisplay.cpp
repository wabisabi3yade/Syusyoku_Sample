#include "pch.h"
#include "AssetDisplay.h"

// アセット管理
#include "AssetCollection.h"

class Texture;
class Mesh_Group;

static void DrawAssetName(const std::string& _treeName, const AssetList& _assets);

void AssetDisplay::Draw()
{
	if (ImGui::TreeNode(ShiftJisToUtf8("アセット").c_str()))
	{
		AssetList& Tassets = pAssetCollection->GetAssetList<Texture>();
		DrawAssetName("テクスチャ", Tassets);

		AssetList& MBassets = pAssetCollection->GetAssetList<Mesh_Group>();
		DrawAssetName("モデル", MBassets);

		AssetList& Massets = pAssetCollection->GetAssetList<Material>();
		DrawAssetName("マテリアル", Massets);

		AssetList& Aassets = pAssetCollection->GetAssetList<AnimationData>();
		DrawAssetName("アニメーション", Aassets);

		AssetList& Bassets = pAssetCollection->GetAssetList<BoneList>();
		DrawAssetName("ボーン", Bassets);

		ImGui::TreePop();
	}
}

void DrawAssetName(const std::string& _treeName, const AssetList& _assets)
{
	// アセットを表示
	if (ImGui::TreeNode(TO_UTF8(_treeName)))
	{
		// アセットの名前を表示
		for (auto itr = _assets.begin(); itr != _assets.end(); itr++)
		{
			ImGui::Text(TO_UTF8(itr->first));
		}

		ImGui::TreePop();
	}
}
