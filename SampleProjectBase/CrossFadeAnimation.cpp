#include "pch.h"
#include "CrossFadeAnimation.h"

#include "Bone.h"
#include "AnimationNode_Base.h"
#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

CrossFadeAnimation::CrossFadeAnimation()
	: pFromNode(nullptr), pToNode(nullptr), transitionWeight(0.0f), transitionTime(0.0f), elapsedTime(0.0f), curFromRatio(0.0f), curToRatio(0.0f), easeKind(HashiTaku::EaseKind::InOutCubic)
{
}

void CrossFadeAnimation::Begin(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode, float _transitionTime, float _fromStartRatio, float _toStartRatio, HashiTaku::EaseKind _easeKind)
{
	pFromNode = &_fromNode;
	pToNode = &_toNode;

	elapsedTime = 0.0f;
	transitionTime = _transitionTime;
	curFromRatio = _fromStartRatio;
	curToRatio = _toStartRatio;
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

void CrossFadeAnimation::SetFromCurRatio(float _fromRatio)
{
	curFromRatio = _fromRatio;
	curFromRatio = std::clamp(curFromRatio, 0.0f, 1.0f);
}

void CrossFadeAnimation::SetToCurRatio(float _toRatio)
{
	curToRatio = _toRatio;
	curToRatio = std::clamp(curToRatio, 0.0f, 1.0f);
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

void CrossFadeAnimation::ProgressTime(float _deltaTime)
{
	// 遷移時間を超えないように、時間を進める
	elapsedTime += _deltaTime;
	elapsedTime = std::min(elapsedTime, transitionTime);

	// 遷移割合をイージングを考慮した値で求める
	float ratio = elapsedTime / transitionTime;
	transitionWeight = Easing::EaseValue(ratio, easeKind);
}

void CrossFadeAnimation::Update(BoneList& _updateBones, float _playSpeed)
{
	// アニメーション再生の速度を考慮する
	float deltaTime = MainApplication::DeltaTime() * _playSpeed;

	ProgressTime(deltaTime);

	// 割合を進める
	float fromTime = pFromNode->GetAnimationTime() * curFromRatio;
	fromTime += deltaTime;
	curFromRatio = fromTime / pFromNode->GetAnimationTime();

	if (curFromRatio > 1.0f)	// 割合が1超えたら
	{
		if (pFromNode->GetIsLoop())
			curFromRatio -= 1.0f;
		else
			curFromRatio = 1.0f;
	}

	// 割合を進める
	float toTime = pToNode->GetAnimationTime() * curToRatio;
	toTime += deltaTime;
	curToRatio = toTime / pToNode->GetAnimationTime();

	if (curToRatio > 1.0f)	// 割合が1超えたら
	{
		if (pToNode->GetIsLoop())
			curToRatio -= 1.0f;
		else
			curToRatio = 1.0f;
	}

	// 補間したトランスフォームをボーンに適用させる
	u_int boneCnt = _updateBones.GetBoneCnt();
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		BoneTransform fromTransform;
		pFromNode->GetAnimTransform(fromTransform, b_i, curFromRatio);

		BoneTransform toTransform;
		pToNode->GetAnimTransform(toTransform, b_i, curToRatio);

		BoneTransform interpTransform;
		Interpolate(fromTransform, toTransform, interpTransform);

		Bone& bone = _updateBones.GetBone(b_i);
		bone.SetAnimTransform(interpTransform);
	}
}
