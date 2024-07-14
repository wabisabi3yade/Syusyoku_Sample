#include "pch.h"
#include "AssetDisplay.h"

// ImGui関連
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// アセット管理
#include "AssetCollection.h"

class Texture;
class Mesh_Base;

static void DrawAssetName(const std::string& _treeName, const AssetList& _assets);

void AssetDisplay::Draw()
{
#ifdef EDIT
	ImGui::Begin(ShiftJisToUtf8("アセット").c_str());

	AssetList& Tassets = pAssetCollection->GetAssetList<Texture>();
	DrawAssetName("テクスチャ", Tassets);

	AssetList& MBassets = pAssetCollection->GetAssetList<Mesh_Base>();
	DrawAssetName("モデル", MBassets);

	AssetList& Massets = pAssetCollection->GetAssetList<Material>();
	DrawAssetName("マテリアル", Massets);

	ImGui::End();

#endif // EDIT
}

void DrawAssetName(const std::string& _treeName, const AssetList& _assets)
{
	// アセットを表示
	if (ImGui::TreeNode(ShiftJisToUtf8(_treeName).c_str()))
	{
		// アセットの名前を表示
		for (auto itr = _assets.begin(); itr != _assets.end(); itr++)
		{
			ImGui::Text(ShiftJisToUtf8(itr->first).c_str());
		}

		ImGui::TreePop();
	}
}
