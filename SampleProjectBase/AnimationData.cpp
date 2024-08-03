#include "pch.h"
#include "AnimationData.h"

using namespace DirectX::SimpleMath;

void AnimationData::AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode)
{
	pAnimChannels.push_back(std::move(_pAnimNode));
}

const AnimationChannel* AnimationData::FindChannel(u_int _boneIdx) const
{
	auto itr = std::find_if(pAnimChannels.begin(), pAnimChannels.end(),
		[&](const std::unique_ptr<AnimationChannel>& channel)
		{
			return channel->GetBodeIdx() == _boneIdx;
		});

	if (itr == pAnimChannels.end())
	{
		return nullptr;
	}

	return (*itr).get();
}

void AnimationData::SetAnimationTime(float _animTime)
{
	assert(_animTime >= 0.0f);
	animationTime_s = std::max(_animTime, 0.0f);
}

void AnimationData::SetTimePerKey(float _timePerKey)
{
	assert(_timePerKey >= 0.0f);
	timePerKey_s = std::max(_timePerKey, 0.0f);
}

std::string AnimationData::GetBoneName(u_int _nodeId) const
{
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "ノードIDが配列外を指しています");

	return pAnimChannels[_nodeId]->GetName();
}

u_int AnimationData::GetBoneIdx(u_int _nodeId) const
{
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "ノードIDが配列外を指しています");

	return pAnimChannels[_nodeId]->GetBodeIdx();
}

u_int AnimationData::GetChannelCount() const
{
	return static_cast<u_int>(pAnimChannels.size());
}

DirectX::SimpleMath::Vector3 AnimationData::GetScale(u_int _boneId, float _playingTime) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// 対応したボーンがないなら
	if (channel == nullptr) return Vector3::One;

	if (channel->GetScaleKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel->GetScaleKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevScaleKey(_playingTime, timePerKey_s);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel->GetScaleKeyCnt())
		nextKeyNum = 0;

	const AnimKey_V3& prevKey = channel->GetScaleKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel->GetScaleKey(nextKeyNum);

	float deltaTime = (nextKey.startKeyNum - prevKey.startKeyNum) * timePerKey_s;

	// 割合
	float ratio = (_playingTime - prevKey.startKeyNum * timePerKey_s) / deltaTime;

	// 線形補間
	Vector3 lerpedScale = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return lerpedScale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternion(u_int _boneId, float _playingTime) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// 対応したボーンがないなら
	if (channel == nullptr) return Quaternion::Identity;

	if (channel->GetQuatKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel->GetQuatKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevQuatKey(_playingTime, timePerKey_s);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel->GetQuatKeyCnt())
		nextKeyNum = 0;

	const AnimKey_Q& prevKey = channel->GetQuatKey(prevKeyNum);
	const AnimKey_Q& nextKey = channel->GetQuatKey(nextKeyNum);

	float deltaTime = (nextKey.startKeyNum - prevKey.startKeyNum) * timePerKey_s;

	// 割合
	float ratio = (_playingTime - prevKey.startKeyNum * timePerKey_s) / deltaTime;

	// 球面線形補間
	Quaternion slerpedQuat = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

	return slerpedQuat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPosition(u_int _boneId, float _playingTime) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// 対応したボーンがないなら
	if (channel == nullptr) return Vector3::Zero;

	if (channel->GetPosKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel->GetPosKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevPosKey(_playingTime, timePerKey_s);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel->GetPosKeyCnt())
		nextKeyNum = 0;

	const AnimKey_V3& prevKey = channel->GetPosKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel->GetPosKey(nextKeyNum);

	float deltaTime = (nextKey.startKeyNum - prevKey.startKeyNum) * timePerKey_s;

	// 割合
	float ratio = (_playingTime - prevKey.startKeyNum * timePerKey_s) / deltaTime;

	// 線形補間
	Vector3 lerpedPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return lerpedPos;
}

float AnimationData::GetAnimationTime() const
{
	return animationTime_s;
}
