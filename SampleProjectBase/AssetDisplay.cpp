#include "pch.h"
#include "AssetDisplay.h"

// �A�Z�b�g�Ǘ�
#include "AssetCollection.h"
#include "AssetLoader.h"

class Texture;
class Mesh_Group;

using namespace DirectX::SimpleMath;

constexpr u_int BUFFER_NUM(256);

void AssetDisplay::Draw()
{
	if (ImGui::TreeNode(ShiftJisToUtf8("�A�Z�b�g").c_str()))
	{
		DisplayTexture();

		DisplayModel();

		DisplayMaterial();

		DisplayAnimation();

		DisplayBoneList();

		ImGui::TreePop();
	}
}

void AssetDisplay::Display(const std::unordered_map<std::string, std::unique_ptr<Asset_Base>>& _assets)
{
	// �A�Z�b�g�̖��O��\��
	for (auto itr = _assets.begin(); itr != _assets.end(); itr++)
	{
		ImGui::Text(TO_UTF8(itr->first));
	}
}

void AssetDisplay::DisplayTexture()
{
	// �A�Z�b�g��\��
	if (!ImGui::TreeNode(TO_UTF8("�e�N�X�`��"))) return;

	AssetList& Tassets = pAssetCollection->GetAssetList<Texture>();
	Display(Tassets);

	static char str[BUFFER_NUM] = "";
	ImGui::InputText("path", str, BUFFER_NUM);

	if (ImGui::Button("Load"))
		AssetLoader::TextureLoad(str);

	std::string name = ImGuiMethod::InputText("name");

	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<Texture>(name);

	ImGui::TreePop();
}

void AssetDisplay::DisplayModel()
{
	if (!ImGui::TreeNode(TO_UTF8("���b�V��"))) return;

	AssetList& assets = pAssetCollection->GetAssetList<Mesh_Group>();
	Display(assets);

	static char str[BUFFER_NUM] = "";
	ImGui::InputText("path", str, BUFFER_NUM);

	static float scale = 1.0f;
	ImGui::DragFloat("scale", &scale);

	static bool isFlipY = false;
	ImGui::Checkbox("flipY", &isFlipY);

	static bool isRight = false;
	ImGui::Checkbox("rightHand", &isRight);

	static bool getSize = false;
	ImGui::Checkbox("getSize", &getSize);

	if (ImGui::Button("Load"))
		AssetLoader::ModelLoad(str, scale, isFlipY, isRight, getSize);

	std::string name = ImGuiMethod::InputText("name");

	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<Mesh_Group>(name);

	ImGui::TreePop();
}

void AssetDisplay::DisplayMaterial()
{
	if (!ImGui::TreeNode(TO_UTF8("�}�e���A��"))) return;

	AssetList& assets = pAssetCollection->GetAssetList<Material>();
	Display(assets);

	std::string name = ImGuiMethod::InputText("name");

	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<Material>(name);

	ImGui::TreePop();
}

void AssetDisplay::DisplayAnimation()
{
	if (!ImGui::TreeNode(TO_UTF8("�A�j���[�V����"))) return;

	AssetList& assets = pAssetCollection->GetAssetList<AnimationData>();
	Display(assets);

	static char animation[BUFFER_NUM] = "";
	ImGui::InputText("path", animation, BUFFER_NUM);

	static char boneList[BUFFER_NUM] = "";
	ImGui::InputText("boneList", boneList, BUFFER_NUM);

	static bool isRight = false;
	ImGui::Checkbox("rightHand", &isRight);

	if (ImGui::Button("Load"))
		AssetLoader::AnimationLoad(animation, boneList, isRight);

	std::string name = ImGuiMethod::InputText("name");

	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<AnimationData>(name);

	ImGui::TreePop();
}

void AssetDisplay::DisplayBoneList()
{
	if (!ImGui::TreeNode(TO_UTF8("�X�P���g��"))) return;

	AssetList& boneAssets = pAssetCollection->GetAssetList<BoneList>();
	Display(boneAssets);

	std::string name = ImGuiMethod::InputText("name");

	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<BoneList>(name);

	ImGui::TreePop();
}

template<class T>
void DeleteAsset()
{
	
}
