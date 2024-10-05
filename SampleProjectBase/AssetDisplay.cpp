#include "pch.h"
#include "AssetDisplay.h"

// アセット機能
#include "AssetLoader.h"
#include "MaterialCreater.h"
#include "AnimControllerCreater.h"

#include "SkeletalMesh.h"

class Texture;
class Mesh_Group;

using namespace DirectX::SimpleMath;

char AssetDisplay::inputText[IM_INPUT_BUF] = {};

void AssetDisplay::Draw()
{
	if (ImGuiMethod::TreeNode("Asset"))
	{
		DisplayTexture();
		DisplayModel();
		DisplayMaterial();
		DisplayAnimation();
		DisplayBoneList();
		DisplayAnimationController();

		ImGui::TreePop();
	}
}

void AssetDisplay::Display(const std::unordered_map<std::string, std::unique_ptr<Asset_Base>>& _assets)
{
	// アセットの名前を表示
	for (auto itr = _assets.begin(); itr != _assets.end(); itr++)
	{
		ImGui::Text(TO_UTF8(itr->first));
	}
}

void AssetDisplay::DisplayTexture()
{
	// アセットを表示
	if (!ImGuiMethod::TreeNode("Texture")) return;

	AssetList& Tassets = pAssetCollection->GetAssetList<Texture>();
	Display(Tassets);
	;
	ImGui::InputText("path", inputText, IM_INPUT_BUF);

	if (ImGui::Button("Load"))
		AssetLoader::TextureLoad(inputText);

	DeleteInputAsset();
	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<Texture>(inputText);

	ImGui::TreePop();
}

void AssetDisplay::DisplayModel()
{
	if (!ImGuiMethod::TreeNode("Mesh")) return;

	AssetList& assets = pAssetCollection->GetAssetList<Mesh_Group>();
	Display(assets);

	ImGui::InputText("path", inputText, IM_INPUT_BUF);

	static float scale = 1.0f;
	ImGui::DragFloat("scale", &scale);

	static bool isFlipY = false;
	ImGui::Checkbox("flipY", &isFlipY);

	static bool isRight = false;
	ImGui::Checkbox("rightHand", &isRight);

	static bool getSize = false;
	ImGui::Checkbox("getSize", &getSize);

	if (ImGui::Button("Load"))
		AssetLoader::ModelLoad(inputText, scale, isFlipY, isRight, getSize);

	ImGui::InputText("name", inputText, IM_INPUT_BUF);

	DeleteInputAsset();
	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<Mesh_Group>(inputText);

	ImGui::TreePop();
}

void AssetDisplay::DisplayMaterial()
{
	if (!ImGuiMethod::TreeNode("Material")) return;

	AssetList& assets = pAssetCollection->GetAssetList<Material>();
	Display(assets);

	DeleteInputAsset();
	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<Material>(inputText);

	ImGui::TreePop();
}

void AssetDisplay::DisplayAnimation()
{
	if (!ImGuiMethod::TreeNode("AnimationData")) return;

	AssetList& assets = pAssetCollection->GetAssetList<AnimationData>();
	Display(assets);

	static char animation[IM_INPUT_BUF] = "";
	ImGui::InputText("path", animation, IM_INPUT_BUF);

	static char boneList[IM_INPUT_BUF] = "";
	ImGui::InputText("boneList", boneList, IM_INPUT_BUF);

	static bool isRight = false;
	ImGui::Checkbox("rightHand", &isRight);

	if (ImGui::Button("Load"))
		AssetLoader::AnimationLoad(animation, boneList, isRight);

	DeleteInputAsset();
	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<AnimationData>(inputText);
	ImGui::TreePop();
}

void AssetDisplay::DisplayBoneList()
{
	if (!ImGuiMethod::TreeNode("Skeleton")) return;

	AssetList& boneAssets = pAssetCollection->GetAssetList<BoneList>();

	for (auto& asset : boneAssets)
	{
		if (!ImGuiMethod::TreeNode(asset.second->GetAssetName().c_str()))
			continue;

		BoneList& boneList = static_cast<BoneList&>(*asset.second);
		// ボーンの名前表示
		for (u_int b_i = 0; b_i < boneList.GetBoneCnt(); b_i++)
		{
			std::string boneName = boneList.GetBone(b_i).GetBoneName();
			ImGui::Text(boneName.c_str());
		}

		ImGui::TreePop();
	}

	DeleteInputAsset();
	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<BoneList>(inputText);

	ImGui::TreePop();
}

void AssetDisplay::DisplayAnimationController()
{
	if (!ImGuiMethod::TreeNode("AnimationController")) return;
	std::unique_ptr<AnimControllerCreater> pAnimConCreate = std::make_unique<AnimControllerCreater>();

	AssetList& animConList = pAssetCollection->GetAssetList<AnimationController>();

	// アニメーションコントローラ編集
	for (auto& asset : animConList)
	{
		std::string assetName = asset.second->GetAssetName();
		if (ImGuiMethod::TreeNode(assetName))
		{
			AnimationController& animCon = static_cast<AnimationController&>(*asset.second);
			ImGui::Begin(assetName.c_str());
			animCon.ImGuiCall();
			ImGui::End();
			ImGui::TreePop();
		}
	}
		

	ImGui::InputText("assetName", inputText, IM_INPUT_BUF);

	if (ImGui::Button("Create"))
	{
		pAnimConCreate->CraeteAsset(inputText);
	}

	DeleteInputAsset();
	if (ImGui::Button("Delete"))
		pAssetCollection->DeleteAsset<AnimationController>(inputText);

	ImGui::TreePop();
}

void AssetDisplay::DeleteInputAsset()
{
	ImGui::InputText("name", inputText, IM_INPUT_BUF);
	ImGui::SameLine();
}
