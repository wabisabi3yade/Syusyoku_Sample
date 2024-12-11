#include "pch.h"
#include "LayerdAnimationNode.h"
#include "AssetGetter.h"
#include "SkeletalMesh.h"

constexpr int NOT_SET_BONEID(-1);	// ボーンを指定していない時のボーンID

LayerdAnimationNode::LayerdAnimationNode(const BoneList& _boneList, const std::string& _nodeName) :
	AnimationNode_Base(_nodeName, NodeType::Layerd),
	pBoneList(&_boneList),
	pBaseAnimation(nullptr),
	pBlendAnimation(nullptr),
	beginBlendPlayFrame(0),
	beginBlendBoneId(NOT_SET_BONEID)
{
	blendSpeedCurve.SetCurveName("Blend Speed");
}

void LayerdAnimationNode::GetAnimTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const
{
	// 機能できていないなら
	if (!CanWarking()) return;

	// ベース側
	u_int baseBoneCnt = static_cast<u_int>(baseBoneIds.size());
	for (u_int b_i = 0; b_i < baseBoneCnt; b_i++)
	{
		u_int uBoneId = static_cast<u_int>(baseBoneIds[b_i]);
		pBaseAnimation->GetTransformByRatio(uBoneId, _baseRatio, _outTransforms[uBoneId]);
	}

	// ブレンド側
	u_int blendBoneCnt = static_cast<u_int>(blendBoneIds.size());
	for (u_int b_i = 0; b_i < blendBoneCnt; b_i++)
	{
		u_int uBoneId = static_cast<u_int>(blendBoneIds[b_i]);
		pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, _outTransforms[uBoneId]);
	}
}

float LayerdAnimationNode::GetBlendCurveSpeed(float _playRatio) const
{
	return blendSpeedCurve.GetValue(_playRatio);
}

float LayerdAnimationNode::GetBlendAnimationTime() const
{
	if (!pBlendAnimation) return 1.0f;

	return pBlendAnimation->GetAnimationTime();
}

float LayerdAnimationNode::GetBeginBlendPlayRatio() const
{
	if (!pBlendAnimation) return 0.0f;
	return static_cast<float>(beginBlendPlayFrame) / pBlendAnimation->GetAllAnimationFrame();
}

int LayerdAnimationNode::GetBeginBlendBoneId() const
{
	return beginBlendBoneId;
}

nlohmann::json LayerdAnimationNode::Save()
{
	auto data = AnimationNode_Base::Save();

	data["blendBeginFrame"] = beginBlendPlayFrame;
	data["blendCurve"] = blendSpeedCurve.Save();

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

	HashiTaku::LoadJsonUnsigned("blendBeginFrame", beginBlendPlayFrame, _data);
	nlohmann::json blendCurveData;
	if (HashiTaku::LoadJsonData("blendCurve", blendCurveData, _data))
	{
		blendSpeedCurve.Load(blendCurveData);
	}

	std::string loadStr;
	if (HashiTaku::LoadJsonString("baseAnim", loadStr, _data))
	{
		pBaseAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
		SetAnimationTime(pBaseAnimation->GetAnimationTime());
	}
	if (HashiTaku::LoadJsonString("blendAnim", loadStr, _data))
	{
		pBlendAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
		beginBlendPlayFrame = 0;
	}
	if (HashiTaku::LoadJsonInteger("beginBoneId", beginBlendBoneId, _data))
	{
		SetBlendBeginBone(beginBlendBoneId);
	}
}

bool LayerdAnimationNode::CanWarking() const
{
#ifdef EDIT
	// 基礎となるアニメーションがなかったら機能しない
	if (!pBaseAnimation)
	{
		assert(!"基礎アニメーションをセットしてね");
		return false;
	}

	if (!pBlendAnimation)
	{
		assert(!"ブレンドアニメーションをセットしてね");
		return false;
	}
#endif // EDIT

	return true;
}

void LayerdAnimationNode::SetBlendBeginBone(int _beginBoneId)
{
	// ボーンの数より大きい　または　0以下なら処理しない
	if (_beginBoneId >= static_cast<int>(pBoneList->GetBoneCnt()) ||
		_beginBoneId < 0) return;

	beginBlendBoneId = _beginBoneId;

	// ベース側とブレンド側でボーンを分けていく
	baseBoneIds.clear();
	blendBoneIds.clear();
	SeparateBoneGroup(*pBoneList->GetRootNode());
}

void LayerdAnimationNode::SeparateBoneGroup(const TreeNode& _treeNode)
{
	if (_treeNode.HasBone())
	{
		u_int boneId = _treeNode.GetBoneIdx();

		if (boneId == beginBlendBoneId) // ブレンド開始ボーンだったら
		{
			// ブレンドグループを作成する
			CreateBlendBoneGroup(_treeNode);
			return; // 以降のボーンは処理しない
		}

		baseBoneIds.push_back(boneId);	// ベースに追加
	}

	u_int childCnt = _treeNode.GetChildNum();
	// 再帰で分けていく
	for (u_int c_i = 0; c_i < childCnt; c_i++)
		SeparateBoneGroup(*_treeNode.GetChild(c_i));
}

void LayerdAnimationNode::CreateBlendBoneGroup(const TreeNode& _blendBoneNode)
{
	if (_blendBoneNode.HasBone())
		blendBoneIds.push_back(_blendBoneNode.GetBoneIdx());	// ブレンドに追加

	// 再帰で分けていく
	u_int childCnt = _blendBoneNode.GetChildNum();
	for (u_int c_i = 0; c_i < childCnt; c_i++)
		CreateBlendBoneGroup(*_blendBoneNode.GetChild(c_i));
}

void LayerdAnimationNode::ImGuiDebug()
{
#ifdef EDIT
	AnimationNode_Base::ImGuiDebug();

	// ブレンド側の開始フレーム
	u_int maxFrame = 0;
	if (pBlendAnimation) maxFrame = pBlendAnimation->GetAllAnimationFrame();
	ImGui::DragScalar("BlendBeginFrame", ImGuiDataType_U32, &beginBlendPlayFrame, 1, 0, &maxFrame);

	// ブレンド側の速度カーブ
	blendSpeedCurve.ImGuiCall();

	// アニメーションをセットする
	static std::string str;
	AssetGetter::ImGuiGetCombobox<AnimationData>("Animation", str);
	if (ImGui::Button("Set Base"))
	{
		AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
		if (pGetData)
		{
			pBaseAnimation = pGetData;
			SetAnimationTime(pBaseAnimation->GetAnimationTime());
		}
	}
	ImGui::SameLine();

	// アニメーション名表示
	std::string animName = "Null";
	if (pBaseAnimation)
		animName = pBaseAnimation->GetAssetName();
	ImGui::Text(animName.c_str());

	if (ImGui::Button("Set Blend"))
	{
		AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
		if (pGetData) pBlendAnimation = pGetData;
	}
	ImGui::SameLine();
	// アニメーション名表示
	animName = "Null";
	if (pBlendAnimation)
		animName = pBlendAnimation->GetAssetName();
	ImGui::Text(animName.c_str());

	// ブレンド開始するボーンのIdをセット
	if (ImGui::DragInt("BlendBoneId", &beginBlendBoneId, 1, 0, (int)pBoneList->GetBoneCnt()))
	{
		SetBlendBeginBone(beginBlendBoneId);
	}
#endif // EDIT
}
