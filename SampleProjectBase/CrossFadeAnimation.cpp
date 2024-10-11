#include "pch.h"
#include "CrossFadeAnimation.h"

#include "Bone.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

CrossFadeAnimation::CrossFadeAnimation()
	: pFromNodePlayer(nullptr), pToNodePlayer(nullptr), pBoneList(nullptr), transitionWeight(0.0f), transitionTime(0.0f), elapsedTime(0.0f), easeKind(HashiTaku::EaseKind::InOutCubic)
{
}

void CrossFadeAnimation::Begin(AnimNodePlayer_Base& _fromNode, AnimNodePlayer_Base& _toNode, BoneList& _updateBones, float _transitionTime, HashiTaku::EaseKind _easeKind)
{
	pFromNodePlayer = &_fromNode;
	pToNodePlayer = &_toNode;
	pBoneList = &_updateBones;
	elapsedTime = 0.0f;
	transitionTime = _transitionTime;
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

	HASHI_DEBUG_LOG(std::to_string(transitionWeight));
}

void CrossFadeAnimation::Update(float _playSpeed)
{
	// アニメーション再生の速度を考慮する
	float deltaTime = MainApplication::DeltaTime() * _playSpeed;

	ProgressTime(deltaTime);

	// 更新処理
	std::vector<BoneTransform> fromBoneTransforms;
	pFromNodePlayer->UpdateCall(fromBoneTransforms, _playSpeed);

	std::vector<BoneTransform> toBoneTransforms;
	pToNodePlayer->UpdateCall(toBoneTransforms, _playSpeed);

	// 補間したトランスフォームをボーンに適用させる
	u_int boneCnt = pBoneList->GetBoneCnt();
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);

		BoneTransform interpTransform;
		Interpolate(fromBoneTransforms[b_i], toBoneTransforms[b_i], bone.GetRefelenceAnimTransform());
	}
}
