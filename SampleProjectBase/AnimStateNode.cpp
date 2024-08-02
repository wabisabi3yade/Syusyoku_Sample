#include "pch.h"
#include "AnimStateNode.h"

// �������
#include "InertialInterpolation.h"

using namespace DirectX::SimpleMath;

void AnimStateNode::SetAnimationData(AnimationData& _animData)
{
	pAnimationData = &_animData;
}

void AnimStateNode::UpdateCache(std::vector<BoneTransform>& boneTransforms, float _deltaTime)
{
	if (lastBoneCache.isEnable)
	{
		secondLastBoneCache = std::move(lastBoneCache);
	}

	lastBoneCache.transform = std::move(boneTransforms);
	lastBoneCache.deltaTime_s = _deltaTime;
	lastBoneCache.isEnable = true;
}

const AnimationData& AnimStateNode::GetAnimationData()
{
	return *pAnimationData;
}

std::vector<BoneTransform> AnimStateNode::CalcTranslation(AnimStateNode& _nextAnimation, float _blendTime)
{



	return std::vector<BoneTransform>();
}

std::vector<BoneTransform> AnimStateNode::InertInterpolation(AnimStateNode& _nextAnimation, float _blendTime)
{
	struct InertBase
	{
		float x0;   // ��������
		float v0;   // �������x
		float t1;   // �J�ڏI���̎���
		float a0;   // ���������x
		float A;
		float B;
		float C;
	};

	assert(_blendTime > 0.0f);



	//Vector3 vec = curValue - nextValue;
	//x0 = vec.magnitude;
	//if (x0 > Mathf.Epsilon) {
	//	baseVec = vec.normalized;
	//	var xn1 = Vector3.Dot(prevValue - nextValue, baseVec);
	//	v0 = (x0 - xn1) / deltaTime;
	//}
	//else {
	//	baseVec = Vector3.zero;
	//	v0 = 0;
	//}

	//calculate(time);

	return std::vector<BoneTransform>();
}
