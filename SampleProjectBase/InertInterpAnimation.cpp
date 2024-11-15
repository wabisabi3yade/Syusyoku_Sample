#include "pch.h"
#include "InertInterpAnimation.h"

#include "SingleAnimationNode.h"

#include "Bone.h"

using namespace DirectX::SimpleMath;

InertInterpAnimation::InertInterpAnimation()
	: transitionElapsedTime(0.0f), transitionTime(0.0f)
{
}

bool InertInterpAnimation::Calculate(const std::vector<BoneTransform>& _nextAnimation, float _blendTime)
{
	// キャッシュがないなら
	if (!secondLastBoneCache.isEnable) return false;

	// 初期化
	transitionElapsedTime = 0.0f;
	transitionTime = _blendTime;
	positionTransition.clear();
	scaleTransition.clear();
	rotationTransition.clear();

	u_int boneCnt = static_cast<u_int>(lastBoneCache.transform.size());
	positionTransition.resize(boneCnt);
	scaleTransition.resize(boneCnt);
	rotationTransition.resize(boneCnt);

	changeTimeTransform.clear();
	changeTimeTransform.resize(boneCnt);

	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		// 遷移時のトランスフォームを取得
		changeTimeTransform[b_i] = lastBoneCache.transform[b_i];

		// ボーンの慣性補間の初期処理する
		BoneInitTransition(b_i, _nextAnimation, _blendTime);
	}

	return true;
}

void InertInterpAnimation::UpdateBoneCache(std::vector<BoneTransform>& boneTransforms, float _deltaTime)
{
	if (lastBoneCache.isEnable)
	{
		secondLastBoneCache = std::move(lastBoneCache);
	}

	lastBoneCache.transform = std::move(boneTransforms);
	lastBoneCache.deltaTime_s = _deltaTime;
	lastBoneCache.isEnable = true;
}

DirectX::SimpleMath::Vector3 InertInterpAnimation::CalcBlendPos(u_int _boneIdx, float _blendingTime)
{
	float blendValue = CalcBlend(positionTransition[_boneIdx], _blendingTime);

	Vector3 blendPos = blendValue * positionTransition[_boneIdx].normalizeVector +
		changeTimeTransform[_boneIdx].position;

	return blendPos;
}

DirectX::SimpleMath::Vector3 InertInterpAnimation::CalcBlendScale(u_int _boneIdx, float _blendingTime)
{
	float blendValue = CalcBlend(scaleTransition[_boneIdx], _blendingTime);

	Vector3 blendScale = blendValue * scaleTransition[_boneIdx].normalizeVector +
		changeTimeTransform[_boneIdx].scale;

	return blendScale;
}

DirectX::SimpleMath::Quaternion InertInterpAnimation::CalcBlendRot(u_int _boneIdx, float _blendingTime)
{
	QuatTransition qT = rotationTransition[_boneIdx];
	float blendValue = CalcBlend(qT, _blendingTime);

	Quaternion q = Quaternion::CreateFromAxisAngle(qT.axis, blendValue);

	return Quat::Multiply(q, changeTimeTransform[_boneIdx].rotation);
}

float InertInterpAnimation::ProgressTransitionTime(float _deltaTime)
{
	transitionElapsedTime += _deltaTime;

	return transitionElapsedTime;
}

bool InertInterpAnimation::GetIsTransitionEnd()
{
	return transitionElapsedTime >= transitionTime;
}

float InertInterpAnimation::GetElapsedTime() const
{
	return transitionElapsedTime;
}

float InertInterpAnimation::GetTransitionTime() const
{
	return transitionTime;
}

void InertInterpAnimation::BoneInitTransition(u_int _boneIdx, const std::vector<BoneTransform>& _requestData, float _blendTime)
{
	const BoneTransform& requestTransform = _requestData[_boneIdx];

	BoneTransform last = lastBoneCache.transform[_boneIdx];
	BoneTransform secondLast = secondLastBoneCache.transform[_boneIdx];

	// 座標
	InitTransition(positionTransition[_boneIdx], requestTransform.position, last.position, secondLast.position, _blendTime);

	// スケール
	InitTransition(scaleTransition[_boneIdx], requestTransform.scale, last.scale, secondLast.scale, _blendTime);

	// 回転
	InitTransition(rotationTransition[_boneIdx], requestTransform.rotation, last.rotation, secondLast.rotation, _blendTime);

}

void InertInterpAnimation::InitTransition(VectorTransition& _transition, const DirectX::SimpleMath::Vector3& _requestPose, const DirectX::SimpleMath::Vector3& _lastPose, const DirectX::SimpleMath::Vector3& _secondLastPose, float _blendTime)
{

	assert(lastBoneCache.deltaTime_s > 0.0f);

	// 遷移先までの差分
	Vector3 vecX0 = _lastPose - _requestPose;
	_transition.x0 = vecX0.Length();

	if (_transition.x0 > Mathf::epsilon)	// 限りなく0に近くないなら
	{
		vecX0.Normalize(_transition.normalizeVector);

		Vector3 vecXn1 = _secondLastPose - _requestPose;
		float xn1 = vecXn1.Dot(_transition.normalizeVector);

		_transition.v0 = (_transition.x0 - xn1) / lastBoneCache.deltaTime_s;
	}
	else	// 0に近いなら
	{
		_transition.normalizeVector = Vector3::Zero;
		_transition.v0 = 0.0f;
	}

	// 残りの値を計算する
	CalcTransition(_transition, _blendTime);
}


void InertInterpAnimation::InitTransition(QuatTransition& _transition, const DirectX::SimpleMath::Quaternion& _requestPose, const DirectX::SimpleMath::Quaternion& _lastPose, const DirectX::SimpleMath::Quaternion& _secondLastPose, float _blendTime)
{
	assert(lastBoneCache.deltaTime_s > 0.0f);

	Quaternion invRequestPose;
	_requestPose.Inverse(invRequestPose);

	Quaternion q0 = Quat::Multiply(_lastPose, invRequestPose);

	Quat::ToAxisAngle(q0, _transition.axis, _transition.x0);
	_transition.axis.Normalize();
	_transition.x0 = Mathf::Repeat(_transition.x0 + Mathf::PI, Mathf::PI * 2.0f) - Mathf::PI;

	Quaternion qn1 = Quat::Multiply(_secondLastPose, invRequestPose);

	float xn1 = Mathf::PI;

	if (abs(qn1.w) > Mathf::epsilon)
	{
		Vector3 q_xyz = Vector3(qn1.x, qn1.y, qn1.z);

		xn1 = 2.0f * atan(q_xyz.Dot(_transition.axis) / qn1.w);
		xn1 = Mathf::Repeat(xn1 + Mathf::PI, Mathf::PI * 2) - Mathf::PI;
	}

	float deltaAngle = _transition.x0 - xn1;
	deltaAngle = Mathf::Repeat(deltaAngle + Mathf::PI, Mathf::PI * 2.0f) - Mathf::PI;

	_transition.v0 = deltaAngle / lastBoneCache.deltaTime_s;

	CalcTransition(_transition, _blendTime);
}

float InertInterpAnimation::CalcBlend(const TransitionBase& _transition, float _blendingTime)
{
	float t_1 = std::min(_blendingTime, _transition.t1);
	float t_2 = t_1 * t_1;
	float t_3 = t_1 * t_2;
	float t_4 = t_1 * t_3;
	float t_5 = t_1 * t_4;

	return (_transition.A * t_5) + (_transition.B * t_4) + (_transition.C * t_3) + (0.5f * _transition.a0 * t_2) + (_transition.v0 * t_1);
}

void InertInterpAnimation::CalcTransition(TransitionBase& _transition, float _blendTime)
{
	if (_transition.x0 <= Mathf::epsilon)
	{
		_transition.t1 = 0.0f;
		_transition.a0 = 0.0f;
		_transition.A = 0.0f;
		_transition.B = 0.0f;
		_transition.C = 0.0f;
	}
	else
	{
		if (_transition.v0 != 0.0f && (_transition.x0 / _transition.v0) < 0.0f)
		{
			_transition.t1 = std::min(_blendTime, -5.0f * _transition.x0 / _transition.v0);
		}
		else
		{
			_transition.t1 = _blendTime;
		}

		// 時間の累乗を計算
		float t1_2 = _transition.t1 * _transition.t1;
		float t1_3 = t1_2 * _transition.t1;
		float t1_4 = t1_3 * _transition.t1;
		float t1_5 = t1_4 * _transition.t1;

		_transition.a0 = ((-8.f * _transition.v0 * _transition.t1) + (-20.f * _transition.x0)) / t1_2;

		_transition.A = -((1.f * _transition.a0 * _transition.t1 * _transition.t1) + (6.f * _transition.v0 * _transition.t1) + (12.f * _transition.x0)) / (2.f * t1_5);
		_transition.B = ((3.f * _transition.a0 * t1_2) + (16.f * _transition.v0 * _transition.t1) + (30.f * _transition.x0)) / (2.f * t1_4);
		_transition.C = -((3.f * _transition.a0 * t1_2) + (12.f * _transition.v0 * _transition.t1) + (20.f * _transition.x0)) / (2.f * t1_3);
	}
}