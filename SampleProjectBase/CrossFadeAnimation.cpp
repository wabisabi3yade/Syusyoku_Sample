#include "pch.h"
#include "CrossFadeAnimation.h"

#include "Bone.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"

namespace HashiTaku
{
	using namespace DXSimp;

	CrossFadeAnimation::CrossFadeAnimation()
		: pFromNodePlayer(nullptr), 
		pToNodePlayer(nullptr),
		pAssetBoneList(nullptr), 
		transitionWeight(0.0f), 
		transitionTime(0.0f), 
		elapsedTime(0.0f), 
		easeKind(EaseKind::InOutCubic)
	{
	}

	void CrossFadeAnimation::Begin(AnimNodePlayer_Base& _fromNode, AnimNodePlayer_Base& _toNode, BoneList& _updateBones, float _transitionTime, EaseKind _easeKind)
	{
		pFromNodePlayer = &_fromNode;
		pToNodePlayer = &_toNode;
		pAssetBoneList = &_updateBones;
		elapsedTime = 0.0f;
		transitionTime = std::max(_transitionTime, Mathf::epsilon);
		easeKind = _easeKind;
	}

	void CrossFadeAnimation::Interpolate(const BoneTransform& _fromTransforms, const BoneTransform& _toTransforms, BoneTransform& _outTransforms)
	{
		// 補間計算する

		// 座標
		_outTransforms.position = Vector3::Lerp(_fromTransforms.position, _toTransforms.position, transitionWeight);

		// スケール
		_outTransforms.scale = Vector3::Lerp(_fromTransforms.scale, _toTransforms.scale, transitionWeight);

		// 回転量
		_outTransforms.rotation = Quaternion::Slerp(_fromTransforms.rotation, _toTransforms.rotation, transitionWeight);
	}

	bool CrossFadeAnimation::GetIsTransitionEnd() const
	{
		return transitionWeight >= 1.0f;
	}

	float CrossFadeAnimation::GetElapsedTime() const
	{
		return elapsedTime;
	}

	float CrossFadeAnimation::GetTransitionTime() const
	{
		return transitionTime;
	}

	float CrossFadeAnimation::GetTransitionWeight() const
	{
		return transitionWeight;
	}

	void CrossFadeAnimation::ProgressTime(float _deltaTime)
	{
		// 遷移時間を超えないように、時間を進める
		elapsedTime += _deltaTime;
		elapsedTime = std::min(elapsedTime, transitionTime);

		// 遷移割合をイージングを考慮した値で求める
		float ratio = elapsedTime / transitionTime;
		transitionWeight = Easing::EaseValue(ratio, easeKind);

		// 範囲におさめる
		transitionWeight = std::clamp(transitionWeight, 0.0f, 1.0f);
	}

	void CrossFadeAnimation::Update(float _deltaTime, float _controllerSpeed)
	{
		u_int boneCnt = pAssetBoneList->GetBoneCnt();

		// 遷移の時間を進め、アニメーションのウェイトを変化させる
		ProgressTime(_deltaTime);

		std::vector<BoneTransform> fromBoneTransforms;
		fromBoneTransforms.resize(boneCnt);
		std::vector<BoneTransform> toBoneTransforms;
		toBoneTransforms.resize(boneCnt);

		// ローカル行列で初期化
		for (u_int b_i = 0; b_i < boneCnt; b_i++)
		{
			const Bone& bone = *pAssetBoneList->GetBone(b_i);
			fromBoneTransforms[b_i] = bone.GetLocalNodeTransform();
			toBoneTransforms[b_i] = bone.GetLocalNodeTransform();
		}

		// 遷移元のアニメーション更新
		pFromNodePlayer->OnInterpolateUpdate(fromBoneTransforms, _deltaTime, _controllerSpeed);

		// 遷移先のアニメーション更新
		pToNodePlayer->OnInterpolateUpdate(toBoneTransforms, _deltaTime, _controllerSpeed);

		// ルートモーションは遷移先のものを使用
		Vector3 toMovement = pToNodePlayer->CalcRootMotionToTransform();
		pToNodePlayer->ApplyRootMotion(toMovement);

		// 補間したトランスフォームをボーンに適用させる
		
		for (u_int b_i = 0; b_i < boneCnt; b_i++)
		{
			Bone& bone = *pAssetBoneList->GetBone(b_i);

			BoneTransform interpTransform;
			Interpolate(fromBoneTransforms[b_i], toBoneTransforms[b_i], bone.GetRefelenceAnimTransform());
		}
	}
}