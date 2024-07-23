#include "pch.h"
#include "AssetDisplay.h"

// ImGui�֘A
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// �A�Z�b�g�Ǘ�
#include "AssetCollection.h"

class Texture;
class Mesh_Group;

static void DrawAssetName(const std::string& _treeName, const AssetList& _assets);

void AssetDisplay::Draw()
{
	if (ImGui::TreeNode(ShiftJisToUtf8("�A�Z�b�g").c_str()))
	{
		AssetList& Tassets = pAssetCollection->GetAssetList<Texture>();
		DrawAssetName("�e�N�X�`��", Tassets);

		AssetList& MBassets = pAssetCollection->GetAssetList<Mesh_Group>();
		DrawAssetName("���f��", MBassets);

		AssetList& Massets = pAssetCollection->GetAssetList<Material>();
		DrawAssetName("�}�e���A��", Massets);

		AssetList& Aassets = pAssetCollection->GetAssetList<AnimationData>();
		DrawAssetName("�A�j���[�V����", Aassets);

		ImGui::TreePop();
	}
}

void DrawAssetName(const std::string& _treeName, const AssetList& _assets)
{
	// �A�Z�b�g��\��
	if (ImGui::TreeNode(TO_UTF8(_treeName)))
	{
		// �A�Z�b�g�̖��O��\��
		for (auto itr = _assets.begin(); itr != _assets.end(); itr++)
		{
			ImGui::Text(TO_UTF8(itr->first));
		}

		ImGui::TreePop();
	}
}
