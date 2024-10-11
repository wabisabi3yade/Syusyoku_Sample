#include "pch.h"
#include "SingleAnimationNode.h"

// 慣性補間
#include "InertInterpAnimation.h"

#include "SkeletalMesh.h"

#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

void SingleAnimationNode::ImGuiPlaying()
{
	AnimationNode_Base::ImGuiPlaying();
}


void SingleAnimationNode::Update(BoneList& _boneList)
{
	float playingRatio = GetCurPlayRatio();

	////ボーン数ループ
	//for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	//{
	//	Bone& bone = _boneList.GetBone(b_i);

	//	BoneTransform transform;

	//	// 再生時間から各パラメータを取得
	//	//クォータニオン
	//	transform.rotation = pAnimationData->GetQuaternionByRatio(b_i, playingRatio);

	//	// 座標
	//	transform.position = pAnimationData->GetPositionByRatio(b_i, playingRatio);

	//	bone.SetAnimTransform(transform);
	//}
}

void SingleAnimationNode::SetAnimationData(const std::string& _animName)
{
	AnimationData* pData = AssetGetter::GetAsset<AnimationData>(_animName);

	if (!pData)	// 正常に取得できませんでした
	{
		HASHI_DEBUG_LOG(_animName + ":アニメーションを取得できませんでした");
		return;
	}

	pAnimationData = pData;
	SetAnimationTime(pAnimationData->GetAnimationTime());
}

const AnimationData& SingleAnimationNode::GetAnimationData() const
{
	return *pAnimationData;
}

void SingleAnimationNode::GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const
{
	pAnimationData->GetTransformByRatio(_boneId, _requestRatio, _outTransform);
}

const DirectX::SimpleMath::Vector3& SingleAnimationNode::GetRootMotionPosSpeed() const
{
	return pAnimationData->GetRootMotionPosSpeedPerSec();
}

nlohmann::json SingleAnimationNode::Save()
{
	auto data = AnimationNode_Base::Save();
	if (pAnimationData)
		data["animName"] = pAnimationData->GetAssetName();
	return data;
}

void SingleAnimationNode::Load(const nlohmann::json& _data)
{
	AnimationNode_Base::Load(_data);
	pAnimationData = HashiTaku::LoadJsonAsset<AnimationData>("animName", _data);
}

void SingleAnimationNode::ImGuiSetting()
{
	AnimationNode_Base::ImGuiSetting();

	std::string animName;
	if (pAnimationData)
	{
		animName = pAnimationData->GetAssetName();
	}

	if (AssetGetter::ImGuiGetCombobox<AnimationData>("animation", animName))
	{
		SetAnimationData(animName);
	}
}


