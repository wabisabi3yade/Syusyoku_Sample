#include "pch.h"
#include "AssetDisplay.h"

// ImGui�֘A
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// �A�Z�b�g�Ǘ�
#include "AssetCollection.h"

class Texture;
class Mesh_Base;

static void DrawAssetName(const std::string& _treeName, const AssetList& _assets);

void AssetDisplay::Draw()
{
#ifdef EDIT
	ImGui::Begin(ShiftJisToUtf8("�A�Z�b�g").c_str());

	AssetList& Tassets = pAssetCollection->GetAssetList<Texture>();
	DrawAssetName("�e�N�X�`��", Tassets);

	AssetList& MBassets = pAssetCollection->GetAssetList<Mesh_Base>();
	DrawAssetName("���f��", MBassets);

	AssetList& Massets = pAssetCollection->GetAssetList<Material>();
	DrawAssetName("�}�e���A��", Massets);

	ImGui::End();

#endif // EDIT
}

void DrawAssetName(const std::string& _treeName, const AssetList& _assets)
{
	// �A�Z�b�g��\��
	if (ImGui::TreeNode(ShiftJisToUtf8(_treeName).c_str()))
	{
		// �A�Z�b�g�̖��O��\��
		for (auto itr = _assets.begin(); itr != _assets.end(); itr++)
		{
			ImGui::Text(ShiftJisToUtf8(itr->first).c_str());
		}

		ImGui::TreePop();
	}
}
