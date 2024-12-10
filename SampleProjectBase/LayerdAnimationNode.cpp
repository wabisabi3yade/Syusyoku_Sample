#include "pch.h"
#include "LayerdAnimationNode.h"
#include "AssetGetter.h"

LayerdAnimationNode::LayerdAnimationNode(const std::string& _nodeName) :
	AnimationNode_Base(_nodeName, NodeType::Layerd),
	pBaseAnimation(nullptr),
	pBlendAnimation(nullptr),
	beginBlendBoneId(0)
{
}

void LayerdAnimationNode::GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const
{
	// 機能できていないなら
	if (!CanWarking()) return;

	pBaseAnimation->GetTransformByRatio(
		_boneId,
		_requestRatio,
		_outTransform);
}

nlohmann::json LayerdAnimationNode::Save()
{
	auto data = AnimationNode_Base::Save();

	if (pBaseAnimation)
		data["baseAnim"] = pBaseAnimation->GetAssetName();
	if (pBlendAnimation)
		data["blendAnim"] = pBlendAnimation->GetAssetName();

	data["beginBoneId"] = beginBlendBoneId;

	return data;
}

void LayerdAnimationNode::Load(const nlohmann::json& _data)
{
	AnimationNode_Base::Load(_data);

	std::string loadStr;
	if (HashiTaku::LoadJsonString("baseAnim", loadStr, _data))
	{
		pBaseAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
	}
	if (HashiTaku::LoadJsonString("blendAnim", loadStr, _data))
	{
		pBlendAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
	}

	HashiTaku::LoadJsonInteger("beginBoneId", beginBlendBoneId, _data);
}

bool LayerdAnimationNode::CanWarking() const
{
	// 基礎となるアニメーションがなかったら機能しない
	if (!pBaseAnimation)
	{
		assert(!"基礎アニメーションをセットしてね");
		return false;
	}
	return true;
}

void LayerdAnimationNode::ImGuiDebug()
{
	AnimationNode_Base::ImGuiDebug();

	// アニメーションをセットする
	static std::string str;
	AssetGetter::ImGuiGetCombobox<AnimationData>("Animation", str);
	if (ImGui::Button("Set Base"))
	{
		AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
		if (pGetData) pBaseAnimation = pGetData;
	}
	if (ImGui::Button("Set Blend"))
	{
		AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
		if (pGetData) pBlendAnimation = pGetData;
	}

	// ブレンド開始するボーンのIdをセット
	ImGui::InputInt("BlendBoneId", &beginBlendBoneId);
}
