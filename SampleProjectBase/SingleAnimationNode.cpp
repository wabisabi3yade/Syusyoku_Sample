#include "pch.h"
#include "SingleAnimationNode.h"

// 慣性補間
#include "InertInterpAnimation.h"
#include "SkeletalMesh.h"
#include "AssetGetter.h"

namespace HashiTaku
{
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

	u_int SingleAnimationNode::GetRatioToKey(float _ratio) const
	{
		return pAnimationData->GetRatioToFrame(_ratio);
	}

	u_int SingleAnimationNode::GetAllKeyFrame() const
	{
		return pAnimationData->GetAllAnimationFrame();
	}

	const DXSimp::Vector3& SingleAnimationNode::GetRootMotionPosSpeed() const
	{
		return pAnimationData->GetRootMotionPosSpeedPerSec();
	}

	json SingleAnimationNode::Save()
	{
		auto data = AnimationNode_Base::Save();

		if (pAnimationData)
			data["animName"] = pAnimationData->GetAssetName();

		return data;
	}

	void SingleAnimationNode::Load(const json& _data)
	{
		AnimationNode_Base::Load(_data);

		std::string animName;
		if (LoadJsonString("animName", animName, _data))
			SetAnimationData(animName);
	}

	void SingleAnimationNode::ImGuiDebug()
	{
		AnimationNode_Base::ImGuiDebug();

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
}